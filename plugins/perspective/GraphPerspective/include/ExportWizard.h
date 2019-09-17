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
#ifndef EXPORTWIZARD_H
#define EXPORTWIZARD_H

#include <QWizard>
#include <QModelIndex>
#include <tulip/WithParameter.h>

namespace Ui {
class ExportWizard;
}

namespace tlp {
class GraphHierarchiesModel;
}

class ExportWizard : public QWizard {
  Q_OBJECT

  Ui::ExportWizard *_ui;
  tlp::Graph *_graph;

public:
  explicit ExportWizard(tlp::Graph *g, const QString &exportFile, QWidget *parent = nullptr);
  ~ExportWizard() override;

  QString algorithm() const;
  tlp::DataSet parameters() const;
  QString outputFile() const;

protected slots:
  void algorithmSelected(const QModelIndex &index);
  void updateFinishButton();

  void pathChanged(QString s);
  void browseButtonClicked();
  bool validateCurrentPage() override;
};

#endif // EXPORTWIZARD_H
