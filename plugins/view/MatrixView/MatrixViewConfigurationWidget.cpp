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

#include "MatrixViewConfigurationWidget.h"
#include "ui_MatrixViewConfigurationWidget.h"

#include <tulip/Graph.h>
#include <tulip/Perspective.h>
#include <tulip/ColorButton.h>
#include <tulip/TlpQtTools.h>

#include <QMainWindow>

using namespace std;
namespace tlp {
MatrixViewConfigurationWidget::MatrixViewConfigurationWidget(QWidget *parent)
    : QWidget(parent), _ui(new Ui::MatrixViewConfigurationWidget()), _modifyingMetricList(false) {
  _ui->setupUi(this);
  connect(_ui->orderingMetricCombo, SIGNAL(currentIndexChanged(int)), this,
          SLOT(orderingMetricComboIndexChanged(int)));
  connect(_ui->backgroundColorBtn, SIGNAL(colorChanged(QColor)), this,
          SIGNAL(changeBackgroundColor(QColor)));
  connect(_ui->gridDisplayCombo, SIGNAL(currentIndexChanged(int)), this,
          SIGNAL(setGridDisplayMode()));
  connect(_ui->showedgesbox, SIGNAL(clicked(bool)), this, SIGNAL(showEdges(bool)));
  connect(_ui->enableColorInterpolationCBox, SIGNAL(clicked(bool)), this,
          SIGNAL(enableEdgeColorInterpolation(bool)));
  connect(_ui->orientedCBox, SIGNAL(clicked(bool)), this, SIGNAL(updateOriented(bool)));
  connect(_ui->ascendingOrderCBox, SIGNAL(toggled(bool)), this, SLOT(orderingDirectionChanged()));

  if (Perspective::instance())
    _ui->backgroundColorBtn->setDialogParent(Perspective::instance()->mainWindow());
}

MatrixViewConfigurationWidget::~MatrixViewConfigurationWidget() {
  delete _ui;
}

void MatrixViewConfigurationWidget::setBackgroundColor(const QColor &c) {
  _ui->backgroundColorBtn->setColor(c);
}

Color MatrixViewConfigurationWidget::getBackgroundColor() const {
  return _ui->backgroundColorBtn->tulipColor();
}

void MatrixViewConfigurationWidget::setDisplayEdges(const bool state) {
  _ui->showedgesbox->setChecked(state);
  emit showEdges(state);
}

bool MatrixViewConfigurationWidget::displayGraphEdges() const {
  return _ui->showedgesbox->isChecked();
}

void MatrixViewConfigurationWidget::setEdgeColorInterpolation(const bool state) {
  _ui->enableColorInterpolationCBox->setChecked(state);
  emit enableEdgeColorInterpolation(state);
}

bool MatrixViewConfigurationWidget::isEdgeColorInterpolation() const {
  return _ui->enableColorInterpolationCBox->isChecked();
}

void MatrixViewConfigurationWidget::setAscendingOrder(const bool state) {
  _ui->ascendingOrderCBox->setChecked(state);
}

bool MatrixViewConfigurationWidget::ascendingOrder() const {
  return _ui->ascendingOrderCBox->isChecked();
}

void MatrixViewConfigurationWidget::setOriented(const bool state) {
  _ui->orientedCBox->setChecked(state);
}

void MatrixViewConfigurationWidget::setGraph(tlp::Graph *g) {
  if (g == nullptr)
    return;

  QString firstString = _ui->orderingMetricCombo->itemText(0);
  QString currentString = _ui->orderingMetricCombo->currentText();
  _modifyingMetricList = true;
  _ui->orderingMetricCombo->clear();
  _ui->orderingMetricCombo->addItem(firstString);
  int currentIndex = 0;
  int i = 0;
  for (const string &s : g->getProperties()) {
    string type = g->getProperty(s)->getTypename();

    if (type != DoubleProperty::propertyTypename && type != IntegerProperty::propertyTypename &&
        type != StringProperty::propertyTypename)
      continue;

    _ui->orderingMetricCombo->addItem(tlpStringToQString(s));

    if (QStringToTlpString(currentString).compare(s) == 0)
      currentIndex = i;

    ++i;
  }
  _modifyingMetricList = false;
  _ui->orderingMetricCombo->setCurrentIndex(currentIndex);
}

void MatrixViewConfigurationWidget::orderingMetricComboIndexChanged(int i) {
  if (_modifyingMetricList)
    return;

  string name;

  if (i > 0)
    name = QStringToTlpString(_ui->orderingMetricCombo->itemText(i));

  emit metricSelected(name);
}

void MatrixViewConfigurationWidget::orderingDirectionChanged() {
  orderingMetricComboIndexChanged(_ui->orderingMetricCombo->currentIndex());
}

GridDisplayMode MatrixViewConfigurationWidget::gridDisplayMode() const {
  return static_cast<GridDisplayMode>(_ui->gridDisplayCombo->currentIndex());
}

int MatrixViewConfigurationWidget::orderingProperty() const {
  return _ui->orderingMetricCombo->currentIndex();
}

void MatrixViewConfigurationWidget::setOrderingProperty(int index) {
  _ui->orderingMetricCombo->setCurrentIndex(index);
}

void MatrixViewConfigurationWidget::setgridmode(int index) {
  _ui->gridDisplayCombo->setCurrentIndex(index);
}
} // namespace tlp
