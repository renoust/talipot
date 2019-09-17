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

#ifndef PLUGINCREATIONDIALOG_H_
#define PLUGINCREATIONDIALOG_H_

#include <QDialog>

#include <tulip/tulipconf.h>

namespace Ui {
class PythonPluginCreationDialog;
}

namespace tlp {

class TLP_PYTHON_SCOPE PythonPluginCreationDialog : public QDialog {

  Q_OBJECT
  Ui::PythonPluginCreationDialog *_ui;

public:
  PythonPluginCreationDialog(QWidget *parent = nullptr);
  ~PythonPluginCreationDialog() override;
  QString getPluginFileName() const;

  QString getPluginType() const;

  QString getPluginClassName() const;

  QString getPluginName() const;

  QString getPluginAuthor() const;

  QString getPluginDate() const;

  QString getPluginInfo() const;

  QString getPluginRelease() const;

  QString getPluginGroup() const;

protected slots:

  void accept() override;

  void selectPluginSourceFile();
};
} // namespace tlp

#endif /* PLUGINCREATIONDIALOG_H_ */
