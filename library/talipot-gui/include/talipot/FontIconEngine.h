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

#ifndef TALIPOT_FONT_ICON_ENGINE_H
#define TALIPOT_FONT_ICON_ENGINE_H

#include <QIconEngine>
#include <QPixmap>
#include <QFont>
#include <talipot/config.h>

class TLP_QT_SCOPE FontIconEngine : public QIconEngine {
  unsigned int codePoint;
  QFont &font;

  QFont &init(const std::string &iconName);

public:
  FontIconEngine(const std::string &iconName);
  FontIconEngine(const QString &iconName);
  FontIconEngine(const FontIconEngine &engine)
      : QIconEngine(), codePoint(engine.codePoint), font(engine.font) {}

  FontIconEngine *clone() const override {
    return new FontIconEngine(*this);
  }

  void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;

  QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;

  static inline QIcon icon(const char *iconName) {
    return QIcon(new FontIconEngine(std::string(iconName)));
  }

  static inline QIcon icon(const std::string &iconName) {
    return QIcon(new FontIconEngine(iconName));
  }

  static inline QIcon icon(const QString &iconName) {
    return QIcon(new FontIconEngine(iconName));
  }

  static inline QPixmap pixmap(const std::string &iconName, unsigned int height,
                               QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::On) {
    return FontIconEngine(iconName).pixmap(QSize(height, height), mode, state);
  }

  static inline QPixmap pixmap(const QString &iconName, unsigned int height,
                               QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::On) {
    return FontIconEngine(iconName).pixmap(QSize(height, height), mode, state);
  }
};

#endif // TALIPOT_FONT_ICON_ENGINE_H
///@endcond
