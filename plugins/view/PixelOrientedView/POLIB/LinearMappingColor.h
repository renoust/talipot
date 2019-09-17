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

#ifndef LINEAR_MAPPING_COLOR_H
#define LINEAR_MAPPING_COLOR_H

#include "potypes.h"
#include "ColorFunction.h"

namespace pocore {
class LinearMappingColor : public ColorFunction {
public:
  LinearMappingColor(const double &min, const double &max);
  RGBA getColor(const double &value, const unsigned int) const override;

private:
  double _min;
  double _max;
  RGBA startColor;
  RGBA endColor;
};
} // namespace pocore
#endif // LINEAR_MAPPING_COLOR_H
