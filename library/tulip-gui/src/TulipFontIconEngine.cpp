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
#include "tulip/TulipIconicFont.h"
#include "tulip/TulipFontIconEngine.h"
#include "tulip/TlpQtTools.h"

#include <QFile>
#include <QFontDatabase>
#include <QPainter>

#include <unordered_map>

using namespace tlp;

std::unordered_map<std::string, QFont> qFonts;
QFont nullFont;

QFont &TulipFontIconEngine::init(const std::string &iconName) {
  // first set code point
  codePoint = TulipIconicFont::getIconCodePoint(iconName);
  // then get font
  auto &&fontFile = TulipIconicFont::getTTFLocation(iconName);
  if (qFonts.find(fontFile) == qFonts.end()) {
    // load the font file
    auto fontId = QFontDatabase::addApplicationFont(tlpStringToQString(fontFile));
    if (fontId == -1) {
      qDebug() << "Error when loading font file " << tlpStringToQString(fontFile);
      return nullFont;
    }

    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (!fontFamilies.empty()) {
      qFonts.emplace(std::make_pair(fontFile, QFont(fontFamilies.at(0))));
    } else {
      qDebug() << "No data found when loading file" << tlpStringToQString(fontFile);
      return nullFont;
    }
  }
  return qFonts[fontFile];
}

TulipFontIconEngine::TulipFontIconEngine(const std::string &iconName) : font(init(iconName)) {}

TulipFontIconEngine::TulipFontIconEngine(const QString &iconName)
    : font(init(QStringToTlpString(iconName))) {}

void TulipFontIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode,
                                QIcon::State) {
  painter->save();

  // set the correct color
  QColor color(50, 50, 50);

  if ((mode == QIcon::Active) || (mode == QIcon::Selected))
    color.setRgb(10, 10, 10);
  else if (mode == QIcon::Disabled)
    color.setRgb(70, 70, 70, 60);
  painter->setPen(color);

  // add some 'padding' around the icon
  font.setPixelSize(qRound(rect.height() * 0.9));
  // set the font
  painter->setFont(font);

  painter->drawText(rect, QString(QChar(static_cast<int>(codePoint))),
                    QTextOption(Qt::AlignCenter | Qt::AlignVCenter));

  painter->restore();
}

QPixmap TulipFontIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) {
  QPixmap pm(size);
  pm.fill(Qt::transparent); // we need transparency
  QPainter painter(&pm);
  paint(&painter, QRect(QPoint(0, 0), size), mode, state);

  return pm;
}
