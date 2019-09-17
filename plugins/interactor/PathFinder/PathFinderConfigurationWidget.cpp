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

#include "PathFinderConfigurationWidget.h"
#include "ui_PathFinderConfiguration.h"

using namespace tlp;
using namespace std;

PathFinderConfigurationWidget::PathFinderConfigurationWidget(QWidget *parent)
    : QWidget(parent), _ui(new Ui::PathFinderConfigurationData) {
  _ui->setupUi(this);

  connect(_ui->weightCombo, SIGNAL(activated(const QString &)), this,
          SIGNAL(setWeightMetric(const QString &)));
  connect(_ui->edgeOrientationCombo, SIGNAL(activated(const QString &)), this,
          SIGNAL(setEdgeOrientation(const QString &)));
  connect(_ui->pathsTypeCombo, SIGNAL(activated(const QString &)), this,
          SIGNAL(setPathsType(const QString &)));
  connect(_ui->toleranceCheck, SIGNAL(clicked(bool)), this, SIGNAL(activateTolerance(bool)));
  connect(_ui->toleranceSpin, SIGNAL(valueChanged(int)), this, SIGNAL(setTolerance(int)));
}

PathFinderConfigurationWidget::~PathFinderConfigurationWidget() {
  delete _ui;
}

void PathFinderConfigurationWidget::addweightComboItem(const QString &s) {
  _ui->weightCombo->addItem(s);
}

void PathFinderConfigurationWidget::addedgeOrientationComboItem(const QString &s) {
  _ui->edgeOrientationCombo->addItem(s);
}

void PathFinderConfigurationWidget::addpathsTypeComboItem(const QString &s) {
  _ui->pathsTypeCombo->addItem(s);
}

void PathFinderConfigurationWidget::setCurrentweightComboIndex(const int i) {
  _ui->weightCombo->setCurrentIndex(i);
}

int PathFinderConfigurationWidget::weightComboFindText(const QString &text) const {
  return _ui->weightCombo->findText(text);
}

void PathFinderConfigurationWidget::setCurrentedgeOrientationComboIndex(const int i) {
  _ui->edgeOrientationCombo->setCurrentIndex(i);
}

int PathFinderConfigurationWidget::edgeOrientationComboFindText(const QString &text) const {
  return _ui->edgeOrientationCombo->findText(text);
}

void PathFinderConfigurationWidget::toleranceChecked(const bool checked) {
  _ui->toleranceCheck->setChecked(checked);
}

void PathFinderConfigurationWidget::setToleranceSpinValue(const int val) {
  _ui->toleranceSpin->setValue(val);
}

void PathFinderConfigurationWidget::highlightersLabelDisabled(const bool disable) {
  _ui->highlightersLabel->setDisabled(disable);
}

void PathFinderConfigurationWidget::addbottomWidget(QWidget *w) {
  _ui->bottomArea->addWidget(w, 0, Qt::AlignLeft);
}

void PathFinderConfigurationWidget::toleranceDisabled(const bool disabled) {
  _ui->toleranceCheck->setDisabled(disabled);
  _ui->toleranceSpin->setDisabled(disabled);
  _ui->toleranceLabel->setDisabled(disabled);
  _ui->tolerancePercentLabel->setDisabled(disabled);
}
