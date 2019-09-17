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

#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>

#include <tulip/tulipconf.h>
#include <tulip/Color.h>
#include <tulip/TulipMetaTypes.h>

namespace tlp {

class TLP_QT_SCOPE ChooseColorButton : public QPushButton {
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor)
  Q_PROPERTY(Color tulipColor READ tulipColor WRITE setTulipColor)

public:
  explicit ChooseColorButton(QWidget *parent = nullptr);

  QColor color() const;
  QString text() const;
  Color tulipColor() const;

  void setDialogParent(QWidget *w);

protected:
  QColor _color;
  QString _text;
  QWidget *_dialogParent;
  QString _dialogTitle;

signals:
  void colorChanged(QColor);
  void tulipColorChanged(Color);

public slots:
  void setColor(const QColor &);
  void setText(const QString &text);
  void setTulipColor(const Color &);
  void setDialogTitle(const QString &);

protected slots:
  void chooseColor();
};

class TLP_QT_SCOPE ColorButton : public ChooseColorButton {
  Q_OBJECT

public:
  explicit ColorButton(QWidget *parent = nullptr) : ChooseColorButton(parent) {}

protected:
  void paintEvent(QPaintEvent *) override;
};
} // namespace tlp

#endif // COLORBUTTON_H
///@endcond
