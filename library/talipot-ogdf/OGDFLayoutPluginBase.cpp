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

#include "talipot2ogdf/OGDFLayoutPluginBase.h"

#include <vector>
#include <talipot/DrawingTools.h>

using namespace std;

OGDFLayoutPluginBase::OGDFLayoutPluginBase(const tlp::PluginContext *context,
                                           ogdf::LayoutModule *ogdfLayoutAlgo)
    : tlp::LayoutAlgorithm(context), tlpToOGDF(nullptr), ogdfLayoutAlgo(ogdfLayoutAlgo) {
  // convert Tulip Graph to OGDF Graph including attributes
  if (graph)
    tlpToOGDF = new TalipotToOGDF(graph, false);
}

OGDFLayoutPluginBase::~OGDFLayoutPluginBase() {
  delete tlpToOGDF;
  delete ogdfLayoutAlgo;
}

bool OGDFLayoutPluginBase::run() {
  if (pluginProgress) {
    // user cannot interact while computing
    pluginProgress->showPreview(false);
    pluginProgress->showStops(false);
  }

  ogdf::GraphAttributes &gAttributes = tlpToOGDF->getOGDFGraphAttr();

  beforeCall();

  try {
    // run the algorithm on the OGDF Graph with attributes
    callOGDFLayoutAlgorithm(gAttributes);
  } catch (PreconditionViolatedException &pvce) {
    std::string msg;

    switch (pvce.exceptionCode()) {
    case pvcSelfLoop:
      msg = "graph contains a self-loop";
      break;

    case pvcTreeHierarchies:
      msg = "hierarchies are not only trees";
      break;

    case pvcAcyclicHierarchies:
      msg = "hierarchies are not acyclic";
      break;

    case pvcSingleSource:
      msg = "graph has not a single source";
      break;

    case pvcUpwardPlanar:
      msg = "graph is not upward planar";
      break;

    case pvcTree:
      msg = "graph is not a rooted tree";
      break;

    case pvcForest:
      msg = "graph is not a rooted forest";
      break;

    case pvcOrthogonal:
      msg = "layout is not orthogonal";
      break;

    case pvcPlanar:
      msg = "graph is not planar";
      break;

    case pvcClusterPlanar:
      msg = "graph is not c-planar";
      break;

    case pvcNoCopy:
      msg = "graph is not a copy of the corresponding graph";
      break;

    case pvcConnected:
      msg = "graph is not connected";
      break;

    case pvcBiconnected:
      msg = "graph is not twoconnected";
      break;

    default:
      msg = "unknown error";
    }

    pluginProgress->setError(std::string("Error\n") + msg);
    return false;
  } catch (AlgorithmFailureException &afce) {
    std::string msg;

    switch (afce.exceptionCode()) {
    case afcIllegalParameter:
      msg = "function parameter is illegal";
      break;

    case afcNoFlow:
      msg = "min-cost flow could not find a legal flow";
      break;

    case afcSort:
      msg = "sequence not sorted";
      break;

    case afcLabel:
      msg = "labelling failed";
      break;

    case afcExternalFace:
      msg = "external face not correct";
      break;

    case afcForbiddenCrossing:
      msg = "crossing forbidden but necessary";
      break;

    case afcTimelimitExceeded:
      msg = "it took too long";
      break;

    case afcNoSolutionFound:
      msg = "couldn't solve the problem";
      break;

    default:
      msg = "unknown error";
    }

    pluginProgress->setError(std::string("Error\n") + msg);
    return false;
  }

  // retrieve nodes coordinates computed by the OGDF Layout Algorithm
  // and store them in the Tulip Layout Property
  const std::vector<tlp::node> &nodes = graph->nodes();
  unsigned int nbElts = nodes.size();

  for (unsigned int i = 0; i < nbElts; ++i) {
    tlp::Coord nodeCoord = tlpToOGDF->getNodeCoordFromOGDFGraphAttr(i);
    result->setNodeValue(nodes[i], nodeCoord);
  }

  // same operation as above but with edges
  const std::vector<tlp::edge> &edges = graph->edges();
  nbElts = edges.size();

  for (unsigned int i = 0; i < nbElts; ++i) {
    vector<tlp::Coord> edgeCoord = tlpToOGDF->getEdgeCoordFromOGDFGraphAttr(i);
    result->setEdgeValue(edges[i], edgeCoord);
  }

  afterCall();

  return true;
}

void OGDFLayoutPluginBase::callOGDFLayoutAlgorithm(ogdf::GraphAttributes &gAttributes) {
  ogdfLayoutAlgo->call(gAttributes);
}

void OGDFLayoutPluginBase::transposeLayoutVertically() {

  const vector<tlp::node> &nodes = graph->nodes();
  const vector<tlp::edge> &edges = graph->edges();

  tlp::BoundingBox graphBB =
      tlp::computeBoundingBox(nodes, edges, result, graph->getSizeProperty("viewSize"),
                              graph->getDoubleProperty("viewRotation"));
  float midY = (graphBB[0][1] + graphBB[1][1]) / 2.f;

  for (auto n : nodes) {
    tlp::Coord nodeCoord = result->getNodeValue(n);
    nodeCoord[1] = midY - (nodeCoord[1] - midY);
    result->setNodeValue(n, nodeCoord);
  }

  for (auto e : edges) {
    std::vector<tlp::Coord> bends = result->getEdgeValue(e);

    if (bends.size()) {
      for (size_t i = 0; i < bends.size(); ++i) {
        bends[i][1] = midY - (bends[i][1] - midY);
      }

      result->setEdgeValue(e, bends);
    }
  }
}
