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
#include "ImportWizard.h"
#include "ui_ImportWizard.h"

#include <QAbstractButton>

#include <tulip/TulipItemDelegate.h>
#include <tulip/ParameterListModel.h>
#include <tulip/ImportModule.h>

#include <tulip/GraphHierarchiesModel.h>
#include <tulip/PluginModel.h>
#include <tulip/TulipSettings.h>

using namespace tlp;
using namespace std;

ImportWizard::ImportWizard(QWidget *parent) : QWizard(parent), _ui(new Ui::ImportWizard) {
  _ui->setupUi(this);

  PluginModel<tlp::ImportModule> *model = new PluginModel<tlp::ImportModule>(_ui->importModules);

  _ui->importModules->setModel(model);
  _ui->importModules->setRootIndex(model->index(0, 0));
  _ui->importModules->expandAll();
  connect(_ui->importModules->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          this, SLOT(algorithmSelected(QModelIndex)));

  _ui->parametersList->setItemDelegate(new TulipItemDelegate(_ui->parametersList));
  _ui->parametersList->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  connect(_ui->importModules, SIGNAL(doubleClicked(QModelIndex)), button(QWizard::FinishButton),
          SLOT(click()));
  // display OK instead of Finish
  setButtonText(QWizard::FinishButton, "OK");

  _ui->parametersFrame->hide();
  QString importLabel("<html><head/><body><p align=\"justify\">Import a graph hierarchy into your "
                      "project. First, select an import method, then adjust its parameters if "
                      "needed.<br/>Click <b>Ok</b> to import your graph, then visualize it using "
                      "the ");

  if (TulipSettings::instance().displayDefaultViews())
    importLabel += "<b>Node Link Diagram</b> and <b>Spreadsheet</b> (automatically opened) views.";
  else
    importLabel += "<img src=\":/tulip/gui/icons/16/list-add.png\"/>&nbsp;<b>Add "
                   "panel</b> button to open specific views on it.";

  importLabel += "<br/><br/>See <b>Edit</b> menu, then <b>Preferences</b> for more options when "
                 "importing a graph.</p></body></html>";
  _ui->label->setText(importLabel);

  updateFinishButton();
}

ImportWizard::~ImportWizard() {
  delete _ui->parametersList->model();
  delete _ui;
}

void ImportWizard::algorithmSelected(const QModelIndex &index) {
  QString alg(index.data().toString());
  string algs = tlp::QStringToTlpString(alg);
  _ui->parametersFrame->setVisible(!alg.isEmpty());
  QAbstractItemModel *oldModel = _ui->parametersList->model();
  QAbstractItemModel *newModel = nullptr;
  bool isGroup = index.model()->index(0, index.column(), index).isValid();

  QString categoryText("<b>Category</b>");
  QString parametersText("<b>Parameters</b>");

  if (!isGroup && PluginLister::pluginExists(algs)) {
    newModel = new ParameterListModel(PluginLister::getPluginParameters(algs));
    parametersText += "&nbsp;<font size=-2>[" + alg + "]</font>";
    std::string group = PluginLister::pluginInformation(algs).group();

    if (!group.empty())
      categoryText += "&nbsp;<font size=-2>[" + tlpStringToQString(group) + "]</font>";
  } else
    categoryText += "&nbsp;<font size=-2>[" + alg + "]</font>";

  _ui->categoryLabel->setText(categoryText);
  _ui->parametersLabel->setText(parametersText);

  _ui->parametersList->setModel(newModel);

  delete oldModel;
  updateFinishButton();
}

QString ImportWizard::algorithm() const {
  if (_ui->importModules->selectionModel()->hasSelection())
    return _ui->importModules->selectionModel()->selectedIndexes()[0].data().toString();

  return "";
}

tlp::DataSet ImportWizard::parameters() const {
  ParameterListModel *model = dynamic_cast<ParameterListModel *>(_ui->parametersList->model());

  if (model == nullptr)
    return DataSet();

  return model->parametersValues();
}

void ImportWizard::updateFinishButton() {
  button(QWizard::FinishButton)->setEnabled(_ui->parametersList->model() != nullptr);
}
