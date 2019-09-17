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

#ifndef SIMPLESTRINGSLISTSELECTIONWIDGET_CPP_
#define SIMPLESTRINGSLISTSELECTIONWIDGET_CPP_

#include <tulip/SimpleStringsListSelectionWidget.h>
#include <tulip/TlpQtTools.h>

#include "ui_SimpleStringsListSelectionWidget.h"

using namespace std;

namespace tlp {

SimpleStringsListSelectionWidget::SimpleStringsListSelectionWidget(
    QWidget *parent, const unsigned int maxSelectedStringsListSize)
    : QWidget(parent), _ui(new Ui::SimpleStringsListSelectionData()),
      maxSelectedStringsListSize(maxSelectedStringsListSize) {

  _ui->setupUi(this);

  if (maxSelectedStringsListSize != 0) {
    _ui->selectButton->setEnabled(false);
  } else {
    _ui->selectButton->setEnabled(true);
  }

  qtWidgetsConnection();
}

SimpleStringsListSelectionWidget::~SimpleStringsListSelectionWidget() {
  delete _ui;
}

void SimpleStringsListSelectionWidget::qtWidgetsConnection() {
  connect(_ui->listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this,
          SLOT(listItemClicked(QListWidgetItem *)));
  connect(_ui->upButton, SIGNAL(clicked()), this, SLOT(pressButtonUp()));
  connect(_ui->downButton, SIGNAL(clicked()), this, SLOT(pressButtonDown()));
  connect(_ui->selectButton, SIGNAL(clicked()), this, SLOT(pressButtonSelectAll()));
  connect(_ui->unselectButton, SIGNAL(clicked()), this, SLOT(pressButtonUnselectAll()));
}

void SimpleStringsListSelectionWidget::setUnselectedStringsList(
    const std::vector<std::string> &unselectedStringsList) {
  for (unsigned int i = 0; i < unselectedStringsList.size(); ++i) {
    QList<QListWidgetItem *> items =
        _ui->listWidget->findItems(tlpStringToQString(unselectedStringsList[i]), Qt::MatchExactly);

    if (items.size() > 0) {
      items[0]->setFlags(items[0]->flags() | Qt::ItemIsUserCheckable);
      items[0]->setCheckState(Qt::Unchecked);
    } else {
      QListWidgetItem *item = new QListWidgetItem(tlpStringToQString(unselectedStringsList[i]));
      item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
      item->setCheckState(Qt::Unchecked);
      _ui->listWidget->addItem(item);
    }
  }
}

void SimpleStringsListSelectionWidget::setSelectedStringsList(
    const std::vector<std::string> &selectedStringsList) {
  for (unsigned int i = 0; i < selectedStringsList.size(); ++i) {
    if (maxSelectedStringsListSize != 0 &&
        getSelectedStringsList().size() == maxSelectedStringsListSize) {
      break;
    }

    QList<QListWidgetItem *> items =
        _ui->listWidget->findItems(tlpStringToQString(selectedStringsList[i]), Qt::MatchExactly);

    if (items.size() > 0) {
      items[0]->setFlags(items[0]->flags() | Qt::ItemIsUserCheckable);
      items[0]->setCheckState(Qt::Checked);
    } else {
      QListWidgetItem *item = new QListWidgetItem(tlpStringToQString(selectedStringsList[i]));
      item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
      item->setCheckState(Qt::Checked);
      _ui->listWidget->addItem(item);
    }
  }
}

void SimpleStringsListSelectionWidget::clearUnselectedStringsList() {
  vector<QListWidgetItem *> itemsToDelete;

  for (int i = 0; i < _ui->listWidget->count(); ++i) {
    QListWidgetItem *item = _ui->listWidget->item(i);

    if (item->checkState() == Qt::Unchecked) {
      itemsToDelete.push_back(item);
    }
  }

  for (unsigned int i = 0; i < itemsToDelete.size(); ++i) {
    delete itemsToDelete[i];
  }
}

void SimpleStringsListSelectionWidget::clearSelectedStringsList() {
  vector<QListWidgetItem *> itemsToDelete;

  for (int i = 0; i < _ui->listWidget->count(); ++i) {
    QListWidgetItem *item = _ui->listWidget->item(i);

    if (item->checkState() == Qt::Checked) {
      itemsToDelete.push_back(item);
    }
  }

  for (unsigned int i = 0; i < itemsToDelete.size(); ++i) {
    delete itemsToDelete[i];
  }
}

void SimpleStringsListSelectionWidget::setMaxSelectedStringsListSize(
    const unsigned int maxSelectedStringsListSize) {
  this->maxSelectedStringsListSize = maxSelectedStringsListSize;

  if (maxSelectedStringsListSize != 0) {
    _ui->selectButton->setEnabled(false);
  } else {
    _ui->selectButton->setEnabled(true);
  }
}

vector<string> SimpleStringsListSelectionWidget::getSelectedStringsList() const {
  vector<string> ret;

  for (int i = 0; i < _ui->listWidget->count(); ++i) {
    QListWidgetItem *item = _ui->listWidget->item(i);

    if (item->checkState() == Qt::Checked) {
      ret.push_back(QStringToTlpString(item->text()));
    }
  }

  return ret;
}

vector<string> SimpleStringsListSelectionWidget::getUnselectedStringsList() const {
  vector<string> ret;

  for (int i = 0; i < _ui->listWidget->count(); ++i) {
    QListWidgetItem *item = _ui->listWidget->item(i);

    if (item->checkState() == Qt::Unchecked) {
      ret.push_back(QStringToTlpString(item->text()));
    }
  }

  return ret;
}

void SimpleStringsListSelectionWidget::selectAllStrings() {
  for (int i = 0; i < _ui->listWidget->count(); ++i) {
    QListWidgetItem *item = _ui->listWidget->item(i);
    item->setCheckState(Qt::Checked);
  }
}

void SimpleStringsListSelectionWidget::unselectAllStrings() {
  for (int i = 0; i < _ui->listWidget->count(); ++i) {
    QListWidgetItem *item = _ui->listWidget->item(i);
    item->setCheckState(Qt::Unchecked);
  }
}

void SimpleStringsListSelectionWidget::pressButtonUp() {
  if (_ui->listWidget->count() > 0) {
    int row = _ui->listWidget->currentRow();

    if (row > 0) {
      QListWidgetItem *item1 = new QListWidgetItem(*(_ui->listWidget->currentItem()));
      QListWidgetItem *item2 = new QListWidgetItem(*(_ui->listWidget->item(row - 1)));
      delete _ui->listWidget->item(row - 1);
      delete _ui->listWidget->item(row - 1);
      _ui->listWidget->insertItem(row - 1, item2);
      _ui->listWidget->insertItem(row - 1, item1);
      _ui->listWidget->setCurrentRow(row - 1);
    }
  }
}

void SimpleStringsListSelectionWidget::pressButtonDown() {
  if (_ui->listWidget->count() > 0) {
    int row = _ui->listWidget->currentRow();

    if (row != -1 && row < (_ui->listWidget->count() - 1)) {
      QListWidgetItem *item1 = new QListWidgetItem(*(_ui->listWidget->currentItem()));
      QListWidgetItem *item2 = new QListWidgetItem(*(_ui->listWidget->item(row + 1)));
      delete _ui->listWidget->item(row);
      delete _ui->listWidget->item(row);
      _ui->listWidget->insertItem(row, item1);
      _ui->listWidget->insertItem(row, item2);
      _ui->listWidget->setCurrentRow(row + 1);
    }
  }
}

void SimpleStringsListSelectionWidget::pressButtonSelectAll() {
  selectAllStrings();
}

void SimpleStringsListSelectionWidget::pressButtonUnselectAll() {
  unselectAllStrings();
}

void SimpleStringsListSelectionWidget::listItemClicked(QListWidgetItem *item) {
  if (maxSelectedStringsListSize != 0 &&
      getSelectedStringsList().size() > maxSelectedStringsListSize) {
    if (item->checkState() == Qt::Checked) {
      item->setCheckState(Qt::Unchecked);
    }
  }
}
} // namespace tlp

#endif /* SIMPLESTRINGSLISTSELECTIONWIDGET_CPP_ */
