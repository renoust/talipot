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
#include <tulip/GlBoundingBoxSceneVisitor.h>

#include <tulip/GlSimpleEntity.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/ParallelTools.h>

using namespace std;

namespace tlp {

GlBoundingBoxSceneVisitor::GlBoundingBoxSceneVisitor(GlGraphInputData *inputData)
    : inputData(inputData) {
  threadSafe = true;
  noBBCheck.assign(ThreadManager::getNumberOfThreads(), false);
  bbs.resize(ThreadManager::getNumberOfThreads());
}

BoundingBox GlBoundingBoxSceneVisitor::getBoundingBox() {
  BoundingBox bb(bbs[0]);

  for (unsigned int i = 1; i < bbs.size(); ++i)
    if (noBBCheck[i])
      bb.expand(bbs[i], true);
  return bb;
}

void GlBoundingBoxSceneVisitor::visit(GlSimpleEntity *entity) {
  if (entity->isVisible()) {
    BoundingBox &&bb = entity->getBoundingBox();

    if (bb.isValid()) {
      auto ti = ThreadManager::getThreadNumber();
      bbs[ti].expand(bb, noBBCheck[ti]);
      noBBCheck[ti] = true;
    }
  }
}

void GlBoundingBoxSceneVisitor::visit(GlNode *glNode) {
  BoundingBox &&bb = glNode->getBoundingBox(inputData);
  auto ti = ThreadManager::getThreadNumber();

  bbs[ti].expand(bb, noBBCheck[ti]);
  noBBCheck[ti] = true;
}

void GlBoundingBoxSceneVisitor::visit(GlEdge *glEdge) {
  BoundingBox &&bb = glEdge->getBoundingBox(inputData);
  auto ti = ThreadManager::getThreadNumber();

  bbs[ti].expand(bb, noBBCheck[ti]);
  noBBCheck[ti] = true;
}
} // namespace tlp
