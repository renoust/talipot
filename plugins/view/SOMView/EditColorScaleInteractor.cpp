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

#include "EditColorScaleInteractor.h"
#include "GlLabelledColorScale.h"

#include <QMouseEvent>

#include <talipot/GlMainWidget.h>
#include <talipot/GlEntity.h>
#include <talipot/GlColorScale.h>
#include <talipot/ColorScaleConfigDialog.h>
#include <talipot/Camera.h>
#include <talipot/GlLayer.h>
#include <talipot/DoubleProperty.h>

#include "SOMView.h"
#include <SOMMap.h>

using namespace tlp;
using namespace std;

EditColorScaleInteractor::EditColorScaleInteractor()
    : currentProperty(nullptr), colorScale(nullptr), widthPercent(.80f), heightPercent(.1f),
      heightPosition(.1f), glMainWidgetWidth(0), glMainWidgetHeight(0),
      selectionLayer(new GlLayer("SelectionLayer")) {}

EditColorScaleInteractor::~EditColorScaleInteractor() {
  selectionLayer->clear();
  delete selectionLayer;
  delete colorScale;
}

bool EditColorScaleInteractor::eventFilter(QObject *obj, QEvent *event) {
  tlp::GlMainWidget *glMainWidget = static_cast<tlp::GlMainWidget *>(obj);

  if (event->type() == QEvent::MouseButtonDblClick) {
    QMouseEvent *me = static_cast<QMouseEvent *>(event);

    glMainWidget->getScene()->getGraphCamera().initGl();
    selectionLayer->set2DMode();
    glMainWidget->getScene()->addExistingLayer(selectionLayer);
    selectionLayer->getCamera().initGl();
    selectionLayer->addGlEntity(colorScale, "colorScale");

    std::vector<SelectedEntity> entities;
    glMainWidget->getScene()->selectEntities(RenderingSimpleEntities, me->pos().x(), me->pos().y(),
                                             2, 2, selectionLayer, entities);
    bool foundGlColorScale = false;

    if (!entities.empty()) {
      for (const auto &entity : entities) {
        if (entity.getSimpleEntity() == colorScale->getGlColorScale()) {
          ColorScaleConfigDialog dialog(*colorScale->getGlColorScale()->getColorScale(),
                                        glMainWidget);
          foundGlColorScale = true;

          if (dialog.exec()) {
            SOMView *somView = static_cast<SOMView *>(view());
            // update shared color scale
            somView->getColorScale()->setColorMap(dialog.getColorScale().getColorMap());
            somView->updateDefaultColorProperty();
          }
        }
      }
    }

    // Empty layer without destructing objects.
    selectionLayer->deleteGlEntity(colorScale);
    glMainWidget->getScene()->removeLayer(selectionLayer, false);

    return foundGlColorScale;
  }

  return false;
}
void EditColorScaleInteractor::viewChanged(View *view) {
  SOMView *somView = static_cast<SOMView *>(view);

  if (somView != nullptr) {
    assert(colorScale == nullptr);
    GlMainWidget *glMainWidget = somView->getMapWidget();
    Size screenSize = {glMainWidget->width() * widthPercent,
                       glMainWidget->height() * heightPercent};
    Coord bottomLeftScreenCoord = {(glMainWidget->width() - screenSize.getW()) * .5f,
                                   glMainWidget->height() * .1f};
    colorScale = new GlLabelledColorScale(bottomLeftScreenCoord, screenSize,
                                          somView->getColorScale(), 0, 0, false);

    propertyChanged(somView, somView->getSelectedProperty(), somView->getSelectedPropertyValues());
  }
}

bool EditColorScaleInteractor::compute(GlMainWidget *) {
  SOMView *somView = static_cast<SOMView *>(view());
  assert(somView != nullptr);

  screenSizeChanged(somView);
  return true;
}

bool EditColorScaleInteractor::draw(GlMainWidget *glMainWidget) {
  SOMView *somView = static_cast<SOMView *>(view());
  assert(somView != nullptr);

  if (colorScale) {
    NumericProperty *newProperty = somView->getSelectedPropertyValues();

    if (currentProperty != newProperty) {
      propertyChanged(somView, somView->getSelectedProperty(), newProperty);
    }

    if (colorScale->isVisible()) {
      glMainWidget->getScene()->getGraphCamera().initGl();
      Camera camera2D = Camera(glMainWidget->getScene(), false);
      camera2D.setScene(glMainWidget->getScene());
      camera2D.initGl();

      for (const auto &it : colorScale->getGlEntities()) {
        it.second->draw(0, &camera2D);
      }
    }
  }

  return true;
}

bool EditColorScaleInteractor::screenSizeChanged(SOMView *somView) {
  GlMainWidget *glMainWidget = somView->getMapWidget();

  if (glMainWidget->width() != glMainWidgetWidth || glMainWidget->height() != glMainWidgetHeight) {
    if (colorScale) {
      Size screenSize = {glMainWidget->width() * widthPercent,
                         glMainWidget->height() * heightPercent};
      Coord bottomLeftScreenCoord = {(glMainWidget->width() - screenSize.getW()) * .5f,
                                     glMainWidget->height() * .1f};
      colorScale->setPosition(bottomLeftScreenCoord);
      colorScale->setSize(screenSize);
      glMainWidgetWidth = glMainWidget->width();
      glMainWidgetHeight = glMainWidget->height();
    }

    return true;
  }

  return false;
}

void EditColorScaleInteractor::propertyChanged(SOMView *somView, const string &propertyName,
                                               NumericProperty *propertyValues) {
  if (propertyValues) {
    colorScale->setVisible(true);
    // If the input samples are normalized we need to translate it to unormalized values before
    // displaying it.
    double minValue = propertyValues->getNodeDoubleMin(somView->getSOM());
    double maxValue = propertyValues->getNodeDoubleMax(somView->getSOM());
    InputSample &inputSample = somView->getInputSample();
    colorScale->setMinValue(inputSample.isUsingNormalizedValues()
                                ? inputSample.unnormalize(minValue, propertyName)
                                : minValue);
    colorScale->setMaxValue(inputSample.isUsingNormalizedValues()
                                ? inputSample.unnormalize(maxValue, propertyName)
                                : maxValue);
  } else {
    colorScale->setVisible(false);
  }

  currentProperty = propertyValues;
}
