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
#ifndef _HierarchicalClustering_H
#define _HierarchicalClustering_H

#include <list>
#include <string>

#include <tulip/TulipPluginHeaders.h>

/** \addtogroup clustering */

class HierarchicalClustering : public tlp::Algorithm {
public:
  PLUGININFORMATION("Hierarchical", "David Auber", "27/01/2000",
                    "This algorithm divides the graph in 2 different subgraphs; the first one "
                    "contains the nodes which have their viewMetric value below the mean, and, the "
                    "other one, in which nodes have their viewMetric value above that mean value. "
                    "Then, the algorithm is recursively applied to this subgraph (the one with the "
                    "values above the threshold) until one subgraph contains less than 10 nodes.",
                    "1.0", "Clustering")
  HierarchicalClustering(tlp::PluginContext *context);
  ~HierarchicalClustering() override;
  bool run() override;

private:
  bool split(tlp::DoubleProperty *, std::list<tlp::node> &);
};

#endif
