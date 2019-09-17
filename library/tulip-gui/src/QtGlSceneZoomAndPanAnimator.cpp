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

#include <QApplication>
#include <QTimeLine>

#include <tulip/QtGlSceneZoomAndPanAnimator.h>
#include <tulip/GlMainWidget.h>

namespace tlp {

QtGlSceneZoomAndPanAnimator::QtGlSceneZoomAndPanAnimator(
    GlMainWidget *glWidget, const BoundingBox &boundingBox, const double duration,
    const std::string &layerName, const bool optimalPath, const double velocity, const double p)
    : GlSceneZoomAndPan(glWidget->getScene(), boundingBox, layerName, 0, optimalPath, p),
      glWidget(glWidget), animationDurationMsec(duration) {
  if (doZoomAndPan) {
    animationDurationMsec *= S / velocity;
  }

  nbAnimationSteps = animationDurationMsec / 40 + 1;
}

void QtGlSceneZoomAndPanAnimator::animateZoomAndPan() {
  QTimeLine timeLine(animationDurationMsec);
  timeLine.setFrameRange(0, nbAnimationSteps);
  connect(&timeLine, SIGNAL(frameChanged(int)), this, SLOT(zoomAndPanAnimStepSlot(int)));

  if (doZoomAndPan || (additionalAnimation != nullptr)) {
    timeLine.start();

    while (timeLine.state() != QTimeLine::NotRunning) {
      QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
  }
}

void QtGlSceneZoomAndPanAnimator::zoomAndPanAnimStepSlot(int animationStep) {
  zoomAndPanAnimationStep(animationStep);
  glWidget->draw();
}
} // namespace tlp
