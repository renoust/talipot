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

#ifndef GRAPH_DIMENSION_H
#define GRAPH_DIMENSION_H

#include <talipot/Graph.h>

#include <string>
#include <map>

#include "DimensionBase.h"
#include "NodeMetricSorter.h"

class GraphDimension : public DimensionBase {

public:
  GraphDimension(tlp::Graph *graph, const std::string &dimensionName);
  ~GraphDimension() override;

  unsigned int numberOfItems() const override;
  unsigned int numberOfValues() const override;
  std::string getItemLabelAtRank(const unsigned int rank) const override;
  std::string getItemLabel(const unsigned int itemId) const override;
  double getItemValue(const unsigned int itemId) const override;
  double getItemValueAtRank(const unsigned int rank) const override;
  unsigned int getItemIdAtRank(const unsigned int rank) override;
  unsigned int getRankForItem(const unsigned int itemId) override;
  double minValue() const override;
  double maxValue() const override;
  std::vector<unsigned int> links(const unsigned int itemId) const override;
  std::string getDimensionName() const override {
    return dimName;
  }
  tlp::Graph *getGraph() const {
    return graph;
  }
  void updateNodesRank();

private:
  template <typename PROPERTY>
  double getNodeValue(const tlp::node n) const;

  tlp::Graph *graph;
  std::string dimName;
  std::string propertyType;
  std::vector<tlp::node> dataOrder;

  NodeMetricSorter *nodeSorter;
  static std::map<tlp::Graph *, unsigned int> graphDimensionsMap;
};

#endif // GRAPH_DIMENSION_H
