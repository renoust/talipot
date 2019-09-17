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

#include <tulip/CoordEditor.h>

#include "ui_CoordEditor.h"

#include <cfloat>

#include <QDoubleValidator>

using namespace tlp;

CoordEditor::CoordEditor(QWidget *parent, bool editSize)
    : QDialog(parent), ui(new Ui::CoordEditor) {
  ui->setupUi(this);

  if (editSize) {
    setWindowTitle("Edit size");
    ui->xLabel->setText("W");
    ui->yLabel->setText("H");
    ui->zLabel->setText("D");
  }

  ui->xSP->setRange(-FLT_MAX, FLT_MAX);
  ui->ySP->setRange(-FLT_MAX, FLT_MAX);
  ui->zSP->setRange(-FLT_MAX, FLT_MAX);
  setCoord(Coord());
  connect(ui->xSP, SIGNAL(valueChanged(double)), this, SLOT(coordUpdated()));
  connect(ui->ySP, SIGNAL(valueChanged(double)), this, SLOT(coordUpdated()));
  connect(ui->zSP, SIGNAL(valueChanged(double)), this, SLOT(coordUpdated()));
  setModal(true);
}

CoordEditor::~CoordEditor() {
  delete ui;
}

Coord CoordEditor::coord() const {
  return currentCoord;
}
void CoordEditor::setCoord(const Coord &coord) {
  currentCoord = coord;
  blockSignals(true);
  ui->xSP->setValue(coord[0]);
  ui->ySP->setValue(coord[1]);
  ui->zSP->setValue(coord[2]);
  blockSignals(false);
  coordUpdated();
}

void CoordEditor::coordUpdated() {
  currentCoord = Coord(float(ui->xSP->value()), float(ui->ySP->value()), float(ui->zSP->value()));
  emit(coordChanged(coord()));
}

void CoordEditor::done(int r) {
  if (r == QDialog::Accepted)
    currentCoord = Coord(float(ui->xSP->value()), float(ui->ySP->value()), float(ui->zSP->value()));

  QDialog::done(r);
}

// to ensure it is shown in the center of its parent
void CoordEditor::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  if (parentWidget())
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() - rect().center());
}
