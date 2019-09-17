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

#ifndef PIXELORIENTEDDRAWING_H_
#define PIXELORIENTEDDRAWING_H_

#include "POLIB/TulipGraphDimension.h"
#include "POLIB/PixelOrientedMediator.h"

#include <tulip/GlSimpleEntity.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/Graph.h>
#include <tulip/GlRect.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlLabel.h>

#include <string>

namespace tlp {

class SizeProperty;
class LayoutProperty;

class PixelOrientedOverview : public GlComposite {

public:
  PixelOrientedOverview(pocore::TulipGraphDimension *data,
                        pocore::PixelOrientedMediator *pixelOrientedMediator, Coord blCornerPos,
                        const std::string &dimName, const Color &backgroundColor,
                        const Color &textColor);

  ~PixelOrientedOverview() override;

  pocore::TulipGraphDimension *getData() const {
    return data;
  }
  std::string getDimensionName() const {
    return dimName;
  }
  LayoutProperty *getPixelViewLayout() const {
    return pixelLayout;
  }
  SizeProperty *getPixelViewSize() const {
    return pixelSize;
  }

  void setBLCorner(const Coord &blCorner);

  void computePixelView(GlMainWidget *glWidget = nullptr);

  bool overviewGenerated() const {
    return overviewGen;
  }

  void setBackgroundColor(const Color &color) {
    backgroundColor = color;
  }
  void setTextColor(const Color &color) {
    textColor = color;
  }

private:
  void computeBoundingBox() {
    GlBoundingBoxSceneVisitor glBBSV(nullptr);
    acceptVisitor(&glBBSV);
    boundingBox = glBBSV.getBoundingBox();
  }

  pocore::TulipGraphDimension *data;
  pocore::PixelOrientedMediator *pixelOrientedMediator;

  GlGraphComposite *graphComposite;
  LayoutProperty *pixelLayout;
  SizeProperty *pixelSize;

  Coord blCornerPos;
  std::string textureName;
  std::string dimName;

  GlLabel *clickLabel;
  GlRect *backgroundRect;
  GlLabel *overviewLabel;
  GlRect *frame;
  GlRect *frame2;

  int overviewId;
  static int overviewCpt;

  bool overviewGen;

  Color backgroundColor, textColor;
};
} // namespace tlp
#endif /* PIXELORIENTEDDRAWING_H_ */
