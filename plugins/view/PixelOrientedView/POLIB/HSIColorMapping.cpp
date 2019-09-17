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

#include "HSIColorMapping.h"

namespace pocore {

HSIColorMapping::HSIColorMapping(const HSI &from, const HSI &to) : hsiColorScale(from, to) {}

RGBA HSIColorMapping::getColor(const double &value, const unsigned int) const {
  return hsiColorScale[value].getRGBA();
}
} // namespace pocore
