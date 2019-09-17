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
#include <tulip/GlGraphRenderer.h>
#include <tulip/GraphProperty.h>
#include <tulip/GlTools.h>
#include <tulip/GlScene.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlSceneVisitor.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GraphParallelTools.h>

using namespace std;

namespace tlp {

GlGraphRenderer::GlGraphRenderer(const GlGraphInputData *inputData)
    : inputData(inputData), graphModified(true), selectionDrawActivate(false),
      selectionIdMap(nullptr), selectionCurrentId(nullptr) {}

void GlGraphRenderer::visitGraph(GlSceneVisitor *visitor, bool visitHiddenEntities) {
  Graph *graph = inputData->getGraph();

  if (!graph)
    return;

  unsigned int nbNodes = graph->numberOfNodes();
  unsigned int nbEdges = graph->numberOfEdges();
  if (!visitHiddenEntities && !inputData->parameters->isViewMetaLabel()) {
    if (!inputData->parameters->isDisplayNodes() && !inputData->parameters->isViewNodeLabel())
      nbNodes = 0;
    if (!inputData->parameters->isDisplayEdges() && !inputData->parameters->isViewEdgeLabel())
      nbEdges = 0;
  }
  visitor->reserveMemoryForGraphElts(nbNodes, nbEdges);
  if (nbNodes)
    visitNodes(graph, visitor);
  if (nbEdges)
    visitEdges(graph, visitor);
  visitor->endOfVisit();
}

void GlGraphRenderer::visitNodes(Graph *graph, GlSceneVisitor *visitor) {
  auto fn = [&](node n, unsigned int i) {
    GlNode glNode(n, i);
    visitor->visit(&glNode);
  };

  if (visitor->isThreadSafe())
    TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, fn);
  else
    TLP_MAP_NODES_AND_INDICES(graph, fn);
}

void GlGraphRenderer::visitEdges(Graph *graph, GlSceneVisitor *visitor) {
  auto fn = [&](edge e, unsigned int i) {
    GlEdge glEdge(e, i);
    visitor->visit(&glEdge);
  };

  if (visitor->isThreadSafe())
    TLP_PARALLEL_MAP_EDGES_AND_INDICES(graph, fn);
  else
    TLP_MAP_EDGES_AND_INDICES(graph, fn);
}
} // namespace tlp
