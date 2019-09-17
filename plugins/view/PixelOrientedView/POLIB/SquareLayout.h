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

#ifndef SQUARELAYOUT_H
#define SQUARELAYOUT_H

#include "LayoutFunction.h"

namespace pocore {
class SquareLayout : public LayoutFunction {
public:
  SquareLayout(unsigned int widht);
  Vec2i project(const unsigned int id) const override;
  unsigned int unproject(const Vec2i &) const override;

private:
  unsigned int _width;
};
} // namespace pocore
#endif // SQUARELAYOUT_H
