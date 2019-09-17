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
#include <tulip/GlSelectSceneVisitor.h>
#include <tulip/GlSimpleEntity.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/GlLayer.h>
#include <tulip/GlLODCalculator.h>

using namespace std;

namespace tlp {

GlSelectSceneVisitor::GlSelectSceneVisitor(SelectionFlag flag, GlGraphInputData *inputData,
                                           GlLODCalculator *calculator)
    : selectionFlag(flag), inputData(inputData), calculator(calculator) {}

void GlSelectSceneVisitor::visit(GlSimpleEntity *entity) {
  if (selectionFlag == SelectSimpleEntities)
    calculator->addSimpleEntityBoundingBox(entity, entity->getBoundingBox());
}

void GlSelectSceneVisitor::visit(GlNode *glNode) {
  if (selectionFlag == SelectNodes)
    calculator->addNodeBoundingBox(glNode->id, glNode->pos, glNode->getBoundingBox(inputData));
}

void GlSelectSceneVisitor::visit(GlEdge *glEdge) {
  if (selectionFlag == SelectEdges)
    calculator->addEdgeBoundingBox(glEdge->id, glEdge->pos, glEdge->getBoundingBox(inputData));
}

void GlSelectSceneVisitor::visit(GlLayer *layer) {
  calculator->beginNewCamera(&layer->getCamera());
}
} // namespace tlp
