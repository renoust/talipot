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

#include "HistogramMetricMapping.h"
#include "HistogramStatistics.h"
#include "HistogramInteractors.h"
#include "HistogramViewNavigator.h"
#include "HistoStatsConfigWidget.h"
#include "HistogramView.h"

#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/PluginNames.h"

namespace tlp {

HistogramInteractor::HistogramInteractor(const QString &iconPath, const QString &text,
                                         const unsigned int priority)
    : NodeLinkDiagramComponentInteractor(iconPath, text, priority) {}

bool HistogramInteractor::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::HistogramViewName);
}

PLUGIN(HistogramInteractorNavigation)
PLUGIN(HistogramInteractorMetricMapping)
PLUGIN(HistogramInteractorStatistics)
PLUGIN(HistogramInteractorGetInformation)

HistogramInteractorNavigation::HistogramInteractorNavigation(const PluginContext *)
    : HistogramInteractor(":/talipot/gui/icons/i_navigation.png", "Navigate in view",
                          StandardInteractorPriority::Navigation) {}

void HistogramInteractorNavigation::construct() {
  setConfigurationWidgetText(
      QString("<html><head><title></title></head><body><h3>View navigation interactor</h3>") +
      "<p>This interactor allows to navigate in the histogram view.</p>" +
      "<p>When there is more than one graph properties selected, the corresponding histograms "
      "previews are generated and displayed in a matrix form. By <b>double clicking on an "
      "histogram," +
      "this one is displayed in fullscreen </b> in a more detailed way and the others interactors "
      "become available. To go back to the histograms previews matrix, double click anywhere in "
      "the view.</p>" +
      "<p>Otherwise, this interactor offers the same functionnalities as the one in the \"Node "
      "Link Diagram view\". The commands are described below :</p>" +
      "<b>Ctrl + Mouse up/down</b> : zoom<br>" + "<b>Ctrl + Mouse left/right</b> : z rotation<br>" +
      "<b>Shift + Mouse</b> : rotation<br>" + "<b>Key up/down</b> : up/down<br>" +
      "<b>Key left/right</b> : left/right<br>" + "<b>Key page up/down</b> : zoom<br>" +
      "<b>Key insert</b> : rotate<br>" + "</body></html>");
  push_back(new HistogramViewNavigator);
  push_back(new MouseNKeysNavigator);
}

HistogramInteractorMetricMapping::HistogramInteractorMetricMapping(const PluginContext *)
    : HistogramInteractor(":/i_histo_color_mapping.png", "Metric Mapping",
                          StandardInteractorPriority::ViewInteractor1) {}

void HistogramInteractorMetricMapping::construct() {
  setConfigurationWidgetText(
      QString("<html><head><title></title></head><body>") + "<h3>Metric mapping interactor</h3>" +
      "<p>This interactor allows to perform a metric mapping on nodes colors, nodes borders "
      "colors, nodes sizes, nodes borders widths or nodes glyphs in a visual way.</p>" +
      "<p>To select the mapping type, do a right click on the scale located at the left of the "
      "histogram vertical axis and pick the one wanted in the popup menu which appears.</p>" +
      "<p>To configure the metric mapping, double click on the scale located at the left of the "
      "histogram vertical axis and use the dialog which appears.</p>" +
      "<h4>Color mapping configuration</h4>" +
      "<p>The configuration dialog for the color mapping is illustrated below.<br />" +
      "<img src=\":/ColorScaleConfigDialog.png\" width=\"280\" height=\"260\" border=\"0\" "
      "alt=\"\"><br />" +
      "The first tab of this dialog allows to manually define a color scale. To do so, start by "
      "picking the number of colors to use by using the spinbox located above the colors table. " +
      "To select the colors to use, double click on the cells of the colors table and a color "
      "picker dialog will appear. A preview of the built color scale is displayed at the right of "
      "the color table.<br />" +
      "The built color scale can be saved using the \"Save color scale\" button.<br /><br />" +
      "<img src=\":/ColorScaleConfigDialog2.png\" width=\"280\" height=\"260\" border=\"0\" "
      "alt=\"\"><br />" +
      "The second tab of the dialog allow to load a previously saved color scale and give also the "
      "possibility to load a color scale from an image file (the color scale must be defined in "
      "the vertical dimension of the image). " +
      "Previously saved color scale can be reedit by double clicking on it. <br />" +
      "Once the color scale to use has been selected or configured, press the \"Ok\" button.</p>" +
      "<h4>Size mapping configuration</h4>" +
      "<p>The configuration dialog for the size mapping is illustrated below.<br />" +
      "<img src=\":/SizeScaleConfigDialog.png\" width=\"280\" height=\"280\" border=\"0\" "
      "alt=\"\"><br />" +
      "The top part of the dialog allows to select on which size property the mapping has to be "
      "performed : <i>viewSize</i> or <i>viewBorderWidth</i>.<br />" +
      "The middle part of the dialog aims to configure the minimum and maximum size to use for the "
      "mapping.<br />" +
      "The bottom part of the dialog allows to select on which dimensions the size mapping has to "
      "be applied when it is performed on the viewSize property.<br />" +
      "Once the wanted parameters have been set, press the \"OK\" button to apply them.</p>" +
      "<h4>Glyph mapping configuration</h4>" +
      "<p>The configuration dialog for the size mapping is illustrated below.<br />" +
      "<img src=\":/GlyphScaleConfigDialog.png\" width=\"280\" height=\"280\" border=\"0\" "
      "alt=\"\"><br />" +
      "Use the spin box located at the top of the dialog to define the number of nodes glyphs to "
      "use for the mapping. <br/>" +
      "Select the glyphs to use by the help of the combo boxes contained in the cells of the "
      "table.<br />" +
      "Press the \"OK\" button to apply the settings. </p>" +
      "<h4>Metric mapping instructions</h4>" +
      "<p>The metric mapping is done with the help of the editable curve drawn on top of the "
      "histogram. By double clicking on it, control points are created which allow to modify the "
      "curve shape (by drag and drop them) and so the metric mapping. The created control points "
      "can also be removed by double clicking on them. </p>" +
      "<p>The mapping performed can be visually interpreted as followed. For each node of the "
      "graph, get the value of the metric property associated with the current displayed "
      "histogram. " +
      "Take the line perpendicular to the horizontal axis of the histogram (the metric axis) which "
      "passes by the point on the metric axis associated with the node metric value. " +
      "Then take the intersection point between this line and the curve controlling the mapping. "
      "The value of the metric is then mapped on the node visual property associated to the y "
      "coordinates of this intersection point according to the scale located at the left of the "
      "histogram vertical axis (which can be a color, a size or a glyph scale). <br />" +
      "The corresponding mapping on the whole graph metric is materialized by the scale located "
      "under the histogram horizontal axis. " +
      "For example, if the curve is a straight line between the bottom left corner and the top "
      "right corner of the histogram, a linear mapping is performed on the metric. " +
      "More complex mapping can be performed like the color mapping illustrated below.<br />" +
      "<img src=\":/HistoColorMapping.png\" width=\"280\" height=\"260\" border=\"0\" alt=\"\"><br "
      "/>" +
      "</p>" + "</body></html>");
  push_back(new HistogramMetricMapping);
  push_back(new MousePanNZoomNavigator);
}

HistogramInteractorStatistics::HistogramInteractorStatistics(const PluginContext *)
    : HistogramInteractor(":/i_histo_statistics.png", "Statistics",
                          StandardInteractorPriority::ViewInteractor2),
      histoStatsConfigWidget(nullptr), histoStatistics(nullptr) {}

HistogramInteractorStatistics::~HistogramInteractorStatistics() {
  delete histoStatsConfigWidget;
}

void HistogramInteractorStatistics::construct() {
  histoStatsConfigWidget = new HistoStatsConfigWidget();
  histoStatistics = new HistogramStatistics(histoStatsConfigWidget);
  push_back(histoStatistics);
  push_back(new MousePanNZoomNavigator);
}

QWidget *HistogramInteractorStatistics::configurationWidget() const {
  return histoStatsConfigWidget;
}

void HistogramInteractorStatistics::install(QObject *target) {
  if (target != nullptr) {
    histoStatistics->computeInteractor();
  }
}

/**
 * We define a specific interactor to show element graph info
 */
class HistogramMouseShowElementInfo : public MouseShowElementInfo {
  HistogramView *hView;

public:
  HistogramMouseShowElementInfo() : MouseShowElementInfo(), hView(nullptr) {}
  ~HistogramMouseShowElementInfo() override {}

  void viewChanged(View *v) override {
    hView = static_cast<HistogramView *>(v);
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
    if (hView->getDataLocation() == EDGE) {
      elementId = hView->getMappedId(elementId);
      return new GraphEdgeElementModel(hView->graph(), elementId, parent);
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
    if (hView->getDataLocation() == EDGE) {
      elementId = hView->getMappedId(elementId);
      return QString("Edge") + " #" + QString::number(elementId);
    }

    return MouseShowElementInfo::elementName(elementType, elementId);
  }
};

HistogramInteractorGetInformation::HistogramInteractorGetInformation(const tlp::PluginContext *)
    : NodeLinkDiagramComponentInteractor(":/talipot/gui/icons/i_select.png",
                                         "Display node or edge properties",
                                         StandardInteractorPriority::GetInformation) {}

void HistogramInteractorGetInformation::construct() {
  setConfigurationWidgetText(QString("<h3>Display node or edge properties</h3>") +
                             "<b>Mouse left click</b> on an element to display its "
                             "properties.<br/>then <b>Mouse left click</b> on a row to edit the "
                             "corresponding value.");
  push_back(new MousePanNZoomNavigator);
  push_back(new HistogramMouseShowElementInfo);
}

bool HistogramInteractorGetInformation::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::HistogramViewName);
}
}
