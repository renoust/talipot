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

#ifndef GRAPH_PERSPECTIVE_H
#define GRAPH_PERSPECTIVE_H

#include <QModelIndex>
#include <QPoint>
#include <talipot/Perspective.h>
#include <talipot/Observable.h>

class QAction;
class QHeaderView;
class QDialog;

class GraphHierarchiesEditor;
class GraphPerspectiveLogger;

namespace tlp {
class GraphHierarchiesModel;
class View;
class BooleanProperty;
class ColorScaleConfigDialog;
}

namespace Ui {
class GraphPerspectiveMainWindowData;
}

class PythonPanel;
namespace tlp {
class PythonIDE;
}

class GraphPerspective : public tlp::Perspective, tlp::Observable {
  Q_OBJECT
  Ui::GraphPerspectiveMainWindowData *_ui;
  tlp::GraphHierarchiesModel *_graphs;
  tlp::ColorScaleConfigDialog *_colorScalesDialog;

  void reserveDefaultProperties();
  QString _lastOpenLocation;
  QString _recentDocumentsSettingsKey;

  void buildRecentDocumentsMenu();
  void addRecentDocument(const QString &path);

  void showStartPanels(tlp::Graph *);
  void applyRandomLayout(tlp::Graph *);

public:
  GraphPerspectiveLogger *_logger;

  PLUGININFORMATION("Talipot", "", "2011/07/11",
                    "Analyze several graphs/subgraphs hierarchies\n(designed to import/explore "
                    "data from various graph or csv file formats)",
                    "1.0", "")
  std::string icon() const override {
    return ":/talipot/graphperspective/icons/32/desktop.png";
  }

  GraphPerspective(const tlp::PluginContext *c);
  ~GraphPerspective() override;
  void start(tlp::PluginProgress *) override;
  tlp::GraphHierarchiesModel *model() const;
  void copy(tlp::Graph *, bool deleteAfter = false);
  tlp::Graph *createSubGraph(tlp::Graph *);

  void treatEvent(const tlp::Event &) override;

  void log(QtMsgType, const QMessageLogContext &, const QString &);

  bool terminated() override;

public slots:
  void importGraph();
  void exportGraph(tlp::Graph *g = nullptr);
  void saveGraphHierarchyInTlpFile(tlp::Graph *g = nullptr);
  void createPanel(tlp::Graph *g = nullptr);
  bool save();
  bool saveAs(const QString &path = "");
  void open(QString fileName = "");
  void openProjectFile(const QString &path) override;

  void showLogger();
  void showAPIDocumentation();
  void showUserDocumentation();
  void showDevelDocumentation();
  void showPythonDocumentation();

  void redrawPanels(bool center = false) override;
  void centerPanelsForGraph(tlp::Graph *, bool graphChanged, bool onlyGlMainView);
  void centerPanelsForGraph(tlp::Graph *g) override {
    centerPanelsForGraph(g, false, false);
  }
  void closePanelsForGraph(tlp::Graph *g = nullptr);
  bool
  setGlMainViewPropertiesForGraph(tlp::Graph *g,
                                  const std::map<std::string, tlp::PropertyInterface *> &propsMap);
  void setSearchOutput(bool);
  void setPythonPanel(bool);
  void openPreferences();

  void setAutoCenterPanelsOnDraw(bool f);

  void pluginsListChanged();

  void showPythonIDE();

  void displayColorScalesDialog();

  void showAboutPage();

protected slots:
  void currentGraphChanged(tlp::Graph *graph);
  void refreshDockExpandControls();
  void panelFocused(tlp::View *);
  void focusedPanelGraphSet(tlp::Graph *);
  void focusedPanelSynchronized();
  void clearGraph();
  void deleteSelectedElements(bool fromRoot = false);
  void deleteSelectedElementsFromRootGraph();
  void invertSelection();
  void reverseSelectedEdges();
  void cancelSelection();
  void make_graph();
  void selectAll(bool nodes = true, bool edges = true);
  void selectAllNodes();
  void selectAllEdges();
  void undo();
  void redo();
  void cut();
  void paste();
  void copy();
  void group();
  void createSubGraph();
  void cloneSubGraph();
  void addEmptySubGraph();
  void CSVImport();
  void logCleared();
  void findPlugins();
  void addNewGraph();
  void newProject();
  void openRecentFile();
  void changeSynchronization(bool);
  void openExternalFile();
  void showHideSideBar();
  void workspaceButtonClicked();
  void resetLoggerDialogPosition();
  void showHideLogger();
  void showHideMenuBar();
  void initPythonIDE();
  void anchoredPythonIDE(bool anchored);

protected:
  bool eventFilter(QObject *, QEvent *) override;
  void importGraph(const std::string &module, tlp::DataSet &data);
  void updateLogIconsAndCounters();
  void destroyWorkspace();

  PythonPanel *_pythonPanel;
  tlp::PythonIDE *_pythonIDE;
  QDialog *_pythonIDEDialog;
};

#endif // GRAPH_PERSPECTIVE_H
