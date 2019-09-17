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

#ifndef PLUGINSCENTER_
#define PLUGINSCENTER_

#include <tulip/TulipRelease.h>

#include <QWidget>
#include <QMap>
#include <QNetworkReply>

namespace Ui {
class PluginsCenterData;
}

class PluginInformationListItem;

class PluginsCenter : public QWidget {
  Q_OBJECT

  Ui::PluginsCenterData *_ui;

  QStringList _categoryFilters;
  QString _nameFilter;

  PluginInformationListItem *_currentItem;

public:
  explicit PluginsCenter(QWidget *parent = nullptr);
  ~PluginsCenter() override;

public slots:
  void reportPluginErrors(const QMap<QString, QString> &errors);

  void showErrorsPage();
  void showWelcomePage();
  void showRepositoriesPage();

  void searchAll();
  void searchAlgorithms();
  void searchImportExport();
  void searchGlyphs();
  void searchViews();
  void searchInteractors();
  void searchPerspectives();

  void setNameFilter(const QString &filter);
  void setCategoryFilter(const QString &filter);
  void setCategoryFilters(const QStringList &filters);

  void refreshFilter();
protected slots:
  void sideListRowChanged(int i);
  void itemFocused();

  void testingChecked(bool);
  void stableChecked(bool);
  void repoAdded();
  void repoRemoved();
};

#endif // PLUGINSCENTER_
