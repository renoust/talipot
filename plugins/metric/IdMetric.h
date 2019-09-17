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

#ifndef _IDMETRIC_H
#define _IDMETRIC_H

#include <tulip/DoubleProperty.h>

/** \addtogroup metric */

/** This plugins assigns id to nodes/edges or both. The ids are those used internally by Tulip.
 *
 */
class IdMetric : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Id", "David Auber", "06/04/2000", "Assigns their Tulip id to nodes and edges.",
                    "1.1", "Misc")
  IdMetric(const tlp::PluginContext *context);
  bool run() override;
};

#endif
