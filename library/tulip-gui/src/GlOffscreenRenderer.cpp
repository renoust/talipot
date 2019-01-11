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

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include <GL/glew.h>

// remove warnings about qt5/glew incompatibility
// as we do not rely on QOpenGLFunctions for rendering
#undef __GLEW_H__
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#define __GLEW_H__
QT_BEGIN_NAMESPACE
QOpenGLContext *qt_gl_global_share_context();
QT_END_NAMESPACE

#include <QOffscreenSurface>

#include <tulip/Camera.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlTools.h>

#include <QPainter>

#include <sstream>

using namespace std;

namespace tlp {

QOffscreenSurface *GlOffscreenRenderer::offscreenSurface = nullptr;
GlOffscreenRenderer *GlOffscreenRenderer::instance = nullptr;

GlOffscreenRenderer *GlOffscreenRenderer::getInstance() {
  if (!instance)
    instance = new GlOffscreenRenderer();
  return instance;
}

GlOffscreenRenderer::GlOffscreenRenderer()
    : vPWidth(512), vPHeight(512), glFrameBuf(nullptr), glFrameBuf2(nullptr),
      mainLayer(new GlLayer("Main")), entitiesCpt(0), zoomFactor(DBL_MAX),
      cameraCenter(FLT_MAX, FLT_MAX, FLT_MAX) {
  GlLayer *backgroundLayer = new GlLayer("Background");
  backgroundLayer->setVisible(true);
  GlLayer *foregroundLayer = new GlLayer("Foreground");
  foregroundLayer->setVisible(true);
  backgroundLayer->set2DMode();
  foregroundLayer->set2DMode();
  scene.addExistingLayer(backgroundLayer);
  scene.addExistingLayer(mainLayer);
  scene.addExistingLayer(foregroundLayer);
  antialiasedFbo = false;
}

GlOffscreenRenderer::~GlOffscreenRenderer() {}

void GlOffscreenRenderer::setViewPortSize(const unsigned int viewPortWidth,
                                          const unsigned int viewPortHeight) {
  vPWidth = viewPortWidth;
  vPHeight = viewPortHeight;
}

void GlOffscreenRenderer::setSceneBackgroundColor(const Color &color) {
  scene.setBackgroundColor(color);
}

unsigned int GlOffscreenRenderer::getViewportWidth() {
  return glFrameBuf->width();
}

unsigned int GlOffscreenRenderer::getViewportHeight() {
  return glFrameBuf->height();
}

void GlOffscreenRenderer::addGlEntityToScene(GlSimpleEntity *entity) {
  ostringstream oss;
  oss << "entity " << ++entitiesCpt;
  mainLayer->addGlEntity(entity, oss.str());
}

void GlOffscreenRenderer::addGraphToScene(Graph *graph) {
  addGraphCompositeToScene(new GlGraphComposite(graph));
}

void GlOffscreenRenderer::addGraphCompositeToScene(GlGraphComposite *graphComposite) {
  // Delete old composite if it exist
  GlSimpleEntity *oldComposite = mainLayer->findGlEntity("graph");

  if (oldComposite != nullptr) {
    mainLayer->deleteGlEntity(oldComposite);
  }

  GlVertexArrayManager *vertexArrayManager =
      graphComposite->getInputData()->getGlVertexArrayManager();
  vertexArrayManager->setHaveToComputeAll(true);
  mainLayer->addGlEntity(graphComposite, "graph");
}

void GlOffscreenRenderer::clearScene(bool deleteGlEntities) {
  mainLayer->getComposite()->reset(deleteGlEntities);
  const vector<pair<string, GlLayer *>> &layersList = scene.getLayersList();

  for (unsigned int i = 0; i < layersList.size(); ++i) {
    if (layersList[i].second != mainLayer) {
      layersList[i].second->getComposite()->reset(true);
    }
  }

  entitiesCpt = 0;
  zoomFactor = DBL_MAX;
}

void GlOffscreenRenderer::initFrameBuffers(const bool antialiased) {

  antialiasedFbo = antialiased && QOpenGLFramebufferObject::hasOpenGLFramebufferBlit();

  if (glFrameBuf != nullptr &&
      (vPWidth != uint(glFrameBuf->width()) || vPHeight != uint(glFrameBuf->height()))) {
    delete glFrameBuf;
    glFrameBuf = nullptr;
    delete glFrameBuf2;
    glFrameBuf2 = nullptr;
  }

  if (glFrameBuf == nullptr) {
    QOpenGLFramebufferObjectFormat fboFmt;
    fboFmt.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    if (antialiasedFbo)
      fboFmt.setSamples(OpenGlConfigManager::getInst().maxNumberOfSamples());

    glFrameBuf = new QOpenGLFramebufferObject(vPWidth, vPHeight, fboFmt);
  }

  if (antialiasedFbo && glFrameBuf2 == nullptr) {
    glFrameBuf2 = new QOpenGLFramebufferObject(vPWidth, vPHeight);
  }
}

void GlOffscreenRenderer::renderScene(const bool centerScene, const bool antialiased) {

  makeOpenGLContextCurrent();

  initFrameBuffers(antialiased);

  scene.setViewport(0, 0, vPWidth, vPHeight);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  GL_TEST_ERROR();
  glFrameBuf->bind();
  // the line above can produce an invalid op error, forget it
  glGetError();
  GL_TEST_ERROR();

  if (centerScene) {
    scene.centerScene();
  }

  Camera &camera = mainLayer->getCamera();

  if (cameraCenter != Coord(FLT_MAX, FLT_MAX, FLT_MAX)) {
    camera.setCenter(cameraCenter);
    camera.setEyes(Coord(0, 0, camera.getSceneRadius()));
    camera.setEyes(camera.getEyes() + camera.getCenter());
    camera.setUp(Coord(0, 1., 0));
  }

  if (zoomFactor != DBL_MAX) {
    camera.setZoomFactor(zoomFactor);
  }

  scene.draw();
  glFrameBuf->release();

  if (antialiasedFbo)
    QOpenGLFramebufferObject::blitFramebuffer(
        glFrameBuf2, QRect(0, 0, glFrameBuf2->width(), glFrameBuf2->height()), glFrameBuf,
        QRect(0, 0, glFrameBuf->width(), glFrameBuf->height()));

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  GL_TEST_ERROR();
  glPopAttrib();
  // the line above can produce an invalid op error, forget it
  glGetError();
  GL_TEST_ERROR();
}

void GlOffscreenRenderer::renderExternalScene(GlScene *scene, const bool antialiased) {

  makeOpenGLContextCurrent();

  initFrameBuffers(antialiased);

  Vector<int, 4> backupViewport = scene->getViewport();

  scene->setViewport(0, 0, vPWidth, vPHeight);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glFrameBuf->bind();
  scene->draw();
  glFrameBuf->release();

  if (antialiasedFbo)
    QOpenGLFramebufferObject::blitFramebuffer(
        glFrameBuf2, QRect(0, 0, glFrameBuf2->width(), glFrameBuf2->height()), glFrameBuf,
        QRect(0, 0, glFrameBuf->width(), glFrameBuf->height()));

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  GL_TEST_ERROR();
  glPopAttrib();
  // the line above can produce an invalid op error, forget it
  glGetError();
  GL_TEST_ERROR();

  scene->setViewport(backupViewport);
}

bool GlOffscreenRenderer::frameBufferOk() const {
  return glFrameBuf->isValid();
}

static inline QImage convertImage(const QImage &image) {
  return QImage(image.constBits(), image.width(), image.height(), QImage::Format_ARGB32)
       .convertToFormat(QImage::Format_RGB32);
}

QImage GlOffscreenRenderer::getImage() {
  makeOpenGLContextCurrent();
  QImage image;
  if (!antialiasedFbo)
    image = convertImage(glFrameBuf->toImage());
  else
    image = convertImage(glFrameBuf2->toImage());
  return image;
}

GLuint GlOffscreenRenderer::getGLTexture(const bool generateMipMaps) {

  makeOpenGLContextCurrent();

  bool canUseMipmaps =
      OpenGlConfigManager::getInst().isExtensionSupported("GL_ARB_framebuffer_object") ||
      OpenGlConfigManager::getInst().isExtensionSupported("GL_EXT_framebuffer_object");

  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureId);

  if (generateMipMaps && canUseMipmaps) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  GL_TEST_ERROR();
  QImage image = getImage().mirrored();
  // the line above can produce an invalid op error, forget it
  glGetError();
  GL_TEST_ERROR();
  unsigned char *buff = image.bits();

  makeOpenGLContextCurrent();

  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getViewportWidth(), getViewportHeight(), 0, GL_BGRA,
               GL_UNSIGNED_BYTE, buff);

  if (generateMipMaps && canUseMipmaps) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  GL_TEST_ERROR();
  glDisable(GL_TEXTURE_2D);
  GL_TEST_ERROR();

  return textureId;
}

QOpenGLContext *GlOffscreenRenderer::getOpenGLContext() {
  if (!offscreenSurface) {
    offscreenSurface = new QOffscreenSurface();
    offscreenSurface->create();
  }
  return qt_gl_global_share_context();
}

void GlOffscreenRenderer::makeOpenGLContextCurrent() {
  getOpenGLContext()->makeCurrent(offscreenSurface);
}

void GlOffscreenRenderer::doneOpenGLContextCurrent() {
  getOpenGLContext()->doneCurrent();
}

void GlOffscreenRenderer::renderGlMainWidget(GlMainWidget *glWidget, bool redrawNeeded) {
  makeOpenGLContextCurrent();
  initFrameBuffers(true);
  glFrameBuf2->bind();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  if (redrawNeeded) {
    glWidget->render(GlMainWidget::RenderingOptions(GlMainWidget::RenderScene), false);
  } else {
    glWidget->render(GlMainWidget::RenderingOptions(), false);
  }
  glPopAttrib();
  glFrameBuf2->release();
}

void GlOffscreenRenderer::cleanOpenGLResources() {
  delete glFrameBuf;
  delete glFrameBuf2;
  delete offscreenSurface;
}

}
