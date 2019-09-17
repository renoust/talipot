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
#ifndef _KRUSKAL_H
#define _KRUSKAL_H

#include <tulip/BooleanProperty.h>

/** \addtogroup selection */

/// This selection plugins implements the so called Kruskal algorithm. This algorithm enables to
/// find a minimum spanning tree in a connected graph.
/**
 * This selection plugins enables to find all nodes and
 * edges at a fixed distance of a set of nodes.
 *
 * This only works on undirected graphs, (ie. the orientation of edges is omitted).
 *
 * It takes one parameter :
 * - DoubleProperty edge weight, this parameter defines the weight of each edge in the graph.
 *
 *  \author Anthony Don, LaBRI University Bordeaux I France:
 */
class Kruskal : public tlp::BooleanAlgorithm {

public:
  PLUGININFORMATION("Kruskal", "Anthony DON", "14/04/03",
                    "Implements the classical Kruskal algorithm to select a minimum spanning tree "
                    "in a connected graph."
                    "Only works on undirected graphs, (ie. the orientation of edges is omitted).",
                    "1.0", "Selection")
  Kruskal(const tlp::PluginContext *context);
  bool run() override;
  bool check(std::string &) override;
};

#endif
