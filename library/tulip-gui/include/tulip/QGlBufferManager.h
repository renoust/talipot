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

#ifndef Tulip_QGLBUFFERMANAGER_H
#define Tulip_QGLBUFFERMANAGER_H

#include <map>
#include <unordered_map>

#include <tulip/tulipconf.h>
#include <QOpenGLFramebufferObject>

namespace tlp {

/** \brief Class to manage QGlFramebufferObject
 */
class TLP_QT_SCOPE QGlBufferManager {

public:
  static void clearBuffers();

  /**
   * Return if QGlFramebufferObject can be used
   */
  static bool canUseFramebufferObject() {
    return QOpenGLFramebufferObject::hasOpenGLFramebufferObjects();
  }

  /**
   * Return a QOpenGLFramebufferObject with given size
   */
  static QOpenGLFramebufferObject *getFramebufferObject(int width, int height);

private:
  static std::map<std::pair<int, int>, QOpenGLFramebufferObject *> widthHeightToFramebuffer;
  static std::unordered_map<QOpenGLFramebufferObject *, std::pair<int, int>>
      framebufferToWidthHeight;
};
} // namespace tlp

#endif // Tulip_QGLBUFFERMANAGER_H
///@endcond
