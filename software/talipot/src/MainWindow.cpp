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

#include "MainWindow.h"

#include <QMenu>
#include <QFileDialog>
#include <QCloseEvent>
#include <QFile>
#include <QProcess>
#include <QDir>
#include <QScrollArea>
#include <QGuiApplication>
#include <QScreen>

#include <talipot/Release.h>
#include <talipot/PythonVersionChecker.h>
#include <talipot/PluginsManager.h>
#include <talipot/Perspective.h>
#include <talipot/TlpTools.h>
#include <talipot/Settings.h>
#include <talipot/Project.h>
#include <talipot/PluginModel.h>
#include <talipot/TlpQtTools.h>

#include "ui_MainWindow.h"
#include "PerspectiveProcessHandler.h"
#include "WelcomePage.h"
#include "PerspectiveItemWidget.h"
#include "PerspectiveSelectionDialog.h"

using namespace tlp;

MainWindow *MainWindow::_instance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _errorMessage(new QLabel()), _ui(new Ui::MainWindowData),
      _systemTrayIcon(nullptr) {
  _ui->setupUi(this);

  QLabel *errorIcon = new QLabel();
  errorIcon->setMaximumSize(16, 16);
  errorIcon->setMinimumSize(16, 16);
  errorIcon->setPixmap(QPixmap(":/talipot/app/icons/16/dialog-error.png"));
  _ui->statusBar->insertWidget(0, errorIcon);
  _ui->statusBar->insertWidget(1, _errorMessage);

  _ui->statusBar->setVisible(false);
  _pageChoosers.clear();
  _pageChoosers.push_back(_ui->welcomePageChooser);
  _pageChoosers.push_back(_ui->pluginsPageChooser);
  _pageChoosers.push_back(_ui->aboutPageChooser);

  QPixmap logo(tlpStringToQString(TalipotBitmapDir + "/welcomelogo.png"));
  // take care of the devicePixelRatio
  // before setting the logo
  logo.setDevicePixelRatio(QGuiApplication::primaryScreen()->devicePixelRatio());
  _ui->mainLogo->setPixmap(logo);
  // set title
  QString title("Talipot ");

  // show patch number only if needed
  if (TALIPOT_INT_VERSION % 10)
    title += TALIPOT_VERSION;
  else
    title += TALIPOT_MM_VERSION;

  setWindowTitle(title);
  _ui->mainTitle->setText(
      QString("<html><head/><body><p align=\"center\"><span style=\" font-family:'MS Shell Dlg 2'; "
              "font-size:18pt; font-weight:600;\">") +
      title + "</span></p></body></html>");

  connect(_ui->welcomePageChooser, SIGNAL(clicked()), this, SLOT(pageChooserClicked()));
  connect(_ui->pluginsPageChooser, SIGNAL(clicked()), this, SLOT(pageChooserClicked()));
  connect(_ui->aboutPageChooser, SIGNAL(clicked()), this, SLOT(pageChooserClicked()));

  // System tray
  _systemTrayIcon = new QSystemTrayIcon(QIcon(":/talipot/gui/icons/talipot.ico"), this);
  _systemTrayIcon->setToolTip("Talipot agent");
  QMenu *systemTrayMenu = new QMenu();
  systemTrayMenu->addAction("Show", this, SLOT(showProjectsCenter()));
  systemTrayMenu->addAction("Hide", this, SLOT(hide()));
  systemTrayMenu->addSeparator();
  systemTrayMenu->addAction("Projects", this, SLOT(showProjectsCenter()));
  systemTrayMenu->addAction("Plugins center", this, SLOT(showPluginsCenter()));
  systemTrayMenu->addAction("About us", this, SLOT(showAboutCenter()));
  systemTrayMenu->addSeparator();
  connect(systemTrayMenu->addAction("Exit"), SIGNAL(triggered()), this, SLOT(close()));
  connect(_ui->exitButton, SIGNAL(clicked()), this, SLOT(close()));
  _systemTrayIcon->setContextMenu(systemTrayMenu);
  connect(_systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(systemTrayRequest(QSystemTrayIcon::ActivationReason)));
  connect(_systemTrayIcon, SIGNAL(messageClicked()), this, SLOT(systemTrayMessageClicked()));
  connect(_ui->pages, SIGNAL(currentChanged(int)), this, SLOT(pageSwitched(int)));
  connect(_ui->welcomePage, SIGNAL(openPerspective(QString)), this,
          SLOT(createPerspective(QString)));
  connect(_ui->welcomePage, SIGNAL(openProject()), this, SLOT(showOpenProjectWindow()));
  connect(_ui->welcomePage, SIGNAL(openFile(QString)), this, SLOT(openProject(QString)));
  _systemTrayIcon->show();

  connect(PerspectiveProcessHandler::instance(), SIGNAL(showPluginsAgent()), this,
          SLOT(showPluginsCenter()));
  connect(PerspectiveProcessHandler::instance(), SIGNAL(showProjectsAgent()), this,
          SLOT(showProjectsCenter()));
  connect(PerspectiveProcessHandler::instance(), SIGNAL(showAboutAgent()), this,
          SLOT(showAboutCenter()));
  connect(PerspectiveProcessHandler::instance(), SIGNAL(showTrayMessage(QString)), this,
          SLOT(showTrayMessage(QString)));
  connect(PerspectiveProcessHandler::instance(), SIGNAL(showErrorMessage(QString, QString)), this,
          SLOT(showErrorMessage(QString, QString)));
  connect(PerspectiveProcessHandler::instance(), SIGNAL(openProject(QString)), this,
          SLOT(openProject(QString)));
  connect(PerspectiveProcessHandler::instance(), SIGNAL(openProjectWith(QString, QString)), this,
          SLOT(openProjectWith(QString, QString)));
  connect(PerspectiveProcessHandler::instance(), SIGNAL(openPerspective(QString)), this,
          SLOT(createPerspective(QString)));

#ifdef TALIPOT_BUILD_PYTHON_COMPONENTS
  checkPython();
#endif
}

MainWindow::~MainWindow() {
  delete _ui;
}

#ifdef TALIPOT_BUILD_PYTHON_COMPONENTS
void MainWindow::checkPython() {
  if (!PythonVersionChecker::isPythonVersionMatching()) {

    QStringList installedPythons = PythonVersionChecker::installedVersions();

    QString requiredPython = "Python " + PythonVersionChecker::compiledVersion();
#ifdef X86_64
    requiredPython += " (64 bit)";
#else
    requiredPython += " (32 bit)";
#endif

    QString errorMessage;

    if (installedPythons.isEmpty()) {
      errorMessage = requiredPython + " does not seem to be installed on your system.\nPlease "
                                      "install it in order to use Talipot.";
    } else {
      errorMessage = "Python version mismatch. Please install " + requiredPython +
                     " in order to use Talipot.\n";

      if (installedPythons.size() == 1) {
        errorMessage += "Detected version is " + installedPythons.at(0) + ".";
      } else {
        errorMessage += "Detected versions are ";

        for (int i = 0; i < installedPythons.size(); ++i) {
          errorMessage += installedPythons.at(i);

          if (i < installedPythons.size() - 1) {
            errorMessage += ", ";
          }
        }

        errorMessage += " .";
      }
    }

    showErrorMessage("Python", errorMessage);
  }
}
#endif

void MainWindow::closeEvent(QCloseEvent *e) {
  _systemTrayIcon->deleteLater();
  e->accept();
}

void MainWindow::pageChooserClicked() {
  if (!isVisible())
    setVisible(true);

  _ui->pages->setCurrentIndex(_pageChoosers.indexOf(sender()));
}

void MainWindow::pageSwitched(int i) {
  if (i >= _pageChoosers.size() || i < 0)
    return;

  static_cast<QToolButton *>(_pageChoosers[i])->setChecked(true);
}

void MainWindow::systemTrayRequest(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::Trigger)
    setVisible(!isVisible());
}

void MainWindow::systemTrayMessageClicked() {
  if (_currentTrayMessage == PluginErrorMessage)
    _ui->pluginsPage->showErrorsPage();
  else
    _ui->welcomePageChooser->click();

  show();

  _currentTrayMessage = NoMessage;
}

PluginsCenter *MainWindow::pluginsCenter() const {
  return _ui->pluginsPage;
}

void MainWindow::createPerspective(const QString &name) {
  createPerspective(name, QVariantMap());
}
void MainWindow::showOpenProjectWindow() {
  setVisible(true);
  QString filePath = QFileDialog::getOpenFileName(
      this,
      "Choose a Talipot project to open with its associated perspective. Or select a "
      "external file format to import.",
      QDir::homePath(),
      "Talipot Files(*.tlp *.tlp.gz *.tlpb *.tlpb.gz *.tlpx *.tlpz *.tlpbz);;Talipot "
      "Project (*.tlpx);;Talipot Graph (*.tlp *.tlp.gz *.tlpb *.tlpb.gz *.tlpz "
      "*.tlpbz)");

  if (filePath.isEmpty())
    return;

  openProject(filePath);
}

void MainWindow::bringWindowToFront() {
  hide();
  show();
  raise();
  activateWindow();
}

void MainWindow::showPluginsCenter() {
  bringWindowToFront();
  _ui->pages->setCurrentWidget(_ui->pluginsPage);
}

void MainWindow::showProjectsCenter() {
  bringWindowToFront();
  _ui->pages->setCurrentWidget(_ui->welcomePage);
}

void MainWindow::showAboutCenter() {
  bringWindowToFront();
  _ui->pages->setCurrentWidget(_ui->aboutPage);
}

void MainWindow::showTrayMessage(const QString &message) {
  showTrayMessage("Perspective", "\n" + message + "\n\n" + "Click to dismiss",
                  uint(QSystemTrayIcon::Information), 3000);
}

void MainWindow::openProject(const QString &file) {

  tlp::Project *project = tlp::Project::openProject(file);

  if (project != nullptr) {
    openProjectWith(file, project->perspective(), QVariantMap());
  } else {
    QString perspectiveName;

    if (PluginsManager::availablePlugins<tlp::Perspective>().size() > 1) {
      PerspectiveSelectionDialog dlg;

      if (dlg.exec() == QDialog::Accepted) {
        perspectiveName = dlg.perspectiveName();
      }
    } else {
      std::string stdName = *(PluginsManager::availablePlugins<tlp::Perspective>().begin());
      perspectiveName = stdName.c_str();
    }

    if (!perspectiveName.isEmpty())
      PerspectiveProcessHandler::instance()->createPerspective(perspectiveName, file,
                                                               QVariantMap());
  }

  delete project;
}

void MainWindow::createPerspective(const QString &name, const QVariantMap &parameters) {
  PerspectiveProcessHandler::instance()->createPerspective(name, "", parameters);
}
void MainWindow::openProjectWith(const QString &file, const QString &perspective,
                                 const QVariantMap &parameters) {
  Settings::instance().addToRecentDocuments(file);
  PerspectiveProcessHandler::instance()->createPerspective(perspective, file, parameters);
}

void MainWindow::showTrayMessage(const QString &title, const QString &message, uint icon,
                                 uint duration) {
  if (_systemTrayIcon == nullptr)
    return;

  _systemTrayIcon->showMessage(title, message, static_cast<QSystemTrayIcon::MessageIcon>(icon),
                               duration);
}

void MainWindow::showErrorMessage(const QString &title, const QString &message) {
  _ui->statusBar->setVisible(true);
  showTrayMessage(title, message, QSystemTrayIcon::Critical, 10000);
  _errorMessage->setText(message);
}

void MainWindow::pluginErrorMessage(const QString &message) {
  _currentTrayMessage = PluginErrorMessage;
  showTrayMessage("Error while loading plugins",
                  message + "\n\nClick on this message to see detailed information",
                  uint(QSystemTrayIcon::Critical), 10000);
}
