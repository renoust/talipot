/**
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
#include "tulip/QOpenGLFramebufferManager.h"

#include <QOpenGLFramebufferObject>

#include <tulip/GlMainWidget.h>
#include <tulip/OpenGlConfigManager.h>

#include <iostream>

//====================================================
tlp::QOpenGLFramebufferManager *tlp::QOpenGLFramebufferManager::inst = nullptr;

using namespace std;

namespace tlp {

QOpenGLFramebufferManager::QOpenGLFramebufferManager() {
  QOpenGLFramebufferObject *glFramebufferObject = new QOpenGLFramebufferObject(2, 2);
  framebufferObjectWork = glFramebufferObject->isValid();
  delete glFramebufferObject;
}

void QOpenGLFramebufferManager::clearBuffers() {
  if (!inst)
    return;

  for (auto it = inst->widthHeightToFramebuffer.begin();
       it != inst->widthHeightToFramebuffer.end(); ++it)
    delete (*it).second;

  inst->widthHeightToFramebuffer.clear();
  inst->framebufferToWidthHeight.clear();
}

QOpenGLFramebufferObject *QOpenGLFramebufferManager::getFramebufferObject(int width, int height) {
  auto it = widthHeightToFramebuffer.find(pair<int, int>(width, height));

  if (it != widthHeightToFramebuffer.end())
    return (*it).second;

  QOpenGLFramebufferObjectFormat fboFmt;
  fboFmt.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
  fboFmt.setSamples(OpenGlConfigManager::getInst().maxNumberOfSamples());

  QOpenGLFramebufferObject *glFramebufferObject = new QOpenGLFramebufferObject(width, height, fboFmt);

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
      glFramebufferObject = new QOpenGLFramebufferObject(width, height);
    }

    while (!glFramebufferObject->isValid() && width > 0 && height > 0) {
      width = width / 2;
      height = height / 2;

      delete glFramebufferObject;
      glFramebufferObject = new QOpenGLFramebufferObject(width, height);
    }
  }

  widthHeightToFramebuffer[pair<int, int>(width, height)] = glFramebufferObject;
  framebufferToWidthHeight[glFramebufferObject] = pair<int, int>(width, height);

  return glFramebufferObject;
}
} // namespace tlp
