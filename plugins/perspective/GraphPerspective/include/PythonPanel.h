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

#ifndef PYTHONPANEL_H
#define PYTHONPANEL_H

#include <QWidget>

namespace Ui {
class PythonPanel;
}

namespace tlp {
class GraphHierarchiesModel;
}

class PythonPanel : public QWidget {

  Q_OBJECT
  Ui::PythonPanel *_ui;

public:
  explicit PythonPanel(QWidget *parent = nullptr);
  ~PythonPanel() override;
  void setModel(tlp::GraphHierarchiesModel *model);

protected:
  void dragEnterEvent(QDragEnterEvent *) override;
  void dropEvent(QDropEvent *) override;

private slots:

  void graphComboIndexChanged();
  void beginCurrentLinesExecution();
  void endCurrentLinesExecution();
};

#endif // PYTHONPANEL_H
