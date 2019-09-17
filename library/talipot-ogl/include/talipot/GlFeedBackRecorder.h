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

#ifndef TALIPOT_GL_FEED_BACK_RECORDER_H
#define TALIPOT_GL_FEED_BACK_RECORDER_H

#include <talipot/OpenGlIncludes.h>

#include <talipot/config.h>
#include <talipot/Vector.h>

namespace tlp {

class GlFeedBackBuilder;

/** \brief Class use to build an object with an OpenGL feedback buffer
 *
 * Class use to build an object with an OpenGL feedback buffer
 * This class call functions of the GlFeedBackBuilder passed to the constructor
 */
class TLP_GL_SCOPE GlFeedBackRecorder {

public:
  /**
   * Constructor : the recorder use GlFeedBackBuilder : builder
   */
  GlFeedBackRecorder(GlFeedBackBuilder *builder, unsigned int pointSize = 7);
  /**
   * Record a new feedback buffer
   * \param doSort : sort the feedback buffer
   * \param size : size of the feedback buffer
   * \param feedBackBuffer : the feedback buffer
   * \param viewport : the viewport of the scene
   */
  void record(bool doSort, GLint size, GLfloat *feedBackBuffer, const Vector<int, 4> &viewport);

private:
  void sortAndRecord(GLint size, GLfloat *feedBackBuffer);
  void record(GLint size, GLfloat *feedBackBuffer);
  GLfloat *recordPrimitive(GLfloat *loc);

  GlFeedBackBuilder *feedBackBuilder;
  unsigned int pointSize;
};
} // namespace tlp

#endif // TALIPOT_GL_FEED_BACK_RECORDER_H
///@endcond
