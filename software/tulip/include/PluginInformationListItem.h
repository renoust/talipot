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
#ifndef PLUGININFORMATIONLISTITEM_H
#define PLUGININFORMATIONLISTITEM_H

#include <QWidget>
#include <tulip/PluginManager.h>

class QNetworkReply;
namespace Ui {
class PluginInformationListItemData;
}

class PluginInformationListItem : public QWidget {
  Q_OBJECT
  Ui::PluginInformationListItemData *_ui;

  tlp::PluginInformation _info;

public:
  explicit PluginInformationListItem(tlp::PluginInformation, QWidget *parent = nullptr);
  ~PluginInformationListItem() override;
  QWidget *description();

public slots:
  void focusOut();
  void focusIn();
  void install();
  void remove();

signals:
  void focused();

protected slots:
  void downloadProgress(qint64, qint64);

protected:
  void enterEvent(QEvent *) override;
};

#endif // PLUGININFORMATIONLISTITEM_H
