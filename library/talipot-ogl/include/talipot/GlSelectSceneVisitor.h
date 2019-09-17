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

///@cond DOXYGEN_HIDDEN

#ifndef TALIPOT_GL_SELECT_SCENE_VISITOR_H
#define TALIPOT_GL_SELECT_SCENE_VISITOR_H

#include <talipot/GlSceneVisitor.h>

namespace tlp {

enum SelectionFlag { SelectSimpleEntities = 1, SelectNodes = 2, SelectEdges = 4 };

class GlGraphInputData;
class GlLODCalculator;

/**
 * This visitor is use when we want to select an entity in scene
 */
class TLP_GL_SCOPE GlSelectSceneVisitor : public GlSceneVisitor {

public:
  /**
   * Constructor with SelectionFlac (SelectSimpleEntity, SelectNodes and SelectEdges),
   * GlGraphInputData and GlLODCalculator
   */
  GlSelectSceneVisitor(SelectionFlag flag, GlGraphInputData *inputData,
                       GlLODCalculator *calculator);

  /**
   * Visit a simple entity
   */
  void visit(GlSimpleEntity *entity) override;
  /**
   * Visit a node
   */
  void visit(GlNode *glNode) override;
  /**
   * Visit an edge
   */
  void visit(GlEdge *glEdge) override;
  /**
   * Visit a layer
   */
  void visit(GlLayer *layer) override;

private:
  SelectionFlag selectionFlag;

  GlGraphInputData *inputData;

  GlLODCalculator *calculator;
};
} // namespace tlp

#endif // TALIPOT_GL_SELECT_SCENE_VISITOR_H
///@endcond
