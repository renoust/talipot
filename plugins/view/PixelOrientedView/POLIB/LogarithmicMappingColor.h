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

#ifndef LOGARITHMIC_MAPPING_COLOR_H
#define LOGARITHMIC_MAPPING_COLOR_H

#include "potypes.h"
#include "LinearMappingColor.h"

namespace pocore {
class LogarithmicMappingColor : public LinearMappingColor {
public:
  LogarithmicMappingColor(const double &min, const double &max);
  RGBA getColor(const double &value, const unsigned int) const override;
};
} // namespace pocore
#endif // LOGARITHMIC_MAPPING_COLOR_H
