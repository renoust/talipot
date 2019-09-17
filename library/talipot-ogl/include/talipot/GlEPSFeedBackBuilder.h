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

#ifndef TALIPOT_GL_EPS_FEED_BACK_BUILDER_H
#define TALIPOT_GL_EPS_FEED_BACK_BUILDER_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <sstream>

#include <talipot/GlTLPFeedBackBuilder.h>
#include <talipot/config.h>
#include <talipot/Color.h>

namespace tlp {

/**
 * Class to build EPS file with OpenGL feedback buffer
 */
class TLP_GL_SCOPE GlEPSFeedBackBuilder : public GlTLPFeedBackBuilder {

public:
  GlEPSFeedBackBuilder() : stream_out(std::stringstream::in | std::stringstream::out) {}

  /**
   * Begin new EPS document with viewport, clearColor, pointSize and lineWidth information
   */
  void begin(const Vector<int, 4> &viewport, GLfloat *clearColor, GLfloat pointSize,
             GLfloat lineWidth) override;
  /**
   * Record a new color
   */
  void colorInfo(GLfloat *data) override;
  /**
   * Record a new GlEntity
   */
  void beginGlEntity(GLfloat data) override;
  /**
   * End of a GlEntity
   */
  void endGlEntity() override;
  /**
   * Record a new GlGraph
   */
  void beginGlGraph(GLfloat data) override;
  /**
   * End of a GlGraph
   */
  void endGlGraph() override;
  /**
   * Record a new Node
   */
  void beginNode(GLfloat data) override;
  /**
   * End of a Node
   */
  void endNode() override;
  /**
   * Record a new Edge
   */
  void beginEdge(GLfloat data) override;
  /**
   * End of a Edge
   */
  void endEdge() override;
  /**
   * Record a new pointToken
   */
  void pointToken(GLfloat *data) override;
  /**
   * Record a new lineToken
   */
  void lineToken(GLfloat *data) override;
  /**
   * Record a new lineResetToken
   */
  void lineResetToken(GLfloat *data) override;
  /**
   * Record a new polygonToken
   */
  void polygonToken(GLfloat *data) override;
  /**
   * End of the EPS document
   */
  void end() override;

  /**
   * Put in str the built EPS document
   */
  void getResult(std::string *str) override;

private:
  std::stringstream stream_out;

  GLfloat pointSize;

  Color fillColor;
  Color strokeColor;
  Color textColor;
};
} // namespace tlp

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // TALIPOT_GL_EPS_FEED_BACK_BUILDER_H
///@endcond
