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

#include "ZoomAndPanHighlighter.h"

#include <tulip/Graph.h>
#include <tulip/GlMainView.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/LayoutProperty.h>
#include <tulip/Coord.h>
#include <tulip/BoundingBox.h>
#include <tulip/DrawingTools.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainWidget.h>
#include <tulip/QtGlSceneZoomAndPanAnimator.h>

using namespace std;
using namespace tlp;

void ZoomAndPanHighlighter::highlight(const PathFinder *, GlMainWidget *glMainWidget,
                                      BooleanProperty *selection, node, node) {
  GlGraphInputData *inputData(getInputData(glMainWidget));
  Graph *graph = glMainWidget->getScene()->getGlGraphComposite()->getGraph();
  LayoutProperty *layout = inputData->getElementLayout();
  BoundingBox bbox(computeBoundingBox(graph, layout, inputData->getElementSize(),
                                      inputData->getElementRotation(), selection));

  QtGlSceneZoomAndPanAnimator animator(glMainWidget, bbox);
  animator.animateZoomAndPan();
}

void ZoomAndPanHighlighter::draw(GlMainWidget *) {}

bool ZoomAndPanHighlighter::isConfigurable() const {
  return false;
}

QWidget *ZoomAndPanHighlighter::getConfigurationWidget() {
  return nullptr;
}
