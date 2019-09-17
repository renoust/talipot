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

#ifndef Tulip_GLTLPFEEDBACKBUILDER_H
#define Tulip_GLTLPFEEDBACKBUILDER_H

#include <tulip/OpenGlIncludes.h>

#include <vector>

#include <tulip/GlFeedBackBuilder.h>

namespace tlp {

enum TLP_FB_TROUGHTOKENTYPE {
  TLP_FB_COLOR_INFO,
  TLP_FB_BEGIN_ENTITY,
  TLP_FB_END_ENTITY,
  TLP_FB_BEGIN_GRAPH,
  TLP_FB_END_GRAPH,
  TLP_FB_BEGIN_NODE,
  TLP_FB_END_NODE,
  TLP_FB_BEGIN_EDGE,
  TLP_FB_END_EDGE
};

class TLP_GL_SCOPE GlTLPFeedBackBuilder : public GlFeedBackBuilder {

public:
  GlTLPFeedBackBuilder()
      : inGlEntity(false), inGlGraph(false), inNode(false), inEdge(false), inColorInfo(false),
        needData(false) {}

  ~GlTLPFeedBackBuilder() override {}

  virtual void colorInfo(GLfloat *) {}
  virtual void beginGlEntity(GLfloat) {}
  virtual void endGlEntity() {}
  virtual void beginGlGraph(GLfloat) {}
  virtual void endGlGraph() {}
  virtual void beginNode(GLfloat) {}
  virtual void endNode() {}
  virtual void beginEdge(GLfloat) {}
  virtual void endEdge() {}
  void passThroughToken(GLfloat *) override;
  void pointToken(GLfloat *) override {}
  void lineToken(GLfloat *) override {}
  void lineResetToken(GLfloat *) override {}
  void polygonToken(GLfloat *) override {}
  void bitmapToken(GLfloat *) override {}
  void drawPixelToken(GLfloat *) override {}
  void copyPixelToken(GLfloat *) override {}
  void end() override {}

private:
  bool inGlEntity;
  bool inGlGraph;
  bool inNode;
  bool inEdge;
  bool inColorInfo;
  bool needData;

  std::vector<GLfloat> dataBuffer;
};
} // namespace tlp

#endif // Tulip_GLFEEDBACKBUILDER_H
///@endcond
