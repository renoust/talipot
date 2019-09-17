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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
  Q_OBJECT

  Ui::PreferencesDialog *_ui;

public:
  explicit PreferencesDialog(QWidget *parent = nullptr);
  ~PreferencesDialog() override;

public slots:
  void writeSettings();
  void readSettings();
  void cellChanged(int row, int column);
  void randomSeedCheckChanged(int);
  void showGraphDefaultsContextMenu(const QPoint &);

private slots:
  void usetlpbformat(int state);
  void resetToTulipDefaults(int row = -1, int updateMode = 0);
};

#endif // PREFERENCESDIALOG_H
