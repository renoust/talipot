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

#include "PerspectiveItemWidget.h"

#include "ui_PerspectiveItem.h"

#include <QMouseEvent>
#include <QGuiApplication>
#include <QScreen>

#include <talipot/PluginLister.h>
#include <talipot/TlpQtTools.h>
#include "MainWindow.h"

using namespace tlp;

PerspectiveItemWidget::PerspectiveItemWidget(const QString &perspectiveName, QWidget *parent)
    : QWidget(parent), _perspectiveName(perspectiveName), _ui(new Ui::PerspectiveItemData) {
  _ui->setupUi(this);
  _ui->name->setText(_perspectiveName);
  const tlp::Plugin &info = PluginLister::pluginInformation(QStringToTlpString(_perspectiveName));
  _ui->description->setText(info.info().c_str());

  QPixmap px(info.icon().c_str());
  // take care of the devicePixelRatio
  // before setting the label pixmap
  px.setDevicePixelRatio(QGuiApplication::primaryScreen()->devicePixelRatio());
  _ui->icon->setPixmap(px);
}

PerspectiveItemWidget::~PerspectiveItemWidget() {
  delete _ui;
}

void PerspectiveItemWidget::run() {
  MainWindow::instance()->createPerspective(_perspectiveName);
  emit selected();
}

void PerspectiveItemWidget::mouseDoubleClickEvent(QMouseEvent *) {
  run();
}

void PerspectiveItemWidget::focusInEvent(QFocusEvent *) {
  _ui->frame->setProperty("highlighted", true);
  _ui->frame->setStyleSheet(_ui->frame->styleSheet());
}

void PerspectiveItemWidget::focusOutEvent(QFocusEvent *) {
  _ui->frame->setProperty("highlighted", false);
  _ui->frame->setStyleSheet(_ui->frame->styleSheet());
}
