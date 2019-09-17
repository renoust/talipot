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

#ifndef _StrongComponent_H
#define _StrongComponent_H
#include <stack>
#include <unordered_map>
#include <tulip/TulipPluginHeaders.h>
struct NodeInfo {
  NodeInfo(int stra = 0, int sta = 0) : prefixOrder(stra), minAttach(sta) {}
  int prefixOrder;
  int minAttach;
};

/** This plugin is an implementation of a strongly connected components decomposition.
 *
 *  \note This algorithm assigns to each node a value defined as following : If two nodes are in the
 * same
 *  strongly connected component they have the same value else they have a
 *  different value.
 *
 */
class StrongComponent : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Strongly Connected Component", "David Auber", "12/06/2001",
                    "Implements a strongly connected components decomposition.", "1.0", "Component")
  StrongComponent(const tlp::PluginContext *context);
  ~StrongComponent() override;
  bool run() override;

private:
  int attachNumerotation(tlp::node, std::unordered_map<tlp::node, bool> &,
                         std::unordered_map<tlp::node, bool> &,
                         std::unordered_map<tlp::node, int> &, int &, std::stack<tlp::node> &,
                         int &);
};

#endif
