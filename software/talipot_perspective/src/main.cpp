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

#include <QString>
#include <QDir>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QStandardPaths>

#include <CrashHandling.h>

#include <talipot/Exception.h>
#include <talipot/Release.h>
#include <talipot/PluginLibraryLoader.h>
#include <talipot/PluginLister.h>
#include <talipot/TlpTools.h>
#include <talipot/TlpQtTools.h>
#include <talipot/Project.h>
#include <talipot/SimplePluginProgressWidget.h>
#include <talipot/PluginLister.h>
#include <talipot/Perspective.h>
#include <talipot/Interactor.h>
#include <talipot/GlyphManager.h>
#include <talipot/EdgeExtremityGlyphManager.h>
#include <talipot/QGlBufferManager.h>
#include <talipot/TlpQtTools.h>
#include <talipot/Settings.h>
#include <talipot/WorkspacePanel.h>
#include <talipot/View.h>
#include <talipot/GlMainView.h>
#include <talipot/GlMainWidget.h>

#include "PerspectiveMainWindow.h"

#include <iostream>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef interface
#undef interface
#endif

using namespace std;
using namespace tlp;

struct PluginLoaderToProgress : public PluginLoader {
  PluginProgress *_progress;
  bool _debug_output;
  int max_step;
  int step;

  PluginLoaderToProgress(PluginProgress *progress = nullptr, bool debug_output = false)
      : _progress(progress), _debug_output(debug_output), max_step(0), step(0) {}

  void start(const std::string &path) override {
    step = 0;
    _progress->setComment("Entering " + path);
    if (_debug_output)
      tlp::debug() << "Entering " << path << std::endl;
  }

  void finished(bool state, const std::string &msg) override {
    if (state) {
      _progress->setComment("Plugins successfully loaded");
      if (_debug_output)
        tlp::debug() << "Plugins successfully loaded" << std::endl;
    } else {
      _progress->setComment(msg);
      if (_debug_output)
        tlp::debug() << msg << std::endl;
    }
  }

  void numberOfFiles(int n) override {
    max_step = n;
  }

  void loading(const std::string &filename) override {
    step++;
    _progress->progress(step, max_step);
    _progress->setComment("Loading " + filename);
    if (_debug_output)
      tlp::debug() << "Loading " << filename << std::endl;
  }

  void loaded(const tlp::Plugin *plugin, const std::list<tlp::Dependency> &) override {
    if (_debug_output)
      tlp::debug() << "  - Plugin '" << plugin->name() << "' registered" << std::endl;
  }

  void aborted(const std::string &fileName, const std::string &errorMsg) override {
    tlp::error() << "[Warning] Failed to load " << fileName << ": " << errorMsg << std::endl;
  }
};

void usage(const QString &error) {
  int returnCode = 0;

  if (!error.isEmpty()) {
    QMessageBox::warning(nullptr, "Error", error);
    returnCode = 1;
  }

  cout
      << "Usage: talipot_perspective [OPTION] [FILE]" << endl
      << "Run a Talipot Perspective plugin into its dedicated process." << endl
      << "If Talipot main process is already running, embedded perspective will run into managed "
         "mode."
      << endl
      << endl
      << "FILE: a Talipot project file to open. The perspective to use will be read from the "
         "project "
         "meta-information. If the \"--perspective\" flag is used, talipot_perspective will try to "
         "open the file with the given perspective (the project meta-information is ignored)."
      << endl
      << "List of OPTIONS:" << endl
      << "  --perspective=<perspective_name> (-p perspective_name)\tStart the perspective "
         "specified by perspective_name."
      << endl
      << "  --geometry=<X,Y,width,height>\tSet the given rectangle as geometry for the main window."
      << endl
      << "  --title=<title>\tDisplay a specific name in the loading dialog." << endl
      << "  --icon=<relative path>\tChoose the icon in the loading dialog by providing a path "
         "relative to Talipot bitmap directory."
      << endl
      << "  --help (-h)\tDisplay this help message and ignore other options." << endl
      << endl
      << "Other options (written as --<option_name>=<value>) will be passed to the perspective."
      << endl
      << "Available perspectives:" << endl;
  tlp::initTalipotSoftware();
  list<string> perspectives = PluginLister::availablePlugins<Perspective>();

  for (auto &name : perspectives) {
    cout << " - " << name << endl;
    string usage_str("");
    auto p = PluginLister::getPluginObject<Perspective>(name);
#if TALIPOT_INT_MM_VERSION > 503
#error "The check talipotRelease() != 5.3.0 in non longer needed"
#endif
    if (p->talipotRelease() != "5.3.0")
      p->usage(usage_str);
    else
      usage_str = "5.3.0 perspective !!!";
    delete p;
    cout << "   " << usage_str << endl;
    cout << endl;
  }

  exit(returnCode);
}

int main(int argc, char **argv) {

  CrashHandling::installCrashHandler();

  QString title("Talipot ");
  QString iconPath;

  // show patch number only if needed
  if (TALIPOT_INT_VERSION % 10)
    title += TALIPOT_VERSION;
  else
    title += TALIPOT_MM_VERSION;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
#endif
  QApplication talipot_perspective(argc, argv);
  // the applicationName below is used to identify the location
  // of downloaded plugins, so it must be the same as in
  // talipot/main.cpp
  talipot_perspective.setApplicationName(title);

  // Check arguments
  QString perspectiveName, projectFilePath;
  QVariantMap extraParams;
  QRect windowGeometry;
  PerspectiveContext *context = new PerspectiveContext();

  QRegExp perspectiveRegexp("^\\-\\-perspective=(.*)");
  QRegExp pRegexp("^\\-p");
  QRegExp titleRegexp("^\\-\\-title=(.*)");
  QRegExp iconRegexp("^\\-\\-icon=(.*)");
  QRegExp portRegexp("^\\-\\-port=([0-9]*)");
  QRegExp idRegexp("^\\-\\-id=([0-9]*)");
  QRegExp geometryRegexp("^\\-\\-geometry=([0-9]*)\\,([0-9]*)\\,([0-9]*)\\,([0-9]*)");
  QRegExp debugPluginLoadRegExp("^\\-debug_plugin_load");
  bool debugPluginLoad = false;
  QRegExp extraParametersRegexp("^\\-\\-([^=]*)=(.*)");

  QStringList args = QApplication::arguments();

  for (int i = 1; i < args.size(); ++i) {
    QString a = args[i];

    if ((a == "--help") || (a == "-h")) {
      usage("");
    } else if (perspectiveRegexp.exactMatch(a)) {
      perspectiveName = perspectiveRegexp.cap(1);
    } else if (pRegexp.exactMatch(a)) {
      perspectiveName = args[++i];
    } else if (titleRegexp.exactMatch(a)) {
      title = titleRegexp.cap(1);
    } else if (iconRegexp.exactMatch(a)) {
      iconPath = iconRegexp.cap(1);
    } else if (geometryRegexp.exactMatch(a)) {
      windowGeometry = QRect(geometryRegexp.cap(1).toInt(), geometryRegexp.cap(2).toInt(),
                             geometryRegexp.cap(3).toInt(), geometryRegexp.cap(4).toInt());
    } else if (portRegexp.exactMatch(a)) {
      context->talipotPort = portRegexp.cap(1).toUInt();
    } else if (debugPluginLoadRegExp.exactMatch(a))
      debugPluginLoad = true;
    else if (idRegexp.exactMatch(a)) {
      context->id = idRegexp.cap(1).toUInt();
      QString dumpPath = QDir(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0))
                             .filePath("talipot_perspective-" + idRegexp.cap(1) + ".log");
      CrashHandling::setDumpPath(QStringToTlpString(dumpPath));
    } else if (extraParametersRegexp.exactMatch(a)) {
      extraParams[extraParametersRegexp.cap(1)] = extraParametersRegexp.cap(2);
    } else {
      projectFilePath = a;
    }
  }

  // Create perspective's window
  PerspectiveProcessMainWindow *mainWindow = new PerspectiveProcessMainWindow(title);
  mainWindow->setVisible(false);

  // Progress bar dialog
  SimplePluginProgressDialog *progress = new SimplePluginProgressDialog();
  progress->setStopButtonVisible(false);
  progress->setCancelButtonVisible(false);
  progress->showPreview(false);

  progress->resize(500, std::min(50, progress->height()));
  progress->setComment(QString("Initializing ") + title);
  progress->setWindowTitle(title);
  progress->progress(0, 100);

  initTalipotLib(QStringToTlpString(QApplication::applicationDirPath()).c_str());

#ifdef _MSC_VER
  // Add path to Talipot pdb files generated by Visual Studio (for configurations Debug and
  // RelWithDebInfo)
  // It allows to get a detailed stack trace when Talipot crashes.
  CrashHandling::setExtraSymbolsSearchPaths(tlp::TalipotShareDir + "pdb");
#endif

  QIcon icon = progress->windowIcon();

  if (!iconPath.isEmpty()) {
    QString iconFullPath = tlpStringToQString(TalipotBitmapDir) + iconPath;
    QIcon tmp(iconFullPath);

    if (tmp.pixmap(QSize(16, 16)).isNull() == false)
      icon = tmp;
    else
      usage("Could not load icon: " + iconFullPath);
  }

  progress->setWindowIcon(icon);
  progress->show();

  Project *project = nullptr;

  // Init talipot
  try {
    PluginLoaderToProgress loader(progress, debugPluginLoad);
    tlp::initTalipotSoftware(&loader);
  } catch (tlp::Exception &e) {
    QMessageBox::warning(nullptr, "Error", e.what());
    exit(1);
  }

  QFileInfo fileInfo(projectFilePath);

  if (!projectFilePath.isEmpty() && (!fileInfo.exists() || fileInfo.isDir())) {
    usage("File " + projectFilePath + " not found or is a directory");
  }

  if (!projectFilePath.isEmpty() && projectFilePath.endsWith(".tlpx")) {
    project = Project::openProject(projectFilePath, progress);
  }

  if (project == nullptr) {
    context->externalFile = projectFilePath;
    project = Project::newProject();
  } else if (perspectiveName.isEmpty()) {
    perspectiveName = project->perspective();
  }

  if (perspectiveName.isEmpty()) {
    // set Talipot as default perspective
    perspectiveName = "Talipot";
  }

  context->project = project;
  context->parameters = extraParams;
  project->setPerspective(perspectiveName);

  // Initialize main window.
  progress->progress(100, 100);
  progress->setComment("Setting up GUI (this can take some time)");
  context->mainWindow = mainWindow;

  // Create perspective object
  Perspective *perspective =
      PluginLister::getPluginObject<Perspective>(tlp::QStringToTlpString(perspectiveName), context);

  if (perspective == nullptr) {
    usage("Cannot open perspective: " + perspectiveName +
          "\nWrong plugin type or plugin not found.");
  }

  Perspective::setInstance(perspective);
  mainWindow->setProject(project);

  perspective->start(progress);

  mainWindow->projectFileChanged(projectFilePath);

  delete progress;
  delete context;

  mainWindow->setWindowIcon(icon);
  mainWindow->show();

  // the delay of geometry update until perspective execution
  // seems to ensure that the four parameters (x,y,w,h)
  // are taken into account
  if (windowGeometry.isValid()) {
    mainWindow->setGeometry(windowGeometry);
  } else {
    mainWindow->move(0, 0);
    mainWindow->resize(QDesktopWidget().availableGeometry(mainWindow).size() * 0.9);
  }

  Settings::instance().setFirstRun(false);
  Settings::instance().setFirstTalipotMMRun(false);
  int result = talipot_perspective.exec();
  delete perspective;
  delete mainWindow;

  // We need to clear allocated Qt buffers and QGlWidget to remove a segfault when we close talipot
  QGlBufferManager::clearBuffers();
  GlMainWidget::clearFirstQGLWidget();

  return result;
}
