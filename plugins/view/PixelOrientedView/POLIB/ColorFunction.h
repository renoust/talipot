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

#ifndef COLORFUNCTION_H
#define COLORFUNCTION_H

#include "potypes.h"

namespace pocore {
class ColorFunction {
public:
  virtual ~ColorFunction() {}
  virtual RGBA getColor(const double &value, const unsigned int itemId) const = 0;
};
} // namespace pocore
#endif // COLORFUNCTION_H
