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
///@cond DOXYGEN_HIDDEN

#ifndef TULIPFONTICONENGINE_H
#define TULIPFONTICONENGINE_H

#include <QIconEngine>
#include <QPixmap>
#include <QFont>
#include <tulip/tulipconf.h>

class TLP_QT_SCOPE TulipFontIconEngine : public QIconEngine {
  unsigned int codePoint;
  QFont &font;

  QFont &init(const std::string &iconName);

public:
  TulipFontIconEngine(const std::string &iconName);
  TulipFontIconEngine(const QString &iconName);
  TulipFontIconEngine(const TulipFontIconEngine &engine)
      : QIconEngine(), codePoint(engine.codePoint), font(engine.font) {}

  TulipFontIconEngine *clone() const override {
    return new TulipFontIconEngine(*this);
  }

  void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;

  QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;

  static inline QIcon icon(const char *iconName) {
    return QIcon(new TulipFontIconEngine(std::string(iconName)));
  }

  static inline QIcon icon(const std::string &iconName) {
    return QIcon(new TulipFontIconEngine(iconName));
  }

  static inline QIcon icon(const QString &iconName) {
    return QIcon(new TulipFontIconEngine(iconName));
  }

  static inline QPixmap pixmap(const std::string &iconName, unsigned int height,
                               QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::On) {
    return TulipFontIconEngine(iconName).pixmap(QSize(height, height), mode, state);
  }

  static inline QPixmap pixmap(const QString &iconName, unsigned int height,
                               QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::On) {
    return TulipFontIconEngine(iconName).pixmap(QSize(height, height), mode, state);
  }
};

#endif
///@endcond
