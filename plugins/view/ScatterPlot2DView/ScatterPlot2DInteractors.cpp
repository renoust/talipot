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

#include <talipot/MouseInteractors.h>
#include <talipot/MouseShowElementInfo.h>
#include <talipot/GraphElementModel.h>

#include "ScatterPlot2DInteractors.h"
#include "ScatterPlot2DViewNavigator.h"
#include "ScatterPlotTrendLine.h"
#include "ScatterPlotCorrelCoeffSelector.h"
#include "ScatterPlot2DView.h"
#include "ScatterPlot2D.h"
#include "ScatterPlotCorrelCoeffSelectorOptionsWidget.h"

#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/PluginNames.h"

using namespace std;

namespace tlp {

ScatterPlot2DInteractor::ScatterPlot2DInteractor(const QString &iconPath, const QString &text,
                                                 const unsigned int priority)
    : NodeLinkDiagramComponentInteractor(iconPath, text, priority) {}

bool ScatterPlot2DInteractor::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::ScatterPlot2DViewName);
}

PLUGIN(ScatterPlot2DInteractorNavigation)
PLUGIN(ScatterPlot2DInteractorTrendLine)
PLUGIN(ScatterPlot2DInteractorCorrelCoeffSelector)
PLUGIN(ScatterPlot2DInteractorGetInformation)

ScatterPlot2DInteractorNavigation::ScatterPlot2DInteractorNavigation(const tlp::PluginContext *)
    : ScatterPlot2DInteractor(":/talipot/gui/icons/i_navigation.png", "Navigate in view",
                              StandardInteractorPriority::Navigation) {}

void ScatterPlot2DInteractorNavigation::construct() {
  setConfigurationWidgetText(
      QString("<html><head>") + "<title></title>" + "</head>" + "<body>" +
      "<h3>View navigation interactor</h3>" +
      "<p>This interactor allows to navigate in the scatter plot view.</p>" +
      "<p>When there is more than one graph properties selected, the corresponding scatter plots "
      "previews are generated and displayed in a matrix form. By <b>double clicking on a scatter "
      "plot, " +
      "this one is displayed in fullscreen </b> in a more detailed way and the others interactors "
      "become available. To go back to the scatter plots matrix, double click anywhere in the "
      "view.</p>" +
      "<p>Otherwise, this interactor offers the same functionnalities as the one in the \"Node "
      "Link Diagram view\". The commands are described below :</p>" +
      "<b>Ctrl + Mouse up/down</b> : zoom<br>" + "<b>Ctrl + Mouse left/right</b> : z rotation<br>" +
      "<b>Shift + Mouse</b> : rotation<br>" + "<b>Key up/down</b> : up/down<br>" +
      "<b>Key left/right</b> : left/right<br>" + "<b>Key page up/down</b> : zoom<br>" +
      "<b>Key insert</b> : rotate<br>" + "</body>" + "</html>");
  push_back(new ScatterPlot2DViewNavigator);
  push_back(new MouseNKeysNavigator);
}

ScatterPlot2DInteractorTrendLine::ScatterPlot2DInteractorTrendLine(const PluginContext *)
    : ScatterPlot2DInteractor(":/i_scatter_trendline.png", "Trend line",
                              StandardInteractorPriority::ViewInteractor1) {}

void ScatterPlot2DInteractorTrendLine::construct() {
  push_back(new ScatterPlotTrendLine);
  push_back(new MousePanNZoomNavigator);
}

ScatterPlot2DInteractorCorrelCoeffSelector::ScatterPlot2DInteractorCorrelCoeffSelector(
    const tlp::PluginContext *)
    : ScatterPlot2DInteractor(":/talipot/gui/icons/i_magic.png", "Correlation Coefficient Selector",
                              StandardInteractorPriority::ViewInteractor2),
      optionsWidget(nullptr) {}

void ScatterPlot2DInteractorCorrelCoeffSelector::construct() {
  optionsWidget = new ScatterPlotCorrelCoeffSelectorOptionsWidget();
  push_back(new ScatterPlotCorrelCoeffSelector(optionsWidget));
  push_back(new MousePanNZoomNavigator);
}

ScatterPlot2DInteractorCorrelCoeffSelector::~ScatterPlot2DInteractorCorrelCoeffSelector() {
  delete optionsWidget;
}

QWidget *ScatterPlot2DInteractorCorrelCoeffSelector::configurationWidget() const {
  return optionsWidget;
}

/**
 * We define a specific interactor to show element graph info
 */
class ScatterPlot2DMouseShowElementInfo : public MouseShowElementInfo {
  ScatterPlot2DView *scp2DView;

public:
  ScatterPlot2DMouseShowElementInfo() : MouseShowElementInfo(), scp2DView(nullptr) {}
  ~ScatterPlot2DMouseShowElementInfo() override {}

  void viewChanged(View *v) override {
    scp2DView = static_cast<ScatterPlot2DView *>(v);
    MouseShowElementInfo::viewChanged(v);
  }

protected:
  /**
   * @brief buildModel create and returns the model to visualize edit elements parameters.
   * @param elementType the type of the element can be NODE or EDGE
   * @param elementId elementId the id of the element
   * @param parent the parent for the model creation.
   * @return
   */
  QAbstractItemModel *buildModel(ElementType elementType, unsigned int elementId,
                                 QObject *parent) const override {
    if (scp2DView->getDataLocation() == EDGE) {
      elementId = scp2DView->getMappedId(elementId);
      return new GraphEdgeElementModel(scp2DView->graph(), elementId, parent);
    }

    return MouseShowElementInfo::buildModel(elementType, elementId, parent);
  }

  /**
   * @brief elementName returns the title of the element.
   * @param elementType the type of the element can be NODE or EDGE
   * @param elementId the id of the element
   * @return
   */
  QString elementName(ElementType elementType, unsigned int elementId) const override {
    if (scp2DView->getDataLocation() == EDGE) {
      elementId = scp2DView->getMappedId(elementId);
      return QString("Edge") + " #" + QString::number(elementId);
    }

    return MouseShowElementInfo::elementName(elementType, elementId);
  }
};

ScatterPlot2DInteractorGetInformation::ScatterPlot2DInteractorGetInformation(
    const tlp::PluginContext *)
    : NodeLinkDiagramComponentInteractor(":/talipot/gui/icons/i_select.png",
                                         "Display node or edge properties",
                                         StandardInteractorPriority::GetInformation) {}

void ScatterPlot2DInteractorGetInformation::construct() {
  setConfigurationWidgetText(QString("<h3>Display node or edge properties</h3>") +
                             "<b>Mouse left click</b> on an element to display its "
                             "properties.<br/>then <b>Mouse left click</b> on a row to edit the "
                             "corresponding value.");
  push_back(new MousePanNZoomNavigator);
  push_back(new ScatterPlot2DMouseShowElementInfo);
}

bool ScatterPlot2DInteractorGetInformation::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::ScatterPlot2DViewName);
}
}
