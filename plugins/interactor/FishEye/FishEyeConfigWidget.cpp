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

#include "FishEyeConfigWidget.h"

#include "ui_FishEyeConfigWidget.h"

using namespace tlp;

FishEyeConfigWidget::FishEyeConfigWidget(QWidget *parent)
    : QWidget(parent), _ui(new Ui::FishEyeConfigWidget) {
  _ui->setupUi(this);
}

FishEyeConfigWidget::~FishEyeConfigWidget() {
  delete _ui;
}

int FishEyeConfigWidget::getFishEyeType() const {
  if (_ui->furnasFisheyeRb->isChecked()) {
    return 1;
  } else if (_ui->auberFishEyeRb->isChecked()) {
    return 2;
  } else {
    return 3;
  }
}

float FishEyeConfigWidget::getFishEyeRadius() const {
  return float(_ui->radiusValSP->value());
}

void FishEyeConfigWidget::setFishEyeRadius(const float radius) {
  _ui->radiusValSP->setValue(radius);
}

float FishEyeConfigWidget::getFishEyeHeight() const {
  return float(_ui->heightValSP->value());
}

void FishEyeConfigWidget::setFishEyeHeight(const float height) {
  _ui->heightValSP->setValue(height);
}

float FishEyeConfigWidget::getFishEyeRadiusIncrementStep() const {
  return float(_ui->radiusIncrementStepSP->value());
}

float FishEyeConfigWidget::getFishEyeHeightIncrementStep() const {
  return float(_ui->heightIncrementStepSP->value());
}
