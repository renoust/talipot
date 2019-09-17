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

#include "SplashScreen.h"

#include <QPainter>
#include <QApplication>
#include <QPropertyAnimation>
#include <QDir>

#include <talipot/TlpTools.h>
#include <talipot/Plugin.h>
#include <talipot/TlpQtTools.h>

using namespace tlp;

SplashScreen::SplashScreen() : PluginLoader(), QSplashScreen(), _fileCounter(0) {
  setPixmap(QPixmap(QDir(QApplication::applicationDirPath())
                        .absoluteFilePath("../share/talipot/bitmaps/logo.bmp")));
  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
  QPropertyAnimation *fadeInAnimation = new QPropertyAnimation(this, "windowOpacity");
  fadeInAnimation->setStartValue(0);
  fadeInAnimation->setEndValue(1);
  fadeInAnimation->setDuration(200);
  show();
  fadeInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void SplashScreen::start(const std::string &) {
  _title = "Loading...";
  _message = QString();
  repaint();
}

void SplashScreen::loading(const std::string &filename) {
  QApplication::processEvents();
  _fileCounter++;
  _message = filename.c_str();
  repaint();
}

void SplashScreen::loaded(const Plugin *info, const std::list<Dependency> &) {
  _message = tlp::tlpStringToQString(info->name()) + " loaded.";
}

void SplashScreen::aborted(const std::string &filename, const std::string &erreurmsg) {
  _message = QString("Error loading ") + filename.c_str() + ": " + erreurmsg.c_str();
  _errors[filename.c_str()] = erreurmsg.c_str();
}

void SplashScreen::finished(bool state, const std::string &) {
  _title = "Plugins loaded.";

  if (!state)
    _message = "Errors have been reported, see details on the startup screen.";

  repaint();
}

void SplashScreen::drawContents(QPainter *painter) {
  QSize size(pixmap().size());
  painter->drawPixmap(0, 0, pixmap());
  QRectF messageRect(0, size.height() * 2 / 3, size.width(), size.height() / 3);

  painter->setPen(Qt::transparent);
  QLinearGradient grad(messageRect.x(), messageRect.y(), messageRect.x(),
                       messageRect.y() + messageRect.height());
  grad.setColorAt(0, Qt::transparent);
  grad.setColorAt(0.4, QColor(255, 255, 255, 170));
  painter->setBrush(grad);
  painter->drawRect(messageRect);

  painter->setBrush(Qt::black);
  painter->setPen(Qt::black);
  QFont font = painter->font();
  font.setBold(true);
  painter->setFont(font);
  painter->drawText(messageRect.x(), messageRect.y() + messageRect.height() / 3,
                    messageRect.width(), messageRect.height() / 3, Qt::AlignHCenter, _title);
  font.setBold(false);
  painter->setFont(font);
  painter->drawText(messageRect.x(), messageRect.y() + messageRect.height() * 2 / 3,
                    messageRect.width(), messageRect.height() / 3, Qt::AlignHCenter, _message);

  QRectF progressRect(messageRect.x() + 10, messageRect.y() + messageRect.height() - 10,
                      messageRect.width() - 20, 5);
  painter->setBrush(QColor(0, 0, 0, 50));
  painter->setPen(QColor(0, 0, 0, 50));
  painter->drawRect(progressRect);
  qreal w = progressRect.width();

  if (_numberOfFiles > 0)
    w = 1. * _fileCounter * progressRect.width() / _numberOfFiles;

  painter->setBrush(QColor(0, 0, 0, 200));
  painter->drawRect(progressRect.x(), progressRect.y(), w, progressRect.height());
}
