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

#ifndef LAYOUTFUNCTION_H
#define LAYOUTFUNCTION_H
#include "potypes.h"

namespace pocore {
class LayoutFunction {
public:
  virtual ~LayoutFunction() {}
  virtual Vec2i project(const unsigned int id) const = 0;
  virtual unsigned int unproject(const Vec2i &) const = 0;
};
} // namespace pocore
#endif // LAYOUTFUNCTION_H
