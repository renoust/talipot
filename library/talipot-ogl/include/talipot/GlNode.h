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

#ifndef TALIPOT_GL_NODE_H
#define TALIPOT_GL_NODE_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <climits>
#include <talipot/Color.h>
#include <talipot/Coord.h>
#include <talipot/Size.h>
#include <talipot/GlBox.h>
#include <talipot/GlComplexeEntity.h>
#include <talipot/GlLabel.h>
#include <talipot/GlSceneVisitor.h>
#include <talipot/GlGraphInputData.h>

#include <memory>
#include <vector>

namespace tlp {

struct OcclusionTest;

/**
 * Class to represent a node of a graph
 */
class TLP_GL_SCOPE GlNode final : public GlComplexeEntity {

public:
  /**
   * Default constructor with id
   * id must be the id of the node in graph
   */
  GlNode(unsigned int _nid = UINT_MAX, unsigned int _npos = UINT_MAX)
      : id(_nid), pos(_npos), oldId(UINT_MAX),
        selectionBox(Coord(0, 0, 0), Size(1, 1, 1), Color(0, 0, 255, 255), Color(0, 255, 0, 255),
                     false, true, "", 3) {
    if (!label.get()) {
      std::call_once(onceFlag, []() { label.reset(new GlLabel); });
    }
  }

  /**
   * Virtual function to accept GlSceneVisitor on this class
   */
  void acceptVisitor(GlSceneVisitor *visitor) override {
    visitor->visit(this);
  }

  /**
   * Return the node bounding box
   */
  BoundingBox getBoundingBox(const GlGraphInputData *data) override;

  /**
   * Draw the node with level of detail : lod and Camera : camera
   */
  void draw(float lod, const GlGraphInputData *data, Camera *camera) override;

  /**
   * Draw the label of the node if drawNodesLabel is true and if label selection is equal to
   * drawSelect
   */
  void drawLabel(bool drawSelect, OcclusionTest *test, const GlGraphInputData *data, float lod);

  /**
   * Draw the label of the node if drawEdgesLabel is true
   */
  void drawLabel(OcclusionTest *test, const GlGraphInputData *data) override;

  /**
   * Draw the label of the node if drawEdgesLabel is true
   */
  void drawLabel(OcclusionTest *test, const GlGraphInputData *data, float lod,
                 Camera *camera = nullptr);

  // node id and node position in graph->nodes()
  unsigned int id, pos;

  /**
   * This function is used by the engine to get point coordinates of the node
   */
  Coord getPoint(GlGraphInputData *inputData) {
    init(inputData);
    return coord;
  }

  /**
   * This function is used by the engine to get color of the node
   */
  Color getColor(GlGraphInputData *inputData) {
    node n(id);

    return (inputData->getElementBorderWidth()->getNodeValue(n) > 0)
               ? inputData->getElementBorderColor()->getNodeValue(n)
               : inputData->getElementColor()->getNodeValue(n);
  }

protected:
  unsigned int oldId;
  GlBox selectionBox;
  static std::unique_ptr<GlLabel> label;
  static std::once_flag onceFlag;

  // initialize the data member below
  void init(const GlGraphInputData *data);

  tlp::Coord coord;
  int glyph;
  tlp::Size size;
  float rot;
  bool selected;
};
}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // TALIPOT_GL_NODE_H
///@endcond
