/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <stack>
#include <queue>
#include <unordered_map>
#include <talipot/DoubleProperty.h>
#include <talipot/StaticProperty.h>
#include <talipot/MutableContainer.h>
#include <talipot/GraphTools.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // directed
    "Indicates if the graph should be considered as directed or not.",

    // norm
    "If true the node measure will be normalized<br>"
    " - if not directed : m(n) = 2*c(n) / (#V - 1)(#V - 2)<br>"
    " - if directed     : m(n) = c(n) / (#V - 1)(#V - 2)<br>"
    "If true the edge measure will be normalized<br>"
    " - if not directed : m(e) = 2*c(e) / (#V / 2)(#V / 2)<br>"
    " - if directed     : m(e) = c(e) / (#V / 2)(#V / 2)",

    // weight
    "An existing edge weight metric property.",

    // Average path length
    "The computed average path length (-1 if not computed)"};

/** \addtogroup metric */

/** This plugin is an implementation of betweenness centrality parameter.
 *  (see http://en.wikipedia.org/wiki/Centrality#Betweenness_centrality for more details)
 *
 *  Algorithm published by:
 *
 *  U. Brandes, \n
 *  "A Faster Algorithm for Betweenness Centrality", \n
 *  "Journal of Mathematical Sociology" \n
 *  "2001", \n
 *  volume 25, \n
 *  pages 163-177
 *
 *  The edge betweenness centrality is also computed, it is described in :
 *
 *  Newman, M. E. J. and Girvan, M. \n
 *  "Finding and evaluating community structure in networks",  \n
 *  "Physics Reviews E",  \n
 *  "2004",  \n
 *  volume 69
 *
 *
 *  \note The complexity of the algorithm is O(|V| * |E|) in time
 *        on unweighted graphs and O(|V||E| + |V|^2 log |V|) on
 *        weighted graphs.
 *
 *  <b>HISTORY</b>
 *
 *  - 26/04/19 Version 1.3: Weighted version
 *  - 16/02/11 Version 1.2: Edge betweenness computation added
 *  - 08/02/11 Version 1.1: Normalisation option added
 *  - 03/01/05 Version 1.0: Initial release
 *
 */
class BetweennessCentrality : public DoubleAlgorithm {
public:
  PLUGININFORMATION("Betweenness Centrality", "David Auber", "03/01/2005",
                    "Computes the betweenness centrality.", "1.3", "Graph")
  BetweennessCentrality(const PluginContext *context) : DoubleAlgorithm(context) {
    addInParameter<bool>("directed", paramHelp[0], "false");
    addInParameter<bool>("norm", paramHelp[1], "false", false);
    addInParameter<NumericProperty *>("weight", paramHelp[2], "", false);
    addOutParameter<double>("average path length", paramHelp[3], "-1");
  }
  bool run() override {
    result->setAllNodeValue(0.0);
    result->setAllEdgeValue(0.0);
    bool directed = false;
    bool norm = false;
    NumericProperty *weight = nullptr;

    if (dataSet != nullptr) {
      dataSet->get("directed", directed);
      dataSet->get("norm", norm);
      dataSet->get("weight", weight);
    }

    // Metric is 0 in this case
    if (graph->numberOfNodes() <= 2)
      return true;

    // Edges weights should be positive
    if (weight && weight->getEdgeDoubleMin() <= 0) {
      pluginProgress->setError("Edges weights should be positive.");
      return false;
    }

    double avg_path_length = 0.;
    unsigned int nbNodes = graph->numberOfNodes();
    unsigned int count = 0;

    pluginProgress->showPreview(false);

    for (auto s : graph->nodes()) {

      if (((++count % 50) == 0) && (pluginProgress->progress(count, nbNodes) != TLP_CONTINUE))
        break;

      stack<node> S;
      unordered_map<node, list<node>> P;
      MutableContainer<int> sigma;

      if (weight)
        computeDijkstra(s, directed, weight, S, P, sigma);
      else
        computeBFS(s, directed, S, P, sigma);

      MutableContainer<double> delta;
      delta.setAll(0.0);

      while (!S.empty()) {
        node w = S.top();
        double wD = delta.get(w.id);
        S.pop();

        for (auto v : P[w]) {
          double vd = double(sigma.get(v.id)) / double(sigma.get(w.id)) * (1.0 + wD);
          delta.add(v.id, vd);
          edge e = graph->existEdge(v, w, directed);

          if (e.isValid()) {
            result->setEdgeValue(e, result->getEdgeValue(e) + vd);
            if (weight)
              avg_path_length += vd * weight->getEdgeDoubleValue(e);
            else
              avg_path_length += vd;
          }
        }

        if (w != s)
          result->setNodeValue(w, result->getNodeValue(w) + wD);
      }
    }

    if (pluginProgress->state() != TLP_CONTINUE)
      return pluginProgress->state() != TLP_CANCEL;

    // Normalization
    if (norm || !directed) {
      double n = graph->numberOfNodes();
      const double nNormFactor = 1.0 / ((n - 1) * (n - 2));

      for (auto s : graph->nodes()) {

        // In the undirected case, the metric must be divided by two, then
        if (norm)
          result->setNodeValue(s, result->getNodeValue(s) * nNormFactor);

        if (!directed)
          result->setNodeValue(s, result->getNodeValue(s) * 0.5);
      }

      const double eNormFactor = 4.0 / (n * n);

      for (auto e : graph->edges()) {

        if (norm)
          result->setEdgeValue(e, result->getEdgeValue(e) * eNormFactor);

        if (!directed)
          result->setEdgeValue(e, result->getEdgeValue(e) * 0.5);
      }
    }
    avg_path_length /= (nbNodes * (nbNodes - 1.));
    dataSet->set("average path length", avg_path_length);

    return pluginProgress->state() != TLP_CANCEL;
  }

private:
  void computeBFS(node s, bool directed, stack<node> &S, unordered_map<node, list<node>> &P,
                  MutableContainer<int> &sigma) {
    sigma.setAll(0);
    sigma.set(s.id, 1);
    MutableContainer<int> d;
    d.setAll(-1);
    d.set(s.id, 0);
    queue<node> Q;
    Q.push(s);

    while (!Q.empty()) {
      node v = Q.front();
      int vd = d.get(v.id);
      int vs = sigma.get(v.id);
      Q.pop();
      S.push(v);

      for (auto w : (directed ? graph->getOutNodes(v) : graph->getInOutNodes(v))) {
        int wd = d.get(w.id);

        if (wd < 0) {
          Q.push(w);
          d.set(w.id, wd = vd + 1);
        }

        if (wd == vd + 1) {
          sigma.add(w.id, vs);
          P[w].push_back(v);
        }
      }
    }
  }

  void computeDijkstra(node s, bool directed, NumericProperty *weight, stack<node> &S,
                       unordered_map<node, list<node>> &P, MutableContainer<int> &sigma) {
    EdgeStaticProperty<double> eWeights(graph);
    eWeights.copyFromNumericProperty(weight);
    NodeStaticProperty<double> nodeDistance(graph);
    tlp::computeDijkstra(graph, s, eWeights, nodeDistance, directed ? DIRECTED : UNDIRECTED, P, &S,
                         &sigma);
  }
};

PLUGIN(BetweennessCentrality)
