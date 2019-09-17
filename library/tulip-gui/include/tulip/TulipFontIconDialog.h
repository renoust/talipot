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
#ifndef TULIPFONTICONDIALOG_H
#define TULIPFONTICONDIALOG_H

#include <tulip/tulipconf.h>

#include <QDialog>
#include <QString>
#include <QIcon>

namespace Ui {
class TulipFontIconDialog;
}

namespace tlp {

class TLP_QT_SCOPE TulipFontIconDialog : public QDialog {

  Q_OBJECT

  Ui::TulipFontIconDialog *_ui;
  QString _selectedIconName;

public:
  TulipFontIconDialog(QWidget *parent = nullptr);

  QString getSelectedIconName() const;

  void setSelectedIconName(const QString &iconName);

  void accept() override;

  void showEvent(QShowEvent *) override;

protected slots:

  void updateIconList();

  void openUrlInBrowser(const QString &url);
};
} // namespace tlp

#endif
///@endcond
