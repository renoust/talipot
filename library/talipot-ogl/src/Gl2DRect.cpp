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

#include <talipot/Gl2DRect.h>
#include <talipot/Camera.h>
#include <talipot/GlXMLTools.h>

#include <limits>

using namespace std;

namespace tlp {

Gl2DRect::Gl2DRect()
    : GlRect(Coord(0.5, -0.5, 0), Coord(-0.5, 0.5, 0), Color(255, 255, 255, 255),
             Color(255, 255, 255, 255), true, false) {}

Gl2DRect::Gl2DRect(float top, float bottom, float left, float right, const std::string &textureName,
                   bool inPercent)
    : GlRect(Coord(0.5, -0.5, 0), Coord(-0.5, 0.5, 0), Color(255, 255, 255, 255),
             Color(255, 255, 255, 255), true, false),
      top(top), bottom(bottom), left(left), right(right), inPercent(inPercent), xInv(false),
      yInv(false) {
  this->textureName = textureName;
}

Gl2DRect::Gl2DRect(float bottom, float left, float height, float width,
                   const std::string &textureName, bool xInv, bool yInv)
    : GlRect(Coord(0.5, -0.5, 0), Coord(-0.5, 0.5, 0), Color(255, 255, 255, 255),
             Color(255, 255, 255, 255), true, false),
      top(bottom + height), bottom(bottom), left(left), right(left + width), inPercent(false),
      xInv(xInv), yInv(yInv) {
  this->textureName = textureName;
}

BoundingBox Gl2DRect::getBoundingBox() {
  BoundingBox bb;

  if (!inPercent) {
    bb.expand(Coord(left, bottom, 0));
    bb.expand(Coord(right, top, 0));
  } else {
    bb.expand(Coord(numeric_limits<float>::min(), numeric_limits<float>::min(), 0));
    bb.expand(Coord(numeric_limits<float>::max(), numeric_limits<float>::max(), 0));
  }

  return bb;
}

void Gl2DRect::draw(float lod, Camera *camera) {
  const Vec4i &viewport = camera->getViewport();

  Coord center;
  Size size;

  if (inPercent) {
    center =
        Coord(viewport[0] +
                  (left * (viewport[2] - viewport[0]) + right * (viewport[2] - viewport[0])) / 2.,
              viewport[1] +
                  (bottom * (viewport[3] - viewport[1]) + top * (viewport[3] - viewport[1])) / 2.,
              0);
    size = Size(right * (viewport[2] - viewport[0]) - left * (viewport[2] - viewport[0]),
                bottom * (viewport[3] - viewport[1]) - top * (viewport[3] - viewport[1]), 0);
  } else {
    float xMin;
    float xMax;
    float yMin;
    float yMax;

    if (!xInv) {
      xMin = left;
      xMax = right;
    } else {
      xMax = viewport[2] - left;
      xMin = viewport[2] - right;
    }

    if (!yInv) {
      yMin = bottom;
      yMax = top;
    } else {
      yMax = viewport[3] - bottom;
      yMin = viewport[3] - top;
    }

    center = Coord((xMin + xMax) / 2., (yMin + yMax) / 2., 0);
    size = Size(xMax - xMin, yMax - yMin, 0);
  }

  glPushMatrix();

  glTranslatef(center[0], center[1], center[2]);
  glScalef(size[0], size[1], 1);

  GlRect::draw(lod, nullptr);

  glPopMatrix();
}
//===========================================================
void Gl2DRect::translate(const Coord &vec) {
  if (!inPercent) {
    boundingBox.translate(vec);
    top += vec[1];
    bottom += vec[1];
    left += vec[0];
    right += vec[0];
  }
}
//===========================================================
void Gl2DRect::setTexture(const std::string &name) {
  textureName = name;
}
//===========================================================
string Gl2DRect::getTexture() {
  return textureName;
}
//===========================================================
void Gl2DRect::setCoordinates(float bottom, float left, float height, float width) {
  this->top = bottom;
  this->left = left;
  this->bottom = top + height;
  this->right = left + width;
}
//===========================================================
void Gl2DRect::getXML(string &outString) {

  GlXMLTools::createProperty(outString, "type", "Gl2DRect", "GlEntity");

  GlXMLTools::getXML(outString, "top", top);
  GlXMLTools::getXML(outString, "bottom", bottom);
  GlXMLTools::getXML(outString, "left", left);
  GlXMLTools::getXML(outString, "right", right);
  GlXMLTools::getXML(outString, "inPercent", inPercent);
  GlXMLTools::getXML(outString, "textureName", textureName);
  GlXMLTools::getXML(outString, "xInv", xInv);
  GlXMLTools::getXML(outString, "yInv", yInv);
}
//============================================================
void Gl2DRect::setWithXML(const string &inString, unsigned int &currentPosition) {

  GlXMLTools::setWithXML(inString, currentPosition, "top", top);
  GlXMLTools::setWithXML(inString, currentPosition, "bottom", bottom);
  GlXMLTools::setWithXML(inString, currentPosition, "left", left);
  GlXMLTools::setWithXML(inString, currentPosition, "right", right);
  GlXMLTools::setWithXML(inString, currentPosition, "inPercent", inPercent);
  GlXMLTools::setWithXML(inString, currentPosition, "textureName", textureName);
  GlXMLTools::setWithXML(inString, currentPosition, "xInv", xInv, false);
  GlXMLTools::setWithXML(inString, currentPosition, "yInv", yInv, false);
}
}
