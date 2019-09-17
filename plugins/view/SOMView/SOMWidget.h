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

#ifndef SOMWIDGET_H_
#define SOMWIDGET_H_

#include <QWidget>

namespace Ui {
class SOMWidget;
}

namespace tlp {
class SOMWidget : public QWidget {
  Q_OBJECT

  Ui::SOMWidget *_ui;

public:
  SOMWidget(QWidget *parent = nullptr);
  ~SOMWidget() override;
};
} // namespace tlp

#endif /* SOMWIDGET_H_ */
