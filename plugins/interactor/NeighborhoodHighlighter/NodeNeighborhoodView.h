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

#ifndef REACHABLESUBGRAPHVIEW_H_
#define REACHABLESUBGRAPHVIEW_H_

#include <unordered_map>
#include <tulip/GraphDecorator.h>

namespace tlp {
class DoubleProperty;
}

using namespace tlp;

class NodeNeighborhoodView : public GraphDecorator {

public:
  enum NeighborNodesType { IN_NEIGHBORS, OUT_NEIGHBORS, IN_OUT_NEIGHBORS };

  NodeNeighborhoodView(Graph *graph, node n,
                       NeighborNodesType neighborsNodesType = IN_OUT_NEIGHBORS,
                       unsigned int neighborhoodDist = 1, bool computeReachableSubGraph = false,
                       const std::string &propertyName = "", int nbNodes = 0);

  void updateWithDistance(const unsigned int dist);

  bool isElement(const node n) const override;
  bool isElement(const edge e) const override;
  unsigned int nodePos(const node n) const override;
  unsigned int edgePos(const edge e) const override;

  Iterator<node> *getNodes() const override;
  Iterator<node> *getInNodes(const node n) const override;
  Iterator<node> *getOutNodes(const node n) const override;
  Iterator<node> *getInOutNodes(const node n) const override;
  Iterator<edge> *getEdges() const override;
  Iterator<edge> *getOutEdges(const node n) const override;
  Iterator<edge> *getInOutEdges(const node n) const override;
  Iterator<edge> *getInEdges(const node n) const override;

  const std::vector<node> &nodes() const override {
    return graphViewNodes;
  }

  unsigned int numberOfNodes() const override {
    return graphViewNodes.size();
  }

  const std::vector<edge> &edges() const override {
    return graphViewEdges;
  }

  unsigned int numberOfEdges() const override {
    return graphViewEdges.size();
  }

  Graph *getRoot() const override {
    return const_cast<NodeNeighborhoodView *>(this);
  }

private:
  void getNeighbors(node n, unsigned int dist, bool noRecursion = false);
  void getInNeighbors(node n, unsigned int dist, bool noRecursion = false);
  void getOutNeighbors(node n, unsigned int dist, bool noRecursion = false);

  node centralNode;

  std::vector<node> graphViewNodes;
  std::vector<edge> graphViewEdges;

  std::unordered_map<unsigned int, std::vector<node>> nodesAtDist;
  std::unordered_map<unsigned int, std::vector<edge>> edgesAtDist;

  NeighborNodesType neighborsType;
  unsigned int currentDist;
  bool computeReachableSubGraph;
  int nbNodes;
  tlp::DoubleProperty *property;
};

#endif /* REACHABLESUBGRAPHVIEW_H_ */
