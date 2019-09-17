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

#ifndef PYTHONTABWIDGET_H
#define PYTHONTABWIDGET_H

#include <QTabWidget>

#include <tulip/tulipconf.h>

class TLP_PYTHON_SCOPE PythonTabWidget : public QTabWidget {

public:
  explicit PythonTabWidget(QWidget *parent = nullptr);

  void setDrawTabBarBgGradient(const bool drawGradient) {
    _drawGradient = drawGradient;
  }

  void setTextColor(const QColor &textColor) {
    _textColor = textColor;
  }

protected:
  void paintEvent(QPaintEvent *event) override;

  bool _drawGradient;
  QColor _textColor;
};

#endif // PYTHONTABWIDGET_H
