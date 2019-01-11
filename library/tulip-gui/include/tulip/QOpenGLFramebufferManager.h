/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN

#ifndef QOPENGL_FRAME_BUFFER_MANAGER_H
#define QOPENGL_FRAME_BUFFER_MANAGER_H

#include <map>

#include <tulip/tulipconf.h>

class QOpenGLFramebufferObject;

namespace tlp {

/** \brief Class to manage QOpenGLFramebufferObject
 * Singleton class to manager QOpenGLFramebufferObject
 */
class TLP_QT_SCOPE QOpenGLFramebufferManager {

public:
  /**
   * Return the QOpenGLFramebufferManager manager singleton, if it doesn't exist this function create it
   */
  static QOpenGLFramebufferManager &getInst() {
    if (!inst)
      inst = new QOpenGLFramebufferManager();

    return *inst;
  }

  static void clearBuffers();

  /**
   * Return if QOpenGLFramebufferObject can be used
   */
  bool canUseFramebufferObject() {
    return framebufferObjectWork;
  }

  /**
   * Return a QOpenGLFramebufferObject with given size
   */
  QOpenGLFramebufferObject *getFramebufferObject(int width, int height);

private:
  /**
   * empty private constructor for singleton
   */
  QOpenGLFramebufferManager();

  static QOpenGLFramebufferManager *inst;

  std::map<std::pair<int, int>, QOpenGLFramebufferObject *> widthHeightToFramebuffer;
  std::map<QOpenGLFramebufferObject *, std::pair<int, int>> framebufferToWidthHeight;

  bool framebufferObjectWork;
};
}

#endif // QOPENGL_FRAME_BUFFER_MANAGER_H
///@endcond
