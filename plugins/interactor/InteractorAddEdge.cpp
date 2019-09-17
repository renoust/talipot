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

#include <tulip/MouseInteractors.h>
#include <tulip/MouseNodeBuilder.h>
#include <tulip/MouseEdgeBuilder.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>

#include "../utils/StandardInteractorPriority.h"
#include "../utils/PluginNames.h"

using namespace tlp;

/** \brief Tulip interactor to add edges
 *
 */
class InteractorAddEdge : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorAddEdge", "Tulip Team", "01/04/2009", "Add nodes/edges Interactor",
                    "1.0", "Modification")
  /**
   * Default constructor
   */
  InteractorAddEdge(const tlp::PluginContext *)
      : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_addedge.png", "Add nodes/edges",
                                           StandardInteractorPriority::AddNodesOrEdges) {}

  /**
   * Construct chain of responsibility
   */
  void construct() override {
    setConfigurationWidgetText("<h3>Add nodes/edges</h3>To add a node: <b>Mouse left</b> click "
                               "outside any node.<br/>To add an edge: <b>Mouse left</b> click on "
                               "the source node,<br/>then <b>Mouse left</b> click on the target "
                               "node.<br/>Any <b>Mouse left</b> click outside a node before the "
                               "click on the target node will add an edge bend,<br/><b>Mouse "
                               "middle</b> click will cancel the current edge construction.");
    push_back(new MousePanNZoomNavigator);
    push_back(new MouseNodeBuilder);
    push_back(new MouseEdgeBuilder);
  }

  QCursor cursor() const override {
    return QCursor(Qt::PointingHandCursor);
  }

  bool isCompatible(const std::string &viewName) const override {
    return (viewName == NodeLinkDiagramComponent::viewName);
  }
};

PLUGIN(InteractorAddEdge)
