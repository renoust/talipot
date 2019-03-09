/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <iostream>
#include <cassert>
#include <vector>

#include <libvpsc/solve_VPSC.h>
#include <libvpsc/variable.h>
#include <libvpsc/constraint.h>
#include <libvpsc/rectangle.h>

#include <tulip/SizeProperty.h>
#include <tulip/StringCollection.h>
#include <tulip/GraphParallelTools.h>

#include "FastOverlapRemoval.h"

using namespace std;
using namespace tlp;

PLUGIN(FastOverlapRemoval)

static const char *paramHelp[] = {

    // layout
    "The property used for the input layout of nodes and edges.",

    // node size
    "The property used for node sizes.",

    // Rotation
    "The property defining rotation angles of nodes around the z-axis.",

    // Iterations
    "The algorithm will be applied N times, each time increasing node size to attain original size "
    "at the final iteration. This greatly enhances the layout.",

    // x border
    "The minimal x border value that will separate the graph nodes after application of the "
    "algorithm.",

    // y border
    "The minimal y border value that will separate the graph nodes after application of the "
    "algorithm."
};

FastOverlapRemoval::FastOverlapRemoval(const tlp::PluginContext *context)
    : tlp::LayoutAlgorithm(context) {
  addInParameter<LayoutProperty>("layout", paramHelp[0], "viewLayout");
  addInParameter<SizeProperty>("bounding box", paramHelp[1], "viewSize");
  addInParameter<DoubleProperty>("rotation", paramHelp[2], "viewRotation");
  addInParameter<int>("number of passes", paramHelp[3], "5");
  addInParameter<double>("x border", paramHelp[4], "0.0");
  addInParameter<double>("y border", paramHelp[5], "0.0");
}

/**
 * The following function transfers the node set into vpsc rectangles
 * and runs fast overlap removal.  This vpsc code was a port of Dwyer
 * used in the InkScape Open Source Software.
 */
bool FastOverlapRemoval::run() {
  LayoutProperty *viewLayout = nullptr;
  SizeProperty *viewSize = nullptr;
  DoubleProperty *viewRot = nullptr;
  double xBorder = 0.;
  double yBorder = 0.;
  int nbPasses = 5;

  if (dataSet != nullptr) {

    dataSet->get("layout", viewLayout);

    if (!dataSet->get("bounding box", viewSize))
      // old name of the parameter
      dataSet->get("boundingBox", viewSize);

    dataSet->get("rotation", viewRot);
    dataSet->get("number of passes", nbPasses);
    dataSet->get("x border", xBorder);
    dataSet->get("y border", yBorder);
  }

  if (viewLayout == nullptr)
    viewLayout = graph->getProperty<LayoutProperty>("viewLayout");

  if (viewSize == nullptr)
    viewSize = graph->getProperty<SizeProperty>("viewSize");

  if (viewRot == nullptr)
    viewRot = graph->getProperty<DoubleProperty>("viewRotation");

  // initialize result for edges
  result->setAllEdgeValue(viewLayout->getEdgeDefaultValue());
  for (auto e : viewLayout->getNonDefaultValuatedEdges())
    result->setEdgeValue(e, viewLayout->getEdgeValue(e));

  size_t nbNodes = graph->numberOfNodes();
  const std::vector<node> &nodes = graph->nodes();

  vpsc::Rectangles nodeRectangles(nbNodes);

  for (float passIndex = 1; passIndex <= nbPasses; ++passIndex) {
    // initialization
    TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node &curNode, unsigned int i) {
      Size sz = viewSize->getNodeValue(curNode) * passIndex / float(nbPasses);
      const Coord &pos = viewLayout->getNodeValue(curNode);
      double curRot = viewRot->getNodeValue(curNode);
      Size rotSize = Size(sz.getW() * fabs(cos(curRot * M_PI / 180.0)) +
                              sz.getH() * fabs(sin(curRot * M_PI / 180.0)),
                          sz.getW() * fabs(sin(curRot * M_PI / 180.0)) +
                              sz.getH() * fabs(cos(curRot * M_PI / 180.0)),
                          1.0f);
      double maxX = pos.getX() + rotSize.getW() / 2.0;
      double maxY = pos.getY() + rotSize.getH() / 2.0;
      double minX = pos.getX() - rotSize.getW() / 2.0;
      double minY = pos.getY() - rotSize.getH() / 2.0;

      nodeRectangles[i] = new vpsc::Rectangle(minX, maxX, minY, maxY);
      nodeRectangles[i]->setXBorder(xBorder);
      nodeRectangles[i]->setYBorder(yBorder);
    });

    // actually apply fast overlap removal
    vpsc::removeRectangleOverlaps(nodeRectangles);

    for (unsigned int i = 0; i < nbNodes; ++i) {
      Coord newPos(nodeRectangles[i]->getCentreX(), nodeRectangles[i]->getCentreY(), 0.0);
      LayoutAlgorithm::result->setNodeValue(nodes[i], newPos);
    }

    for (vpsc::Rectangle *rect : nodeRectangles) {
      delete rect;
    }

  }

  return true;
} // end run
