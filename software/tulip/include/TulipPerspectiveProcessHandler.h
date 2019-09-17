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
#ifndef TULIPPERSPECTIVEPROCESSHANDLER_H
#define TULIPPERSPECTIVEPROCESSHANDLER_H

#include <QObject>
#include <QVariantMap>
#include <QProcess>

#include <QPushButton>

#include <QTcpServer>

#include <ctime>

// Some utility class used in crash handler dialog designer form

class SelectionButton : public QPushButton {
public:
  explicit SelectionButton(QWidget *parent = nullptr);
  void paintEvent(QPaintEvent *e) override;
};

struct PerspectiveProcessInfo {
  PerspectiveProcessInfo() {}

  PerspectiveProcessInfo(const QString &name, const QVariantMap &args, const QString &file,
                         time_t id)
      : name(name), args(args), file(file), _perspectiveId(id) {}

  QString name;
  QVariantMap args;
  QString file;
  QString projectPath;
  time_t _perspectiveId;
};

class TulipPerspectiveProcessHandler : public QTcpServer {
  Q_OBJECT

  QMap<QProcess *, PerspectiveProcessInfo> _processInfo;

  static TulipPerspectiveProcessHandler *_instance;
  TulipPerspectiveProcessHandler();

  QThread *_serverThread;

  QProcess *fromId(unsigned int);

public:
  static TulipPerspectiveProcessHandler *instance();

public slots:
  void createPerspective(const QString &perspective, const QString &file,
                         const QVariantMap &parameters);

protected slots:
  void perspectiveCrashed(QProcess::ProcessError);
  void perspectiveFinished(int exitCode, QProcess::ExitStatus exitStatus);

  void acceptConnection();
  void perspectiveReadyRead();

signals:
  void showPluginsAgent();
  void showProjectsAgent();
  void showAboutAgent();
  void showTrayMessage(QString);
  void showErrorMessage(QString, QString);
  void openProject(QString);
  void openProjectWith(QString, QString);
  void openPerspective(QString);
};

#endif // TULIPPERSPECTIVEPROCESSHANDLER_H
