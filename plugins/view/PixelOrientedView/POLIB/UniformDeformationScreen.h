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

#ifndef UNIFORMDEFORMATIONSCREEN_H
#define UNIFORMDEFORMATIONSCREEN_H

#include <tulip/Matrix.h>
#include "ScreenFunction.h"

namespace pocore {
class UniformDeformationScreen : public ScreenFunction {
public:
  Vec2f project(const Vec2f &) const override;
  Vec2f unproject(const Vec2f &) const override;
  void setTranslation(double x, double y);
  void getTranslation(double &x, double &y);
  void setZoom(double zoom);
  double getZoom();
  UniformDeformationScreen();

private:
  void updateMatrix();

private:
  double _tx;
  double _ty;
  double _zoom;
  tlp::Matrix<double, 3> _mat;
  tlp::Matrix<double, 3> _invMat;
};
} // namespace pocore
#endif // UNIFORMDEFORMATIONSCREEN_H
