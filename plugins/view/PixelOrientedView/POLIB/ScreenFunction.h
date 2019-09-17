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

#ifndef SCREENFUNCTION_H
#define SCREENFUNCTION_H

#include "potypes.h"

namespace pocore {
class ScreenFunction {

public:
  virtual ~ScreenFunction() {}
  virtual Vec2f project(const Vec2f &) const = 0;
  virtual Vec2f unproject(const Vec2f &) const = 0;
};
} // namespace pocore
#endif // SCREENFUNCTION_H
