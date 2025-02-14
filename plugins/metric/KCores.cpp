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

#include <talipot/DoubleProperty.h>
#include <talipot/StringCollection.h>
#include <talipot/GraphMeasure.h>

#include <climits>

using namespace std;
using namespace tlp;

/** \addtogroup metric */
/*@{*/
/**
 * \file
 * \brief A metric based on the K-core decomposition of a graph.
 *
 * K-cores were first introduced in:
 *
 * S. B. Seidman, "Network structure and minimum degree",
 * Social Networks 5:269-287, 1983
 *
 * This is a method for simplifying a graph topology which helps in analysis
 * and visualization of social networks
 *
 * (see http://en.wikipedia.org/wiki/K-core for more details)
 *
 * The K-Cores metric can also be computed according to weighted degrees. See :
 *
 * C. Giatsidis, D. Thilikos, M. Vazirgiannis, \n
 * "Evaluating cooperation in communities with the k-core structure",\n
 * "Proceedings of the 2011 International Conference on Advances in Social Networks Analysis and
 * Mining (ASONAM)",\n
 * "2011"
 *
 * \note Use the default parameters to compute simple K-Cores (undirected and unweighted)
 *
 *  <b>HISTORY</b>
 *
 *  - 2006 Version 1.0 by David Auber, LaBRI,
 *  University Bordeaux I, France
 *  - 2011 Version 2.0: Add In/Out and Weighted computation features
 *  by François Queyroi, LaBRI, University Bordeaux I, France
 *  - 2015 Performance optimization by Patrick Mary
 *
 *
 */
class KCores : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("K-Cores", "David Auber", "28/05/2006",
                    "Node partitioning measure based on the K-core decomposition of a graph.<br/>"
                    "K-cores were first introduced in:<br/><b>Network structure and minimum "
                    "degree</b>, S. B. Seidman, Social Networks 5:269-287 (1983).<br/>"
                    "This is a method for simplifying a graph topology which helps in analysis and "
                    "visualization of social networks.<br>"
                    "<b>Note</b>: use the default parameters to compute simple K-Cores (undirected "
                    "and unweighted).",
                    "2.0", "Graph")

  KCores(const tlp::PluginContext *context);
  ~KCores() override;
  bool run() override;
};

//========================================================================================
static const char *paramHelp[] = {
    // direction
    "This parameter indicates the direction used to compute K-Cores values.",

    // metric
    "An existing edge metric property, used to specify the weights of edges."};
#define DEGREE_TYPE "type"
#define DEGREE_TYPES "InOut;In;Out;"
#define INOUT 0
#define IN 1
#define OUT 2
//========================================================================================
KCores::KCores(const PluginContext *context) : DoubleAlgorithm(context) {
  addInParameter<StringCollection>(DEGREE_TYPE, paramHelp[0], DEGREE_TYPES, true,
                                   "InOut <br> In <br> Out");
  addInParameter<NumericProperty *>("metric", paramHelp[1], "", false);
}
//========================================================================================
KCores::~KCores() {}
//========================================================================================
bool KCores::run() {
  NumericProperty *metric = nullptr;
  StringCollection degreeTypes(DEGREE_TYPES);
  degreeTypes.setCurrent(0);

  if (dataSet != nullptr) {
    dataSet->get(DEGREE_TYPE, degreeTypes);
    dataSet->get("metric", metric);
  }

  EDGE_TYPE degree_type = static_cast<EDGE_TYPE>(degreeTypes.getCurrent());

  // the famous k
  double k = DBL_MAX;
  // use two NodeStaticProperty to hold the nodes infos
  // because the more k increase the more nodes are "deleted"
  NodeStaticProperty<bool> nodeDeleted(graph);
  NodeStaticProperty<double> nodeK(graph);
  degree(graph, nodeK, degree_type, metric, false);
  const std::vector<node> &nodes = graph->nodes();
  // the number of non deleted nodes
  unsigned int nbNodes = nodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    k = std::min(k, nodeK[i]);
    nodeDeleted[i] = false;
  }

  bool noEdgeCheck = (graph == graph->getRoot());

  // loop on remaining nodes
  while (nbNodes) {
    bool modify = true;
    double next_k = DBL_MAX;

    while (modify) {
      modify = false;

      // finally set the values
      for (unsigned int i = 0; i < nodes.size(); ++i) {
        // nothing to do if the node
        // is already deleted
        if (nodeDeleted[i])
          continue;

        double &nK = nodeK[i];
        double current_k = nK;
        node n = nodes[i];

        if (current_k <= k) {
          nK = k;
          // decrease neighbours weighted degree
          const std::vector<edge> &edges = graph->allEdges(n);
          unsigned int nbEdges = edges.size();

          for (unsigned int j = 0; j < nbEdges; ++j) {
            edge ee = edges[j];

            if (noEdgeCheck || graph->isElement(ee)) {
              auto ends = graph->ends(ee);
              node m;

              switch (degree_type) {
              case IN_EDGE:
                if ((m = ends.second) == n)
                  continue;

                break;

              case OUT_EDGE:
                if ((m = ends.first) == n)
                  continue;

                break;

              default:
                m = (ends.first == n) ? ends.second : ends.first;
              }

              unsigned int mPos = graph->nodePos(m);

              if (nodeDeleted[mPos])
                continue;

              nodeK[mPos] -= metric ? metric->getEdgeDoubleValue(ee) : 1;
            }
          }

          // mark node as deleted
          nodeDeleted[i] = true;
          --nbNodes;
          modify = true;
        } else if (current_k < next_k)
          // update next k value
          next_k = current_k;
      }
    }

    k = next_k;
  }

  // finally set the result values
  nodeK.copyToProperty(result);

  return true;
}
//========================================================================================
PLUGIN(KCores)
