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

#ifndef NEIGHBOURHOODHIGHLIGHTERCONFIGWIDGET_H_
#define NEIGHBOURHOODHIGHLIGHTERCONFIGWIDGET_H_

#include "NodeNeighborhoodView.h"

#include <QWidget>

namespace Ui {
class NeighborhoodHighlighterConfigWidgetData;
}

namespace tlp {

class NeighborhoodHighlighterConfigWidget : public QWidget {

  Q_OBJECT

  Ui::NeighborhoodHighlighterConfigWidgetData *_ui;

public:
  NeighborhoodHighlighterConfigWidget(QWidget *parent = nullptr);
  ~NeighborhoodHighlighterConfigWidget() override;

  NodeNeighborhoodView::NeighborNodesType getNeighborsType() const;
  bool computeReachableSubGraph() const;
  void setCurrentMaxDistanceForReachableNodes(unsigned int distance);
  bool bringAndGoAnimation1() const;
  std::string propertyToUse() const;
  int numberOfNodesToBring() const;
  void setPropertyToUse(const std::string &propertyName);
  void setNumberOfNodes(int nodesNb);
  bool isdisplayEdgesCBChecked() const;

signals:
  void updateNeighborhoodGraph();
};
} // namespace tlp
#endif /* NEIGHBOURHOODHIGHLIGHTERCONFIGWIDGET_H_ */
