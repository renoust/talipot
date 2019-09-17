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

#include "LogarithmicMappingColor.h"

namespace pocore {
LogarithmicMappingColor::LogarithmicMappingColor(const double &min, const double &max)
    : LinearMappingColor(log1p(min), log1p(max)) {}
RGBA LogarithmicMappingColor::getColor(const double &value, const unsigned int idColor) const {
  return LinearMappingColor::getColor(log1p(value), idColor);
}
} // namespace pocore
