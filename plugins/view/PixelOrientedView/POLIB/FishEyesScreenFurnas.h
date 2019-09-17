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

#ifndef FISHEYESSCREENFURNAS_H
#define FISHEYESSCREENFURNAS_H
#include <string>
#include "ScreenFunction.h"

namespace pocore {

class FishEyesScreenFurnas : public ScreenFunction {

public:
  FishEyesScreenFurnas();
  Vec2f project(const Vec2f &) const override;
  Vec2f unproject(const Vec2f &) const override;
  void setCenter(double x, double y);
  void setRadius(double r);
  void setHeight(double h);

private:
  double R;
  double k;
  double l;
  Vec2f fisheyesCenter;
};
} // namespace pocore
#endif // FISHEYESSCREENFURNAS_H
