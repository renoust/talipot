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

#ifndef FISH_EYES_SCREEN_H
#define FISH_EYES_SCREEN_H

#include "ScreenFunction.h"

namespace pocore {

class FishEyesScreen : public ScreenFunction {

public:
  FishEyesScreen();
  Vec2f project(const Vec2f &) const override;
  Vec2f unproject(const Vec2f &) const override;
  void setCenter(double x, double y);
  void setRadius(double r);
  double getRadius();
  void setHeight(double h);

private:
  double R;
  double k;
  Vec2f fisheyesCenter;
};
} // namespace pocore
#endif // FISH_EYES_SCREEN_H
