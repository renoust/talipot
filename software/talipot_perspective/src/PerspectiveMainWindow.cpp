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

#include "PerspectiveMainWindow.h"

#include <QCloseEvent>
#include <QShortcut>
#include <QAction>
#include <talipot/Perspective.h>
#include <talipot/Project.h>

PerspectiveProcessMainWindow::PerspectiveProcessMainWindow(QString title, QWidget *parent)
    : QMainWindow(parent), _project(nullptr), _title(title) {}

void PerspectiveProcessMainWindow::closeEvent(QCloseEvent *event) {
  if (tlp::Perspective::instance()->terminated()) {
    QMainWindow::closeEvent(event);
  } else
    event->ignore();
}

void PerspectiveProcessMainWindow::setProject(tlp::Project *project) {
  _project = project;
  connect(project, SIGNAL(projectFileChanged(const QString &)), this,
          SLOT(projectFileChanged(const QString &)));
  connect(tlp::Perspective::instance(), SIGNAL(resetWindowTitle()), this,
          SLOT(projectFileChanged()));
}

void PerspectiveProcessMainWindow::projectFileChanged(const QString &projectFile) {
  QString wTitle(_title);

  wTitle += QString(" [") + _project->perspective() + "]";

  if (!_project->name().isEmpty())
    wTitle += QString(" - ") + _project->name();
  else if (!projectFile.isEmpty())
    wTitle += QString(" - ") + projectFile;
  else { // all graphs are deleted. Clear project. Useful?
    _project->clearProject();
    wTitle += QString(" - unsaved project");
  }

  wTitle += "[*]"; // placeholder for window modification
#ifndef NDEBUG
  wTitle += " - [ Debug mode ]";
#endif
  setWindowTitle(wTitle);
}
