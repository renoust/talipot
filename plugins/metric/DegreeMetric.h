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
#ifndef _DEGREEMETRIC_H
#define _DEGREEMETRIC_H

#include <tulip/DoubleProperty.h>

/** \addtogroup metric */

/** This plugin compute the degree of each node
 *
 *  \note This plug-in exists to obtain a uniform interface in the
 *  Tulip graph visualization software. To access to the degree
 *  of a node it is recommended to use directly the degree function
 *  available in each graph.
 *
 */
class DegreeMetric : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Degree", "David Auber", "04/10/2001", "Assigns its degree to each node.",
                    "1.0", "Graph")
  DegreeMetric(const tlp::PluginContext *context);
  bool run() override;
  bool check(std::string &errorMsg) override;
};

#endif
