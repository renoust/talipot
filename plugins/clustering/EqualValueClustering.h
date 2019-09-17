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
#ifndef _EqualValueClustering_H
#define _EqualValueClustering_H

#include <tulip/TulipPluginHeaders.h>
#include <tulip/NumericProperty.h>

class EqualValueClustering : public tlp::Algorithm {
public:
  PLUGININFORMATION("Equal Value", "Patrick Mary", "16/04/2014",
                    "Performs a graph clusterization grouping in the same cluster the nodes or "
                    "edges having the same value for a given property.",
                    "1.1", "Clustering")
  EqualValueClustering(tlp::PluginContext *context);
  bool run() override;
  bool computeClusters(tlp::NumericProperty *prop, bool onNodes, bool connected);
  bool computeClusters(tlp::PropertyInterface *prop, bool onNodes, bool connected);
};

#endif
