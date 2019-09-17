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

#include "PerspectiveCrashHandler.h"

#include "ui_PerspectiveCrashHandler.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QNetworkReply>
#include <QFile>
#include <QScreen>

#include "FormPost.h"
#include <talipot/Project.h>

static const QString SEPARATOR = "=======================\n";

PerspectiveCrashHandler::PerspectiveCrashHandler(QWidget *parent)
    : QDialog(parent), _ui(new Ui::PerspectiveCrashHandlerData), _isDetailedView(false) {
  _ui->setupUi(this);
  setDetailedView(false);
  connect(_ui->detailsLink, SIGNAL(linkActivated(QString)), this, SLOT(toggleDetailedView()));
  connect(_ui->sendReportButton, SIGNAL(clicked()), this, SLOT(sendReport()));
  // connect(_ui->saveButton,SIGNAL(clicked()),this,SLOT(saveData()));
  _ui->saveButton->hide();
  _ui->commentsEdit->moveCursor(QTextCursor::End);
  QPixmap px(":/talipot/gui/icons/logo32x32.png");
  // take care of the devicePixelRatio
  // before setting the label pixmap
  px.setDevicePixelRatio(QGuiApplication::primaryScreen()->devicePixelRatio());
  _ui->icon->setPixmap(px);
}

PerspectiveCrashHandler::~PerspectiveCrashHandler() {
  delete _ui;
}

void PerspectiveCrashHandler::setDetailedView(bool f) {
  _isDetailedView = f;
  _ui->detailsLink->setText(f ? "<a href=\"Hide details\">Hide details</a>"
                              : "<a href=\"Show details\">View details</a> <span "
                                "style=\"font-size:small\"><i>(sent with your "
                                "comments)</i></span>");
  _ui->stackedWidget->setCurrentIndex(int(f));
}

void PerspectiveCrashHandler::toggleDetailedView() {
  setDetailedView(!_isDetailedView);
}

void PerspectiveCrashHandler::sendReport() {
  _poster = new FormPost;

  _poster->addField("summary",
                    "[ Talipot " + _ui->versionValue->text() + " (" + _ui->plateformValue->text() +
                        ") ] Crash report from perspective: " + _ui->perspectiveNameValue->text() +
                        " " + _ui->perspectiveArgumentsValue->text());

  QString description = +"System:\n\n";
  description += "Plateform: " + _ui->plateformValue->text() + "\n";
  description += "Architecture: " + _ui->archValue->text() + "\n";
  description += "Compiler: " + _ui->compilerValue->text() + "\n";
  description += "Talipot version: " + _ui->versionValue->text() + "\n";
  description += SEPARATOR;
  description += "Perspective:\n";
  description += "Name: " + _ui->perspectiveNameValue->text() + "\n";
  description += "Arguments: " + _ui->perspectiveArgumentsValue->text() + "\n";
  description += SEPARATOR;
  description += "Stack trace:\n";
  description += _ui->dumpEdit->toPlainText();

  _poster->addField("description", description);
  _poster->addField("platform", _ui->compilerValue->text());
  _poster->addField("os", _ui->plateformValue->text());
  _poster->addField("os_build", _ui->archValue->text());
  _poster->addField("steps_to_reproduce", _ui->commentsEdit->toPlainText());

  connect(_poster->postData("http://tulip.labri.fr/devel/talipot_crash_report.php"),
          SIGNAL(finished()), this, SLOT(reportPosted()));

  _ui->sendReportButton->setText("Sending report...");
  _ui->sendReportButton->setEnabled(false);
  _ui->commentsEdit->setEnabled(false);
}

void PerspectiveCrashHandler::reportPosted() {
  QNetworkReply *reply = static_cast<QNetworkReply *>(sender());

  if (reply->error() == QNetworkReply::NoError) {
    _ui->sendReportButton->setText("Report sent");
    _ui->errorReportTitle->setText("<b>Report has been sent. Thank you for supporting Talipot!");
  } else {
    _ui->sendReportButton->setText("Error while sending report");
    _ui->errorReportTitle->setText("<i>" + reply->errorString() + "</i>");
    _ui->commentsEdit->setEnabled(true);
    _ui->sendReportButton->setEnabled(true);
  }

  sender()->deleteLater();
  _poster->deleteLater();
}

void PerspectiveCrashHandler::setEnvData(const QString &plateform, const QString &arch,
                                         const QString &compiler, const QString &version,
                                         const QString &stackTrace) {
  _ui->plateformValue->setText(plateform);
  _ui->archValue->setText(arch);
  _ui->compilerValue->setText(compiler);
  _ui->versionValue->setText(version);
  _ui->dumpEdit->setPlainText(stackTrace);
}

void PerspectiveCrashHandler::setPerspectiveData(const PerspectiveProcessInfo &info) {
  _perspectiveInfo = info;
  _ui->perspectiveNameValue->setText(info.name);
  QString args;

  for (const QString &a : info.args.keys())
    args += "--" + a + "=" + info.args[a].toString() + " ";

  _ui->perspectiveArgumentsValue->setText(args);
}
