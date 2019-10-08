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

#include <talipot/Perspective.h>
#include <talipot/Project.h>

#include <QApplication>
#include <QMainWindow>
#include <QProcess>
#include <QTcpSocket>
#include <QHostAddress>
#include <QAction>
#include <QStatusBar>

#include <talipot/SimplePluginProgressWidget.h>

using namespace tlp;

tlp::Perspective *Perspective::_instance = nullptr;

Perspective *Perspective::instance() {
  return _instance;
}
void Perspective::setInstance(Perspective *p) {
  _instance = p;
}

Perspective::Perspective(const tlp::PluginContext *c)
    : _agentSocket(nullptr), _maximised(false), _project(nullptr), _mainWindow(nullptr),
      _externalFile(QString()), _parameters(QVariantMap()) {
  if (c != nullptr) {
    const PerspectiveContext *perspectiveContext = static_cast<const PerspectiveContext *>(c);
    _mainWindow = perspectiveContext->mainWindow;
    _project = perspectiveContext->project;
    _externalFile = perspectiveContext->externalFile;
    _parameters = perspectiveContext->parameters;
    _perspectiveId = perspectiveContext->id;

    if (perspectiveContext->talipotPort != 0) {
      _agentSocket = new QTcpSocket(this);
      _agentSocket->connectToHost(QHostAddress::LocalHost, perspectiveContext->talipotPort);

      if (!_agentSocket->waitForConnected(2000)) {
        _agentSocket->deleteLater();
        _agentSocket = nullptr;
      }

      if (_project != nullptr) {
        notifyProjectLocation(_project->absoluteRootPath());
      }
    } else {
      qWarning("Perspective running in standalone mode");
    }
  }
}

Perspective::~Perspective() {
  delete _project;

  if (this == _instance)
    _instance = nullptr;
}

PluginProgress *Perspective::progress(ProgressOptions options) {
  SimplePluginProgressDialog *dlg = new SimplePluginProgressDialog(_mainWindow);
  dlg->setWindowIcon(_mainWindow->windowIcon());
  dlg->showPreview(options.testFlag(IsPreviewable));
  dlg->setCancelButtonVisible(options.testFlag(IsCancellable));
  dlg->setStopButtonVisible(options.testFlag(IsStoppable));
  dlg->show();
  QApplication::processEvents();
  return dlg;
}

QMainWindow *Perspective::mainWindow() const {
  return _mainWindow;
}

void Perspective::showFullScreen(bool f) {
  if (f) {
    _maximised = _mainWindow->isMaximized();
    _mainWindow->showFullScreen();
  } else {
    _mainWindow->showNormal();

    if (_maximised)
      _mainWindow->showMaximized();
  }
}

void Perspective::registerReservedProperty(QString s) {
  _reservedProperties.insert(s);
}

void Perspective::centerPanelsForGraph(Graph *) {}

bool Perspective::isReservedPropertyName(QString s) {
  return _reservedProperties.contains(s);
}

bool Perspective::checkSocketConnected() {
  if (_agentSocket != nullptr) {
    if (_agentSocket->state() != QAbstractSocket::UnconnectedState)
      return true;
    else {
      _agentSocket->deleteLater();
      _agentSocket = nullptr;
      qWarning("Tulip launcher closed, now running in standalone mode");
    }
  }

  return false;
}

void Perspective::sendAgentMessage(const QString &msg) {
  if (checkSocketConnected()) {
    _agentSocket->write(msg.toUtf8());
    _agentSocket->flush();
  }
}

void Perspective::showPluginsCenter() {
  sendAgentMessage("SHOW_AGENT\tPLUGINS");
}

void Perspective::showProjectsPage() {
  sendAgentMessage("SHOW_AGENT\tPROJECTS");
}

void Perspective::showAboutPage() {
  sendAgentMessage("SHOW_AGENT\tABOUT");
}

void Perspective::showTrayMessage(const QString &s) {
  sendAgentMessage("TRAY_MESSAGE\t" + s);
}

void Perspective::showErrorMessage(const QString &title, const QString &s) {
  sendAgentMessage("ERROR_MESSAGE\t" + title + " " + s);
}

void Perspective::openProjectFile(const QString &path) {
  if (checkSocketConnected()) {
    sendAgentMessage("OPEN_PROJECT\t" + path);
  } else { // on standalone mode, spawn a new standalone perspective
    QProcess::startDetached(QApplication::applicationFilePath(), QStringList() << path);
  }
}

void Perspective::createPerspective(const QString &name) {
  if (checkSocketConnected()) {
    sendAgentMessage("CREATE_PERSPECTIVE\t" + name);
  } else { // on standalone mode, spawn a new standalone perspective
    QProcess::startDetached(QApplication::applicationFilePath(), QStringList()
                                                                     << "--perspective=" + name);
  }
}

void Perspective::notifyProjectLocation(const QString &path) {
  sendAgentMessage("PROJECT_LOCATION\t" + QString::number(_perspectiveId) + " " + path);
}
