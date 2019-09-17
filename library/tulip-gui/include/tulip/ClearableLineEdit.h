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

#ifndef CLEARABLELINEEDIT_H
#define CLEARABLELINEEDIT_H

#include <QLineEdit>

#include <tulip/tulipconf.h>

class TLP_QT_SCOPE ClearableLineEdit : public QLineEdit {
  static QPixmap *CLEAR_PIXMAP;

  bool _clearButtonHovered;
  static void initPixmap();
  QRect pixmapRect();

public:
  explicit ClearableLineEdit(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
};

#endif // CLEARABLELINEEDIT_H
///@endcond
