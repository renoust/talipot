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

#ifndef TALIPOT_VIEWS_UTILS_H
#define TALIPOT_VIEWS_UTILS_H

#include <unordered_map>

#include <QMainWindow>

#include <talipot/Graph.h>
#include <talipot/Observable.h>
#include <talipot/View.h>
#include <talipot/GraphHierarchiesModel.h>
#include <talipot/Workspace.h>
#include <talipot/WorkspacePanel.h>

typedef std::set<tlp::Observable *>::iterator ObserverIterator;

class ViewsManager;

class ViewMainWindow : public QMainWindow {

  Q_OBJECT

public:
  ViewMainWindow();
};

class ViewsManager : public QObject, public tlp::Observable {

  Q_OBJECT

public:
  ViewsManager();

  std::vector<std::string> getViews();

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

#endif // TALIPOT_VIEWS_UTILS_H
