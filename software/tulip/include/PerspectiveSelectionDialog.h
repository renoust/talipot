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

#ifndef PERSPECTIVESELECTIONDIALOG_H
#define PERSPECTIVESELECTIONDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class PerspectiveSelectionDialog;
}

class PerspectiveSelectionDialog : public QDialog {
  Q_OBJECT

  Ui::PerspectiveSelectionDialog *_ui;
  QString _perspective;

public:
  explicit PerspectiveSelectionDialog(QWidget *parent = nullptr);
  ~PerspectiveSelectionDialog() override;
  QString perspectiveName() const;

protected slots:
  void selectionChanged(const QModelIndex &a, const QModelIndex &b);
};

#endif // PERSPECTIVESELECTIONDIALOG_H
