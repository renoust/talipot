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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_GLSVGFEEDBACKBUILDER_H
#define Tulip_GLSVGFEEDBACKBUILDER_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <sstream>

#include <tulip/GlTLPFeedBackBuilder.h>
#include <tulip/Color.h>

namespace tlp {

class TLP_GL_SCOPE GlSVGFeedBackBuilder : public GlTLPFeedBackBuilder {

public:
  GlSVGFeedBackBuilder()
      : stream_out(std::stringstream::in | std::stringstream::out), inGlEntity(false),
        inGlGraph(false), inNode(false), inEdge(false) {}

  void begin(const Vector<int, 4> &viewport, GLfloat *clearColor, GLfloat pointSize,
             GLfloat lineWidth) override;
  void colorInfo(GLfloat *data) override;
  void beginGlEntity(GLfloat data) override;
  void endGlEntity() override;
  void beginGlGraph(GLfloat data) override;
  void endGlGraph() override;
  void beginNode(GLfloat data) override;
  void endNode() override;
  void beginEdge(GLfloat data) override;
  void endEdge() override;
  void pointToken(GLfloat *data) override;
  void lineToken(GLfloat *data) override;
  void lineResetToken(GLfloat *data) override;
  void polygonToken(GLfloat *data) override;
  void bitmapToken(GLfloat *data) override;
  void drawPixelToken(GLfloat *data) override;
  void copyPixelToken(GLfloat *data) override;
  void end() override;

  void getResult(std::string *str) override;

private:
  std::stringstream stream_out;

  GLfloat clearColor[4];
  GLfloat pointSize;
  GLfloat lineWidth;

  Color fillColor;
  Color strokeColor;
  Color textColor;

  bool inGlEntity;
  bool inGlGraph;
  bool inNode;
  bool inEdge;

  int width;
  int height;
};
} // namespace tlp

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLFEEDBACKBUILDER_H
///@endcond
