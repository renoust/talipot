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

#include <talipot/GlRegularPolygon.h>
#include <talipot/DrawingTools.h>

using namespace std;

namespace tlp {

GlRegularPolygon::GlRegularPolygon(const Coord &position, const Size &size,
                                   unsigned int numberOfSides, const Color &fillColor,
                                   const Color &outlineColor, bool filled, bool outlined,
                                   const string &textureName, float outlineSize)
    : position(position), size(size), numberOfSides(numberOfSides), startAngle(float(M_PI) / 2.0f) {
  computePolygon();

  invertYTexture = false;

  setFillColor(fillColor);
  setOutlineColor(outlineColor);
  setFillMode(filled);
  setOutlineMode(outlined);
  setTextureName(textureName);
  setOutlineSize(outlineSize);
}
//=====================================================
GlRegularPolygon::~GlRegularPolygon() {}
//=====================================================
void GlRegularPolygon::setStartAngle(float angle) {
  startAngle = angle;
  computePolygon();
}
//=====================================================
unsigned int GlRegularPolygon::getNumberOfSides() {
  return numberOfSides;
}
//=====================================================
void GlRegularPolygon::setNumberOfSides(unsigned int number) {
  numberOfSides = number;
  computePolygon();
}
//=====================================================
void GlRegularPolygon::resizePoints(const unsigned int number) {
  setNumberOfSides(number);
}
//=====================================================
void GlRegularPolygon::computePolygon() {
  boundingBox = BoundingBox();
  boundingBox.expand(position + size / 2.f);
  boundingBox.expand(position - size / 2.f);

  setPoints(computeRegularPolygon(numberOfSides, position, size, startAngle));

  clearGenerated();
}
}
