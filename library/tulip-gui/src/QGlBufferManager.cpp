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
#include "tulip/QGlBufferManager.h"

#include <tulip/GlMainWidget.h>
#include <tulip/OpenGlConfigManager.h>

#include <iostream>

//====================================================
using namespace std;

namespace tlp {

std::map<std::pair<int, int>, QOpenGLFramebufferObject *>
    QGlBufferManager::widthHeightToFramebuffer;
std::unordered_map<QOpenGLFramebufferObject *, std::pair<int, int>>
    QGlBufferManager::framebufferToWidthHeight;

void QGlBufferManager::clearBuffers() {
  for (auto it = widthHeightToFramebuffer.begin(); it != widthHeightToFramebuffer.end(); ++it)
    delete (*it).second;

  widthHeightToFramebuffer.clear();
  framebufferToWidthHeight.clear();
}

QOpenGLFramebufferObject *QGlBufferManager::getFramebufferObject(int width, int height) {
  auto it = widthHeightToFramebuffer.find(pair<int, int>(width, height));

  if (it != widthHeightToFramebuffer.end())
    return (*it).second;

  QOpenGLFramebufferObject *glFramebufferObject = new QOpenGLFramebufferObject(
      width, height /*,QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, GL_RGBA32F_ARB*/);

  if (!glFramebufferObject->isValid()) {
    while (!glFramebufferObject->isValid() && !framebufferToWidthHeight.empty()) {
      int widthToRemove = 0;
      int heightToRemove = 0;
      QOpenGLFramebufferObject *bufferToRemove = nullptr;

      for (it = widthHeightToFramebuffer.begin(); it != widthHeightToFramebuffer.end(); ++it) {
        if ((((*it).first.first) * ((*it).first.second)) > widthToRemove * heightToRemove) {
          widthToRemove = (*it).first.first;
          heightToRemove = (*it).first.second;
          bufferToRemove = (*it).second;
        }
      }

      delete bufferToRemove;
      widthHeightToFramebuffer.erase(pair<int, int>(widthToRemove, heightToRemove));
      framebufferToWidthHeight.erase(bufferToRemove);

      delete glFramebufferObject;
      glFramebufferObject = new QOpenGLFramebufferObject(
          width, height /*,QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, GL_RGBA32F_ARB*/);
    }

    while (!glFramebufferObject->isValid() && width > 0 && height > 0) {
      width = width / 2;
      height = height / 2;

      delete glFramebufferObject;
      glFramebufferObject = new QOpenGLFramebufferObject(
          width, height /*,QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, GL_RGBA32F_ARB*/);
    }
  }

  widthHeightToFramebuffer[pair<int, int>(width, height)] = glFramebufferObject;
  framebufferToWidthHeight[glFramebufferObject] = pair<int, int>(width, height);

  return glFramebufferObject;
}
} // namespace tlp
