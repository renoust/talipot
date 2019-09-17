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

#ifndef INTERACTORCONFIGWIDGET_H
#define INTERACTORCONFIGWIDGET_H

#include <QDialog>

namespace Ui {
class InteractorConfigWidget;
}

class QShowEvent;

namespace tlp {
class Interactor;

class InteractorConfigWidget : public QDialog {

  Ui::InteractorConfigWidget *_ui;
  Interactor *_interactor;

public:
  explicit InteractorConfigWidget(QWidget *parent = nullptr);
  ~InteractorConfigWidget();
  bool setWidgets(Interactor *interactor);
  void clearWidgets();

  void showEvent(QShowEvent *) override;
};

} // namespace tlp

#endif // INTERACTORCONFIGWIDGET_H
