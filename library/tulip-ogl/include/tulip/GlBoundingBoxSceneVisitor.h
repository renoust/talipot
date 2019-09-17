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

#ifndef Tulip_GLBOUNDINGBOXSCENEVISITOR_H
#define Tulip_GLBOUNDINGBOXSCENEVISITOR_H
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <vector>

#include <tulip/BoundingBox.h>
#include <tulip/GlSceneVisitor.h>

namespace tlp {

class GlGraphInputData;

/** \brief Visitor to collect boundingBox of all GlEntities
 *
 * Visitor to collect boundingBox of all GlEntities
 * At end, boundingBox member contains the scene boundingBox (in 3D coordinates)
 * This class can be useful to center the scene in a widget for example
 */
class TLP_GL_SCOPE GlBoundingBoxSceneVisitor : public GlSceneVisitor {

public:
  /**
   * Constructor
   */
  GlBoundingBoxSceneVisitor(GlGraphInputData *inputData);

  /**
   * Method used for GlSimpleEntity
   */
  void visit(GlSimpleEntity *entity) override;
  /**
   * Method used for GlNodes (and GlMetaNodes)
   */
  void visit(GlNode *glNode) override;
  /**
   * Method used for GlEdges
   */
  void visit(GlEdge *glEdge) override;

  /**
   * Return the scene boundingBox
   */
  BoundingBox getBoundingBox();

private:
  std::vector<bool> noBBCheck;
  std::vector<BoundingBox> bbs;
  GlGraphInputData *inputData;
};
} // namespace tlp

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLLODSCENEVISITOR_H
///@endcond
