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
#ifndef TULIPPERSPECTIVEPROCESSMAINWINDOW_H
#define TULIPPERSPECTIVEPROCESSMAINWINDOW_H

#include <QMainWindow>

namespace tlp {
class TulipProject;
}

/*
  This class can be used to add pre(-defined behavior to the main window before passing it to the
  perspective.
  Note that no GUI modification should be done since the Perspective must be free to do anything on
  it
  */
class TulipPerspectiveProcessMainWindow : public QMainWindow {
  Q_OBJECT
  tlp::TulipProject *_project;
  QString _title;

public:
  explicit TulipPerspectiveProcessMainWindow(QString title, QWidget *parent = nullptr);
  void setProject(tlp::TulipProject *project);

signals:

public slots:
  void projectFileChanged(const QString &projectFile = QString(""));

protected:
  void closeEvent(QCloseEvent *) override;
};

#endif // TULIPPERSPECTIVEPROCESSMAINWINDOW_H
