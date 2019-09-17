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

#ifndef TALIPOT_GL_QUAD_TREE_LOD_CALCULATOR_H
#define TALIPOT_GL_QUAD_TREE_LOD_CALCULATOR_H

#include <unordered_map>
#include <vector>

#include <talipot/GlCPULODCalculator.h>
#include <talipot/Observable.h>
#include <talipot/GlGraphRenderingParameters.h>

namespace tlp {

class Camera;
template <class TYPE>
class QuadTreeNode;
class GlScene;
class PropertyInterface;
class Graph;
class GlLayer;

/**
 * Class use to compute bounding boxs of a vector of GlEntity
 */
class TLP_GL_SCOPE GlQuadTreeLODCalculator : public GlCPULODCalculator, private Observable {

public:
  GlQuadTreeLODCalculator();
  ~GlQuadTreeLODCalculator() override;

  /**
   * Set the SlScene used by this calculator
   */
  void setScene(GlScene &scene) override;

  /**
   * To know if the calculator need to have entities returned by a visitor in GlScene
   */
  bool needEntities() override;
  /**
   * Set if the calculator need to have entities
   */
  void setNeedEntities(bool) override;

  /**
   * This function is call by GlLODSceneVisitor when a simple entitie is found
   */
  void addSimpleEntityBoundingBox(GlSimpleEntity *entity, const BoundingBox &bb) override;
  /**
   * This function is call by GlLODSceneVisitor when an edge is found
   */
  void addEdgeBoundingBox(unsigned int id, unsigned int pos, const BoundingBox &bb) override;

  /**
   * This function compute LOD
   * See compute function of GlCPULODCalculator for more details
   * This function do some computation and after call computeFor2DCamera() or computeFor3DCamera()
   */
  void compute(const Vector<int, 4> &globalViewport,
               const Vector<int, 4> &currentViewport) override;

  /**
   * Specific function to compute LOD for 3D cameras
   */
  void computeFor3DCamera(LayerLODUnit *layerLODUnit, const Coord &eye,
                          const Matrix<float, 4> &transformMatrix,
                          const Vector<int, 4> &globalViewport,
                          const Vector<int, 4> &currentViewport) override;

  /**
   * Change the input data used by this LOD calculator
   */
  void setInputData(const GlGraphInputData *newInputData) override;

  /**
   * Clone the calculator
   */
  GlLODCalculator *clone() override {
    GlQuadTreeLODCalculator *newCalculator = new GlQuadTreeLODCalculator();
    newCalculator->setScene(*glScene);
    newCalculator->setInputData(inputData);
    return newCalculator;
  }

protected:
  void update(PropertyInterface *property);
  void treatEvent(const Event &ev) override;

  void removeObservers();
  void addObservers();

  void initCamerasObservers();
  void clearCamerasObservers();

  void setHaveToCompute();

  std::vector<QuadTreeNode<std::pair<uint, uint>> *> nodesQuadTree;
  std::vector<QuadTreeNode<std::pair<uint, uint>> *> edgesQuadTree;
  std::vector<QuadTreeNode<GlSimpleEntity *> *> entitiesQuadTree;
  std::vector<std::vector<SimpleEntityLODUnit>> simpleEntities;

  bool haveToCompute;
  bool haveToInitObservers;

  // index of simple entities bounding in bbs (see CPULODCalculator.h)
  const unsigned int seBBIndex;
  // offset of edge entities bounding in bbs
  const unsigned int eBBOffset;

  std::vector<Camera *> cameras;
  std::unordered_map<GlLayer *, Camera> layerToCamera;
  Camera *currentCamera;
  Graph *currentGraph;
  PropertyInterface *layoutProperty;
  PropertyInterface *sizeProperty;
  PropertyInterface *selectionProperty;
  GlGraphRenderingParameters oldParameters;

  int quadTreesVectorPosition;
  int simpleEntitiesVectorPosition;
};
} // namespace tlp

#endif // TALIPOT_GL_QUAD_TREE_LOD_CALCULATOR_H
///@endcond
