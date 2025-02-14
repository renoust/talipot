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

#include <map>

#include <talipot/GlColorScale.h>
#include <talipot/ColorScale.h>
#include <talipot/GlPolyQuad.h>

using namespace std;

namespace tlp {

GlColorScale::GlColorScale(ColorScale *colorScale, const Coord &baseCoord, const float length,
                           const float thickness, Orientation orientation)
    : colorScale(colorScale), baseCoord(baseCoord), length(length), thickness(thickness),
      colorScalePolyQuad(nullptr), orientation(orientation) {
  colorScale->addListener(this);
  updateDrawing();
}

GlColorScale::~GlColorScale() {
  delete colorScalePolyQuad;
}

void GlColorScale::setColorScale(ColorScale *scale) {
  colorScale->removeListener(this);
  colorScale = scale;
  colorScale->addListener(this);
  updateDrawing();
}

void GlColorScale::treatEvent(const Event &evt) {
  if (evt.type() == Event::TLP_MODIFICATION)
    updateDrawing();
}

void GlColorScale::updateDrawing() {
  delete colorScalePolyQuad;

  colorScalePolyQuad = new GlPolyQuad();

  if (!colorScale->colorScaleInitialized()) {
    colorScalePolyQuad->setOutlined(true);
    colorScalePolyQuad->setOutlineWidth(2);
  }

  Coord currentMin, currentMax;

  for (const auto &colorMapIt : colorScale->getColorMap()) {
    if (orientation == Vertical) {
      currentMin.set(baseCoord.getX() - thickness / 2,
                     baseCoord.getY() + colorMapIt.first * length);
      currentMax.set(baseCoord.getX() + thickness / 2,
                     baseCoord.getY() + colorMapIt.first * length);
    } else {
      currentMin.set(baseCoord.getX() + colorMapIt.first * length, baseCoord.getY() - thickness / 2,
                     0);
      currentMax.set(baseCoord.getX() + colorMapIt.first * length, baseCoord.getY() + thickness / 2,
                     0);
    }

    colorScalePolyQuad->addQuadEdge(currentMin, currentMax, colorMapIt.second);
  }

  boundingBox = colorScalePolyQuad->getBoundingBox();
}

void GlColorScale::draw(float lod, Camera *camera) {
  if (colorScalePolyQuad != nullptr) {
    colorScalePolyQuad->draw(lod, camera);
  }
}

Color GlColorScale::getColorAtPos(Coord pos) {
  float position = 0;

  if (orientation == GlColorScale::Vertical) {
    position = (pos.getY() - baseCoord.getY()) / length;
  } else {
    position = (pos.getX() - baseCoord.getX()) / length;
  }

  // Ensure to always have a valid position.
  if (position < 0) {
    position = 0;
  }

  if (position > 1) {
    position = 1;
  }

  return colorScale->getColorAtPos(position);
}

void GlColorScale::translate(const Coord &move) {
  if (colorScalePolyQuad != nullptr) {
    colorScalePolyQuad->translate(move);
    baseCoord += move;
    boundingBox = colorScalePolyQuad->getBoundingBox();
  }
}
}
