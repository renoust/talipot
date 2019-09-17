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

#ifndef VIEW_MAIN_WINDOW_H
#define VIEW_MAIN_WINDOW_H

#include <unordered_map>

#include <QMainWindow>

#include <tulip/Graph.h>
#include <tulip/Observable.h>
#include <tulip/View.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/Workspace.h>
#include <tulip/WorkspacePanel.h>

typedef std::set<tlp::Observable *>::iterator ObserverIterator;

class TulipViewsManager;

class ViewMainWindow : public QMainWindow {

  Q_OBJECT

public:
  ViewMainWindow();
};

class TulipViewsManager : public QObject, public tlp::Observable {

  Q_OBJECT

public:
  TulipViewsManager();

  std::vector<std::string> getTulipViews();

  std::vector<tlp::View *> getOpenedViews();

  std::vector<tlp::View *> getOpenedViewsWithName(const std::string &viewName);

  tlp::View *addView(const std::string &viewName, tlp::Graph *graph,
                     const tlp::DataSet &dataSet = tlp::DataSet(), bool show = true);

  void closeView(tlp::View *view);

  std::vector<tlp::View *> getViewsOfGraph(tlp::Graph *graph);

  void closeAllViews();

  void closeViewsRelatedToGraph(tlp::Graph *graph);

  void setViewVisible(tlp::View *view, const bool visible);

  bool areViewsVisible();

  void resizeView(tlp::View *view, int width, int height);

  void setViewPos(tlp::View *view, int x, int y);

  tlp::GraphHierarchiesModel *graphModel() const {
    return model;
  }

  tlp::Workspace *tlpWorkspace();

  void treatEvent(const tlp::Event &ev) override;

public slots:

  void viewDestroyed(QObject *view);

private:
  std::vector<tlp::View *> openedViews;
  std::unordered_map<tlp::View *, tlp::WorkspacePanel *> viewToPanel;
  std::unordered_map<tlp::View *, ViewMainWindow *> viewToWindow;
  tlp::GraphHierarchiesModel *model;
};

#endif
