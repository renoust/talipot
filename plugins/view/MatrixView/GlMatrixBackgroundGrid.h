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

#ifndef GLMATRIXBACKGROUNDGRID_H
#define GLMATRIXBACKGROUNDGRID_H

#include <tulip/GlSimpleEntity.h>
namespace tlp {
class MatrixView;

class GlMatrixBackgroundGrid : public tlp::GlSimpleEntity {
public:
  GlMatrixBackgroundGrid(MatrixView *view);
  tlp::BoundingBox getBoundingBox() override;
  void draw(float lod, tlp::Camera *camera) override;

  void getXML(std::string &) override {}
  void setWithXML(const std::string &, unsigned int &) override {}

private:
  MatrixView *_view;
};
} // namespace tlp
#endif // GLMATRIXBACKGROUNDGRID_H
