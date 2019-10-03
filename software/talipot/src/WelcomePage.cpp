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

#include "WelcomePage.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QStyle>

#include <talipot/Settings.h>
#include <talipot/PluginManager.h>
#include <talipot/PluginsManager.h>
#include <talipot/Perspective.h>

#include "PerspectiveItemWidget.h"
#include "ui_WelcomePage.h"

static const unsigned RSS_LIMIT = 3;

using namespace tlp;

WelcomePage::WelcomePage(QWidget *parent) : QWidget(parent), _ui(new Ui::WelcomePageData) {
  _ui->setupUi(this);

  // Finalize Ui
  _ui->openProjectButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon));
  connect(_ui->openProjectButton, SIGNAL(clicked()), this, SIGNAL(openProject()));

  // Fetch RSS
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(rssReply(QNetworkReply *)));

  // Recent documents list
  Settings::instance().checkRecentDocuments();
  QStringList recentDocs = Settings::instance().recentDocuments();

  if (!recentDocs.empty()) {
    QString txt;

    for (const QString &txt2 : recentDocs)
      txt += "<p><span><img src=\":/talipot/gui/ui/list_bullet_arrow.png\"></img>   <a href=\"" +
             txt2 + "\">" + txt2 + "</a>" + "</span></p><p/>";

    _ui->recentDocumentsLabel->setText(txt);
  }

  std::list<std::string> perspectives = PluginsManager::availablePlugins<tlp::Perspective>();

  for (std::list<std::string>::iterator it = perspectives.begin(); it != perspectives.end(); ++it) {
    _ui->perspectivesFrame->layout()->addWidget(new PerspectiveItemWidget(it->c_str()));
  }

  _ui->perspectivesFrame->layout()->addItem(
      new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Expanding));
}

WelcomePage::~WelcomePage() {
  delete _ui;
}

void WelcomePage::rssReply(QNetworkReply *reply) {
  sender()->deleteLater();
  QXmlStreamReader xmlReader(reply);
  unsigned i = 0;
  QVBoxLayout *rssLayout = new QVBoxLayout;
  rssLayout->setContentsMargins(0, 0, 0, 0);
  rssLayout->setSpacing(30);

  while (!xmlReader.atEnd() && i < RSS_LIMIT) {
    if (xmlReader.readNextStartElement()) {
      QString title, description;

      if (xmlReader.name() == "item") {
        ++i;
        QXmlStreamReader::TokenType p(xmlReader.readNext());

        while (xmlReader.name() != "item" && p != QXmlStreamReader::EndElement) {
          xmlReader.readNextStartElement();

          if (xmlReader.name() == "title")
            title = xmlReader.readElementText();

          if (xmlReader.name() == "description")
            description = xmlReader.readElementText();
        }

        QString text("<p><span style=\"color:#626262; font-size:large;\">");
        text += title + "</span></p><p><span>" + description + "</span></p>";
        QLabel *label = new QLabel(text, nullptr);
        label->setMinimumWidth(1);
        label->setWordWrap(true);
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        connect(label, SIGNAL(linkActivated(QString)), this, SLOT(openLink(QString)));
        rssLayout->addWidget(label);
      }
    }
  }
}

void WelcomePage::openLink(const QString &link) {
  QDesktopServices::openUrl(link);
}

void WelcomePage::recentFileLinkActivated(const QString &link) {
  if (!QFileInfo(link).exists())
    QMessageBox::critical(this, "Error", "Selected recent project does not exist anymore");
  else
    emit openFile(link);
}
