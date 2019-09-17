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

#ifndef OUTPUTPANELBUTTON_H
#define OUTPUTPANELBUTTON_H

#include <QPushButton>

class OutputPanelButton : public QPushButton {
  Q_OBJECT

  Q_PROPERTY(int number READ number WRITE setNumber)
  int _number;

  Q_PROPERTY(QString title READ title WRITE setTitle)
  QString _title;

  Q_PROPERTY(QColor glowColor READ glowColor WRITE setGlowColor)
  QColor _glowColor;

public:
  explicit OutputPanelButton(QWidget *parent = nullptr);

  int number() const {
    return _number;
  }

  QString title() const {
    return _title;
  }

  QColor glowColor() const {
    return _glowColor;
  }

  QSize sizeHint() const override;

protected:
  void paintEvent(QPaintEvent *) override;

public slots:
  void setNumber(int number) {
    _number = number;
  }

  void setTitle(QString title) {
    _title = title;
  }

  void setGlowColor(const QColor &color);

  void resetBackgroundColor();
};

#endif // OUTPUTPANELBUTTON_H
