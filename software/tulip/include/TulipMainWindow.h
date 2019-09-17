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

#ifndef TULIPMAINWINDOW_H
#define TULIPMAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QSystemTrayIcon>
#include <QVariant>

namespace Ui {
class TulipMainWindowData;
}
namespace tlp {
struct PluginInformation;
}
class PluginsCenter;
class QLabel;

/**
  @brief The window used in the tulip_app process to display the Welcome, plugins center and getting
  help screens.
  This class is basically a controller for the 3 underleying screens that manage their own logic.
  */
class TulipMainWindow : public QMainWindow {
  Q_OBJECT

  static TulipMainWindow *_instance;
  TulipMainWindow(QWidget *parent = nullptr);

  QLabel *_errorMessage;

public:
  static TulipMainWindow *instance() {
    if (!_instance)
      _instance = new TulipMainWindow;

    return _instance;
  }

  ~TulipMainWindow() override;

  PluginsCenter *pluginsCenter() const;

public slots:
  void pluginErrorMessage(const QString &message);
  void showTrayMessage(const QString &title, const QString &message, uint icon, uint duration);
  void showErrorMessage(const QString &title, const QString &message);

  void createPerspective(const QString &name);
  void createPerspective(const QString &name, const QVariantMap &parameters);

  void openProject(const QString &file);
  void openProjectWith(const QString &file, const QString &perspective,
                       const QVariantMap &parameters = QVariantMap());

  void showOpenProjectWindow();

  void showPluginsCenter();
  void showProjectsCenter();
  void showAboutCenter();
  void showTrayMessage(const QString &message);

protected:
  void closeEvent(QCloseEvent *) override;

protected slots:
  void systemTrayRequest(QSystemTrayIcon::ActivationReason);
  void systemTrayMessageClicked();
  void pageChooserClicked();
  void pageSwitched(int);

private:
  void bringWindowToFront();

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  void checkPython();
#endif

  enum SystemTrayMessage { NoMessage, PluginErrorMessage };

  Ui::TulipMainWindowData *_ui;
  QSystemTrayIcon *_systemTrayIcon;
  SystemTrayMessage _currentTrayMessage;

  QVector<QObject *> _pageChoosers;
};

#endif // TULIPMAINWINDOW_H
