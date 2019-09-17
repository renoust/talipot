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

#ifndef Tulip_GLFEEDBACKBUILDER_H
#define Tulip_GLFEEDBACKBUILDER_H

#include <tulip/OpenGlIncludes.h>

#include <tulip/tulipconf.h>
#include <tulip/Vector.h>

namespace tlp {

//====================================================
typedef struct _FeedBack3Dcolor {
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat red;
  GLfloat green;
  GLfloat blue;
  GLfloat alpha;
} Feedback3Dcolor;

//====================================================
typedef struct _DepthIndex {
  GLfloat *ptr;
  GLfloat depth;
} DepthIndex;

//====================================================
/**
 * Abstract class used to build a object with a OpenGl feedback buffer
 */
class TLP_GL_SCOPE GlFeedBackBuilder {

public:
  virtual ~GlFeedBackBuilder() {}

  virtual void begin(const Vector<int, 4> &, GLfloat *, GLfloat, GLfloat) {}
  virtual void passThroughToken(GLfloat *) {}
  virtual void pointToken(GLfloat *) {}
  virtual void lineToken(GLfloat *) {}
  virtual void lineResetToken(GLfloat *) {}
  virtual void polygonToken(GLfloat *) {}
  virtual void bitmapToken(GLfloat *) {}
  virtual void drawPixelToken(GLfloat *) {}
  virtual void copyPixelToken(GLfloat *) {}
  virtual void end() {}

  virtual void getResult(std::string *str) = 0;
};
} // namespace tlp

#endif // Tulip_GLFEEDBACKBUILDER_H
///@endcond
