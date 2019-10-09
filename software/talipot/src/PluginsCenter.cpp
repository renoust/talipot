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

#include "PluginsCenter.h"

#include "PluginErrorReport.h"
#include "PluginInformationListItem.h"

#include <talipot/Algorithm.h>
#include <talipot/TemplateAlgorithm.h>
#include <talipot/PropertyAlgorithm.h>
#include <talipot/View.h>
#include <talipot/Perspective.h>
#include <talipot/Interactor.h>
#include <talipot/Glyph.h>
#include <talipot/EdgeExtremityGlyph.h>
#include <talipot/ImportModule.h>
#include <talipot/ExportModule.h>
#include <talipot/PluginManager.h>
#include <talipot/Settings.h>

#include "ui_PluginsCenter.h"

static const int ALL_ROW = 0;
static const int ALGORITHMS_ROW = 1;
static const int IMPORTEXPORT_ROW = 2;
static const int GLYPHS_ROW = 3;
static const int VIEWS_ROW = 4;
static const int INTERACTORS_ROW = 5;
static const int PERSPECTIVES_ROW = 6;
static const int ERRORS_ROW = 8;

using namespace tlp;

PluginsCenter::PluginsCenter(QWidget *parent)
    : QWidget(parent), _ui(new Ui::PluginsCenterData()), _currentItem(nullptr) {
  _ui->setupUi(this);
}

PluginsCenter::~PluginsCenter() {
  delete _ui;
}

void PluginsCenter::reportPluginErrors(const QMap<QString, QString> &errors) {
  if (!errors.empty())
    _ui->pluginsSideList->item(ERRORS_ROW)
        ->setFlags(Qt::ItemIsEnabled | _ui->pluginsSideList->item(ERRORS_ROW)->flags());

  for (const QString &k : errors.keys()) {
    _ui->errorsLogAreaLayout->addWidget(new PluginErrorReport(k, errors[k]));
  }

  _ui->errorsLogAreaLayout->addItem(
      new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Expanding));
}

void PluginsCenter::showErrorsPage() {
  _ui->pluginsContent->setCurrentWidget(_ui->errorsPage);
}

void PluginsCenter::showWelcomePage() {
  _ui->pluginsContent->setCurrentWidget(_ui->homePage);
}

void PluginsCenter::searchAll() {
  setCategoryFilter("");
}

void PluginsCenter::searchAlgorithms() {
  setCategoryFilters(QStringList()
                     << tlp::ALGORITHM_CATEGORY.c_str() << tlp::BOOLEAN_ALGORITHM_CATEGORY.c_str()
                     << tlp::COLOR_ALGORITHM_CATEGORY.c_str()
                     << tlp::DOUBLE_ALGORITHM_CATEGORY.c_str()
                     << tlp::INTEGER_ALGORITHM_CATEGORY.c_str()
                     << tlp::LAYOUT_ALGORITHM_CATEGORY.c_str()
                     << tlp::STRING_ALGORITHM_CATEGORY.c_str()
                     << tlp::PROPERTY_ALGORITHM_CATEGORY.c_str());
}

void PluginsCenter::searchImportExport() {
  setCategoryFilters(QStringList() << tlp::IMPORT_CATEGORY.c_str() << tlp::EXPORT_CATEGORY.c_str());
}

void PluginsCenter::searchGlyphs() {
  setCategoryFilters(QStringList() << tlp::GLYPH_CATEGORY.c_str() << tlp::EEGLYPH_CATEGORY.c_str());
}

void PluginsCenter::searchViews() {
  setCategoryFilter(tlp::VIEW_CATEGORY.c_str());
}

void PluginsCenter::searchInteractors() {
  setCategoryFilter(tlp::INTERACTOR_CATEGORY.c_str());
}

void PluginsCenter::searchPerspectives() {
  setCategoryFilter(tlp::PERSPECTIVE_CATEGORY.c_str());
}

void PluginsCenter::setCategoryFilter(const QString &filter) {
  setCategoryFilters(QStringList() << filter);
}

void PluginsCenter::setCategoryFilters(const QStringList &filters) {
  _categoryFilters = filters;
  refreshFilter();
}

void PluginsCenter::setNameFilter(const QString &filter) {
  _nameFilter = filter;
  refreshFilter();
}

void PluginsCenter::refreshFilter() {
  _currentItem = nullptr;

  if (_categoryFilters.isEmpty())
    _categoryFilters.push_back("");

  QVBoxLayout *lyt = new QVBoxLayout();

  for (const QString &cf : _categoryFilters) {
    for (const PluginInformation &info : PluginManager::listPlugins(
             PluginManager::Remote | PluginManager::Local, _nameFilter, cf)) {
      PluginInformationListItem *item = new PluginInformationListItem(info);
      connect(item, SIGNAL(focused()), this, SLOT(itemFocused()));
      lyt->addWidget(item);
    }
  }

  lyt->addItem(new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Expanding));

  QString oldObjName = _ui->pluginsSearchListContent->objectName();
  _ui->pluginsSearchList->setWidget(nullptr);
  _ui->pluginsSearchListContent = new QWidget();
  _ui->pluginsSearchListContent->setObjectName(oldObjName);
  _ui->pluginsSearchListContent->setLayout(lyt);
  _ui->pluginsSearchList->setWidget(_ui->pluginsSearchListContent);
  _ui->pluginsContent->setCurrentWidget(_ui->pluginsListPage);
}

void PluginsCenter::sideListRowChanged(int i) {
  switch (i) {
  case ALL_ROW:
    searchAll();
    break;

  case ALGORITHMS_ROW:
    searchAlgorithms();
    break;

  case IMPORTEXPORT_ROW:
    searchImportExport();
    break;

  case GLYPHS_ROW:
    searchGlyphs();
    break;

  case VIEWS_ROW:
    searchViews();
    break;

  case INTERACTORS_ROW:
    searchInteractors();
    break;

  case PERSPECTIVES_ROW:
    searchPerspectives();
    break;

  case ERRORS_ROW:
    showErrorsPage();
    break;
  }
}

void PluginsCenter::itemFocused() {
  if (_currentItem != nullptr) {
    _currentItem->focusOut();
  }

  _currentItem = static_cast<PluginInformationListItem *>(sender());
  _currentItem->focusIn();
}
