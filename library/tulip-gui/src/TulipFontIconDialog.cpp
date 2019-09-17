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

#include <tulip/TulipFontIconDialog.h>
#include <tulip/TulipFontIconEngine.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/TulipMaterialDesignIcons.h>
#include <tulip/TlpQtTools.h>

#include <QRegExp>
#include <QDesktopServices>
#include <QUrl>

#include "ui_TulipFontIconDialog.h"

using namespace tlp;

TulipFontIconDialog::TulipFontIconDialog(QWidget *parent)
    : QDialog(parent), _ui(new Ui::TulipFontIconDialog) {

  _ui->setupUi(this);

  _ui->iconsCreditLabel->setText(
      QString("<html><head/><body><p><span style=\" font-size:8pt;\">Special credit for the design "
              "of icons goes to:</span><br/><span style=\" font-size:8pt; font-weight:600;\">Font "
              "Awesome </span><span style=\"font-size:8pt; color:#0000ff;\"><a "
              "href=\"http://fontawesome.com\">http://fontawesome.com</a></span><span style=\" "
              "font-size:8pt;\"> (v%1)</span><br/><span style=\"font-size:8pt; "
              "font-weight:600;\">Material Design Icons </span><span "
              "style=\"font-size:8pt;color:#0000ff;\"><a "
              "href=\"https://materialdesignicons.com\">https://materialdesignicons.com</a></"
              "span><span style=\" font-size:8pt;\"> (v%2)</span></p></body></html>")
          .arg(TulipFontAwesome::getVersion().c_str())
          .arg(TulipMaterialDesignIcons::getVersion().c_str()));
  connect(_ui->iconNameFilterLineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(updateIconList()));
  connect(_ui->iconsCreditLabel, SIGNAL(linkActivated(const QString &)), this,
          SLOT(openUrlInBrowser(const QString &)));

  updateIconList();
}

void TulipFontIconDialog::updateIconList() {
  _ui->iconListWidget->clear();

  QRegExp regexp(_ui->iconNameFilterLineEdit->text());

  std::vector<std::string> iconNames = TulipFontAwesome::getSupportedIcons();

  for (std::vector<std::string>::const_iterator it = iconNames.begin(); it != iconNames.end();
       ++it) {
    QString iconName = tlpStringToQString(*it);

    if (regexp.indexIn(iconName) != -1) {
      _ui->iconListWidget->addItem(new QListWidgetItem(TulipFontIconEngine::icon(*it), iconName));
    }
  }

  iconNames = TulipMaterialDesignIcons::getSupportedIcons();

  for (std::vector<std::string>::const_iterator it = iconNames.begin(); it != iconNames.end();
       ++it) {
    QString iconName = tlpStringToQString(*it);

    if (regexp.indexIn(iconName) != -1) {
      _ui->iconListWidget->addItem(new QListWidgetItem(TulipFontIconEngine::icon(*it), iconName));
    }
  }

  if (_ui->iconListWidget->count() > 0) {
    _ui->iconListWidget->setCurrentRow(0);
  }
}

QString TulipFontIconDialog::getSelectedIconName() const {
  return _selectedIconName;
}

void TulipFontIconDialog::setSelectedIconName(const QString &iconName) {
  QList<QListWidgetItem *> items = _ui->iconListWidget->findItems(iconName, Qt::MatchExactly);

  if (!items.isEmpty()) {
    _ui->iconListWidget->setCurrentItem(items.at(0));
    _selectedIconName = iconName;
  }
}

void TulipFontIconDialog::accept() {
  if (_ui->iconListWidget->count() > 0) {
    _selectedIconName = _ui->iconListWidget->currentItem()->text();
  }

  QDialog::accept();
}

void TulipFontIconDialog::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  _selectedIconName = _ui->iconListWidget->currentItem()->text();

  if (parentWidget())
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() - rect().center());
}

void TulipFontIconDialog::openUrlInBrowser(const QString &url) {
  QDesktopServices::openUrl(QUrl(url));
}
