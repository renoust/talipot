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

#include <algorithm>
#include <array>

#include <talipot/GlGraphHighDetailsRenderer.h>
#include <talipot/GraphProperty.h>
#include <talipot/NumericProperty.h>
#include <talipot/GlTools.h>
#include <talipot/GlScene.h>
#include <talipot/GlVertexArrayManager.h>
#include <talipot/GlBoundingBoxSceneVisitor.h>
#include <talipot/OcclusionTest.h>
#include <talipot/GlEdge.h>
#include <talipot/GlNode.h>
#include <talipot/Camera.h>
#include <talipot/GlGraphInputData.h>
#include <talipot/GlGraphRenderingParameters.h>
#include <talipot/GlGlyphRenderer.h>
#include <talipot/OpenGlConfigManager.h>

using namespace std;

namespace tlp {

/** \brief Storage class for Z ordering
 * Storage class for Z ordering
 */
struct EntityWithDistance {

  EntityWithDistance(const double &dist, EntityLODUnit *entity)
      : distance(dist), entity(entity), isComplexEntity(false), isNode(false) {}
  EntityWithDistance(const double &dist, ComplexEntityLODUnit *entity, bool isNode)
      : distance(dist), entity(entity), isComplexEntity(true), isNode(isNode) {}

  double distance;
  EntityLODUnit *entity;
  bool isComplexEntity;
  bool isNode;
};

/** \brief Comparator to order entities with Z
 * Comparator to order entities with Z
 */
struct entityWithDistanceCompare {
  static const GlGraphInputData *inputData;
  bool operator()(const EntityWithDistance &e1, const EntityWithDistance &e2) const {

    // opaque objects have to be drawn first
    if (e1.isComplexEntity && e2.isComplexEntity) {
      Color e1Color, e2Color;

      if (e1.isNode) {
        e1Color = inputData->getElementColor()->getNodeValue(
            node(static_cast<ComplexEntityLODUnit *>(e1.entity)->id));
      } else {
        e1Color = inputData->getElementColor()->getEdgeValue(
            edge(static_cast<ComplexEntityLODUnit *>(e1.entity)->id));
      }

      if (e2.isNode) {
        e2Color = inputData->getElementColor()->getNodeValue(
            node(static_cast<ComplexEntityLODUnit *>(e2.entity)->id));
      } else {
        e2Color = inputData->getElementColor()->getEdgeValue(
            edge(static_cast<ComplexEntityLODUnit *>(e2.entity)->id));
      }

      if (e1Color[3] == 255 && e2Color[3] == 255) {
        return e1.distance > e2.distance;
      }

      if (e1Color[3] == 255 && e2Color[3] < 255) {
        return true;
      }

      if (e2Color[3] == 255 && e1Color[3] < 255) {
        return false;
      }
    }

    // transparent objects ordering

    // if e2 bounding box is contained in e1 bounding box,
    // e2 must be drawn before e1
    if (e1.entity->boundingBox.contains(e2.entity->boundingBox)) {
      return false;
    }

    // if e1 bounding box is contained in e2 bounding box,
    // e1 must be drawn before e2
    if (e2.entity->boundingBox.contains(e1.entity->boundingBox)) {
      return true;
    }

    // if e1 is further from the viewer than e2,
    // e1 must be drawn before e2
    if (e1.distance > e2.distance)
      return true;

    // if e2 is further from the viewer than e1,
    // e2 must be drawn before e2
    if (e1.distance < e2.distance)
      return false;

    return (e2.entity->boundingBox.width() >= e1.entity->boundingBox.width());
  }
};

const GlGraphInputData *entityWithDistanceCompare::inputData = nullptr;

/** \brief Comparator to order entities with metric to display labels with metric ordering
 * Comparator to order entities with metric to display labels with metric ordering
 */
class GreatThanNode {
public:
  NumericProperty *metric;
  bool operator()(pair<node, float> n1, pair<node, float> n2) {
    return (metric->getNodeDoubleValue(n1.first) > metric->getNodeDoubleValue(n2.first));
  }
};
/** \brief Comparator to order entities with metric to display labels with metric ordering
 * Comparator to order entities with metric to display labels with metric ordering
 */
class GreatThanEdge {
public:
  NumericProperty *metric;
  bool operator()(pair<edge, float> e1, pair<edge, float> e2) {
    return (metric->getEdgeDoubleValue(e1.first) > metric->getEdgeDoubleValue(e2.first));
  }
};

//===================================================================
//
// GlGraphHighDetailsRenderer functions implementation
//
//===================================================================

GlGraphHighDetailsRenderer::GlGraphHighDetailsRenderer(const GlGraphInputData *inputData)
    : GlGraphRenderer(inputData), lodCalculator(nullptr), baseScene(nullptr),
      fakeScene(new GlScene) {
  fakeScene->createLayer("fakeLayer");
}
//===================================================================
GlGraphHighDetailsRenderer::GlGraphHighDetailsRenderer(const GlGraphInputData *inputData,
                                                       GlScene *scene)
    : GlGraphRenderer(inputData), lodCalculator(nullptr), baseScene(scene), fakeScene(new GlScene) {
  fakeScene->createLayer("fakeLayer");
}
//===================================================================
GlGraphHighDetailsRenderer::~GlGraphHighDetailsRenderer() {
  if (lodCalculator)
    lodCalculator->setAttachedLODCalculator(nullptr);

  delete lodCalculator;

  delete fakeScene;
}
//===================================================================
void GlGraphHighDetailsRenderer::draw(float, Camera *camera) {

  if (!inputData->renderingParameters()->isAntialiased()) {
    OpenGlConfigManager::deactivateAntiAliasing();
  }

  Graph *graph = inputData->getGraph();

  // If we don't init lod calculator : clone the scene one
  if (!lodCalculator) {
    if (baseScene) {
      lodCalculator = baseScene->getCalculator()->clone();
      lodCalculator->setAttachedLODCalculator(baseScene->getCalculator());
    } else {
      lodCalculator = camera->getScene()->getCalculator()->clone();
      lodCalculator->setAttachedLODCalculator(camera->getScene()->getCalculator());
    }

    lodCalculator->setInputData(inputData);
    lodCalculator->setScene(*fakeScene);
  }

  lodCalculator->clear();

  if (!selectionDrawActivate) {
    lodCalculator->setRenderingEntitiesFlag(RenderingAll);
  } else {
    lodCalculator->setRenderingEntitiesFlag(
        static_cast<RenderingEntitiesFlag>(selectionType | RenderingWithoutRemove));
  }

  // Fake scene creation
  //  This scene is needed by lod calculator to compute lod
  fakeScene->setViewport(camera->getViewport()[0], camera->getViewport()[1],
                         camera->getViewport()[2], camera->getViewport()[3]);
  fakeScene->getLayer("fakeLayer")->setSharedCamera(camera);

  // LOD computation
  if (lodCalculator->needEntities()) {
    lodCalculator->visit(fakeScene->getLayer("fakeLayer"));
    visitGraph(lodCalculator);
  }

  if (!selectionDrawActivate) {
    lodCalculator->compute(fakeScene->getViewport(), fakeScene->getViewport());
  } else {
    lodCalculator->compute(fakeScene->getViewport(), selectionViewport);
  }

  LayersLODVector &layersLODVector = lodCalculator->getResult();

  auto vertexArrayManager = inputData->getGlVertexArrayManager();
  bool vertexArrayManagerActivated = vertexArrayManager->isActivated();

  if (vertexArrayManagerActivated) {
    // VertexArrayManager begin
    if (!selectionDrawActivate) {
      // inputData->getGlVertexArrayManager()->activate(true);
      vertexArrayManager->beginRendering();
      inputData->getGlGlyphRenderer()->startRendering();
    } else {
      vertexArrayManager->activate(false);
    }
  }

  // VertexArrayManager update
  if (vertexArrayManager->haveToCompute()) {
    visitGraph(vertexArrayManager, true);
    vertexArrayManager->setHaveToComputeAll(false);
  }

  BooleanProperty *filteringProperty = inputData->parameters->getDisplayFilteringProperty();
  NumericProperty *metric = inputData->parameters->getElementOrderingProperty();
  bool displayNodes = inputData->parameters->isDisplayNodes();
  bool displayMetaNodes = inputData->parameters->isDisplayMetaNodes();
  bool displayMetaNodesLabel = inputData->parameters->isViewMetaLabel();
  bool displayEdges = inputData->parameters->isDisplayEdges();

  bool renderOnlyOneNode = !selectionDrawActivate &&
                           !inputData->getElementLayout()->hasNonDefaultValuatedNodes() &&
                           !inputData->getElementSize()->hasNonDefaultValuatedNodes();

  if (!inputData->parameters->isElementZOrdered()) {

    vector<pair<node, float>> nodesMetricOrdered;
    vector<pair<edge, float>> edgesMetricOrdered;

    // draw nodes and metanodes
    for (auto &it : layersLODVector[0].nodesLODVector) {

      if ((it.lod <= 0) || (filteringProperty && filteringProperty->getNodeValue(node(it.id))))
        continue;

      if (displayNodes ||
          ((displayMetaNodes || displayMetaNodesLabel) && graph->isMetaNode(node(it.id)))) {
        if (!metric) {
          if (selectionDrawActivate) {
            if ((selectionType & RenderingNodes) == 0)
              continue;

            (*selectionIdMap)[*selectionCurrentId] =
                SelectedEntity(graph, it.id, SelectedEntity::NODE_SELECTED);
            glLoadName(*selectionCurrentId);
            (*selectionCurrentId)++;
          }

          GlNode glNode(it.id, it.pos);
          glNode.draw(it.lod, inputData, camera);
        } else {
          nodesMetricOrdered.emplace_back(node(it.id), it.lod);
        }

        if (renderOnlyOneNode)
          break;
      }
    }

    if (metric) {
      // Draw nodes with metric ordering
      GreatThanNode ltn;
      ltn.metric = metric;
      sort(nodesMetricOrdered.begin(), nodesMetricOrdered.end(), ltn);

      if (!inputData->renderingParameters()->isElementOrderedDescending()) {
        std::reverse(nodesMetricOrdered.begin(), nodesMetricOrdered.end());
      }

      for (auto &it : nodesMetricOrdered) {
        if (selectionDrawActivate) {
          if ((selectionType & RenderingNodes) == 0)
            continue;

          (*selectionIdMap)[*selectionCurrentId] =
              SelectedEntity(graph, it.first.id, SelectedEntity::NODE_SELECTED);
          glLoadName(*selectionCurrentId);
          (*selectionCurrentId)++;
        }

        GlNode glNode(it.first.id, graph->nodePos(node(it.first.id)));
        glNode.draw(it.second, inputData, camera);
      }
    }

    if (!renderOnlyOneNode) {

      // draw edges
      for (auto &it : layersLODVector[0].edgesLODVector) {

        if ((it.lod <= 0) || (filteringProperty && filteringProperty->getEdgeValue(edge(it.id))) ||
            !displayEdges)
          continue;

        if (!metric) {
          if (selectionDrawActivate) {
            if ((selectionType & RenderingEdges) == 0)
              continue;

            (*selectionIdMap)[*selectionCurrentId] =
                SelectedEntity(graph, it.id, SelectedEntity::EDGE_SELECTED);
            glLoadName(*selectionCurrentId);
            (*selectionCurrentId)++;
          }

          GlEdge glEdge(it.id, it.pos, selectionDrawActivate);
          glEdge.draw(it.lod, inputData, camera);
        } else {
          edgesMetricOrdered.emplace_back(edge(it.id), it.lod);
        }
      }

      if (metric) {
        // Draw edges with metric ordering
        GreatThanEdge lte;
        lte.metric = metric;
        sort(edgesMetricOrdered.begin(), edgesMetricOrdered.end(), lte);

        if (!inputData->renderingParameters()->isElementOrderedDescending()) {
          reverse(edgesMetricOrdered.begin(), edgesMetricOrdered.end());
        }

        for (auto &it : edgesMetricOrdered) {
          if (selectionDrawActivate) {
            if ((selectionType & RenderingEdges) == 0)
              continue;

            (*selectionIdMap)[*selectionCurrentId] =
                SelectedEntity(graph, it.first.id, SelectedEntity::EDGE_SELECTED);
            glLoadName(*selectionCurrentId);
            (*selectionCurrentId)++;
          }

          GlEdge glEdge(it.first.id, graph->edgePos(edge(it.first.id)), selectionDrawActivate);
          glEdge.draw(it.second, inputData, camera);
        }
      }
    }
  } else {

    entityWithDistanceCompare::inputData = inputData;
    multiset<EntityWithDistance, entityWithDistanceCompare> entitiesSet;
    Coord camPos = camera->getEyes();

    if (!selectionDrawActivate || ((selectionType & RenderingNodes) != 0)) {
      // Collect complex entities
      for (auto &it : layersLODVector[0].nodesLODVector) {

        if ((it.lod < 0) || (filteringProperty && filteringProperty->getNodeValue(node(it.id))))
          continue;

        BoundingBox &bb = it.boundingBox;
        Coord middle((bb[1] + bb[0]) / 2.f);
        double dist =
            (double(middle[0]) - double(camPos[0])) * (double(middle[0]) - double(camPos[0]));
        dist += (double(middle[1]) - double(camPos[1])) * (double(middle[1]) - double(camPos[1]));
        dist += (double(middle[2]) - double(camPos[2])) * (double(middle[2]) - double(camPos[2]));
        entitiesSet.insert(EntityWithDistance(dist, &it, true));
      }
    }

    if (!selectionDrawActivate || ((selectionType & RenderingEdges) != 0)) {
      for (auto &it : layersLODVector[0].edgesLODVector) {
        if ((it.lod < 0) || (filteringProperty && filteringProperty->getEdgeValue(edge(it.id))))
          continue;

        BoundingBox &bb = it.boundingBox;
        Coord middle((bb[0] + bb[1]) / 2.f);
        double dist =
            (double(middle[0]) - double(camPos[0])) * (double(middle[0]) - double(camPos[0]));
        dist += (double(middle[1]) - double(camPos[1])) * (double(middle[1]) - double(camPos[1]));
        dist += (double(middle[2]) - double(camPos[2])) * (double(middle[2]) - double(camPos[2]));
        entitiesSet.insert(EntityWithDistance(dist, &it, false));
      }
    }

    // Draw
    for (multiset<EntityWithDistance, entityWithDistanceCompare>::iterator it = entitiesSet.begin();
         it != entitiesSet.end(); ++it) {
      // Complex entities
      ComplexEntityLODUnit *entity = static_cast<ComplexEntityLODUnit *>(it->entity);

      if (it->isNode) {

        if (displayNodes ||
            ((displayMetaNodes || displayMetaNodesLabel) && graph->isMetaNode(node(entity->id)))) {

          // All opaque elements have been drawn, turn the depth buffer read-only
          // in order for a transparent object to not occlude another transparent object
          if (inputData->getElementColor()->getNodeValue(node(entity->id)).getA() < 255) {
            glDepthMask(GL_FALSE);
          }

          if (selectionDrawActivate) {
            if ((selectionType & RenderingNodes) == 0)
              continue;

            (*selectionIdMap)[*selectionCurrentId] =
                SelectedEntity(graph, entity->id, SelectedEntity::NODE_SELECTED);
            glLoadName(*selectionCurrentId);
            (*selectionCurrentId)++;
          }

          GlNode glNode(entity->id, graph->nodePos(node(entity->id)));
          glNode.draw(entity->lod, inputData, camera);

          if (renderOnlyOneNode)
            break;
        }

      } else {
        if (!displayEdges)
          continue;

        // All opaque elements have been drawn, turn the depth buffer read-only
        // in order for a transparent object to not occlude another transparent object
        if (inputData->getElementColor()->getEdgeValue(edge(entity->id)).getA() < 255) {
          glDepthMask(GL_FALSE);
        }

        if (selectionDrawActivate) {
          if ((selectionType & RenderingEdges) == 0)
            continue;

          (*selectionIdMap)[*selectionCurrentId] =
              SelectedEntity(graph, entity->id, SelectedEntity::EDGE_SELECTED);
          glLoadName(*selectionCurrentId);
          (*selectionCurrentId)++;
        }

        GlEdge glEdge(entity->id, graph->edgePos(edge(entity->id)), selectionDrawActivate);
        glEdge.draw(entity->lod, inputData, camera);
      }
    }

    glDepthMask(GL_TRUE);
  }

  if (!selectionDrawActivate) {
    if (vertexArrayManagerActivated) {
      if (inputData->renderingParameters()->isEdgeFrontDisplay()) {
        vertexArrayManager->endRendering();
        inputData->getGlGlyphRenderer()->endRendering();
      } else {
        inputData->getGlGlyphRenderer()->endRendering();
        vertexArrayManager->endRendering();
      }
    }
  } else {
    selectionDrawActivate = false;
    vertexArrayManager->activate(true);
    OpenGlConfigManager::activateAntiAliasing();
    return;
  }

  /*
    Label draw
  */
  OcclusionTest occlusionTest;
  bool labelDensityAtZero = true;

  if (inputData->parameters->getLabelsDensity() != -100)
    labelDensityAtZero = false;

  if (!labelDensityAtZero) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glDepthFunc(GL_ALWAYS);
    glDisable(GL_CULL_FACE);
    glDisable(GL_COLOR_MATERIAL);

    // Draw Labels for selected entities
    drawLabelsForComplexEntities(true, &occlusionTest, layersLODVector[0]);

    // Draw Labels for unselected entities
    drawLabelsForComplexEntities(false, &occlusionTest, layersLODVector[0]);

    glPopAttrib();
  }

  selectionDrawActivate = false;

  OpenGlConfigManager::activateAntiAliasing();
}
//===================================================================
void GlGraphHighDetailsRenderer::selectEntities(Camera *camera, RenderingEntitiesFlag type, int x,
                                                int y, int w, int h,
                                                vector<SelectedEntity> &selectedEntities) {
  unordered_map<unsigned int, SelectedEntity> idToEntity;
  unsigned int id = 1;

  unsigned int size =
      inputData->getGraph()->numberOfNodes() + inputData->getGraph()->numberOfEdges();

  // Allocate memory to store the result of the selection
  vector<std::array<GLuint, 4>> selectBuf(size);
  glSelectBuffer(size * 4, reinterpret_cast<GLuint *>(selectBuf.data()));
  // Activate Open Gl Selection mode
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);

  initSelectionRendering(type, x, y, w, h, idToEntity, id);

  draw(20, camera);

  glFlush();
  GLint hits = glRenderMode(GL_RENDER);

  while (hits > 0) {
    selectedEntities.push_back(idToEntity[selectBuf[--hits][3]]);
  }
}
//===================================================================
void GlGraphHighDetailsRenderer::initSelectionRendering(
    RenderingEntitiesFlag type, int x, int y, int w, int h,
    unordered_map<unsigned int, SelectedEntity> &idMap, unsigned int &currentId) {
  selectionType = type;
  selectionIdMap = &idMap;
  selectionCurrentId = &currentId;
  selectionDrawActivate = true;
  selectionViewport[0] = x;
  selectionViewport[1] = y;
  selectionViewport[2] = w;
  selectionViewport[3] = h;
}
//===================================================================
void GlGraphHighDetailsRenderer::drawLabelsForComplexEntities(bool drawSelected,
                                                              OcclusionTest *occlusionTest,
                                                              LayerLODUnit &layerLODUnit) {
  Graph *graph = inputData->getGraph();
  BooleanProperty *selectionProperty = inputData->getElementSelected();
  bool viewOutScreenLabel = inputData->parameters->isViewOutScreenLabel();
  NumericProperty *metric = inputData->parameters->getElementOrderingProperty();
  BooleanProperty *filteringProperty = inputData->parameters->getDisplayFilteringProperty();

  // Draw Labels for Nodes
  if (inputData->parameters->isViewNodeLabel() &&
      ((!inputData->getElementLabel()->getNodeDefaultStringValue().empty()) ||
       inputData->getElementLabel()->hasNonDefaultValuatedNodes())) {

    vector<pair<node, float>> nodesMetricOrdered;
    for (auto &it : layerLODUnit.nodesLODVector) {
      float lod = it.lod;

      if ((lod < 0 && !viewOutScreenLabel) ||
          (lod < 10 && inputData->renderingParameters()->isLabelScaled()))
        continue;

      if (viewOutScreenLabel && lod < 0)
        lod = -lod;

      node n(it.id);

      if (filteringProperty && filteringProperty->getNodeValue(n)) {
        continue;
      }

      if (selectionProperty->getNodeValue(n) == drawSelected) {
        if (!metric) {
          GlNode glNode(n.id, it.pos);
          glNode.drawLabel(occlusionTest, inputData, lod, layerLODUnit.camera);
        } else {
          // Metric ordered
          nodesMetricOrdered.emplace_back(n, lod);
        }
      }
    }

    // If not Metric ordered : a this point selected nodes are draw

    if (metric) {
      // Draw selected nodes label with metric ordering
      GreatThanNode ltn;
      ltn.metric = metric;
      sort(nodesMetricOrdered.begin(), nodesMetricOrdered.end(), ltn);

      for (auto it : nodesMetricOrdered) {
        GlNode glNode(it.first.id, graph->nodePos(node(it.first.id)));
        glNode.drawLabel(occlusionTest, inputData, it.second, layerLODUnit.camera);
      }
    }
  }

  // Draw Labels for Edges
  if (inputData->parameters->isViewEdgeLabel() &&
      ((!inputData->getElementLabel()->getEdgeDefaultStringValue().empty()) ||
       inputData->getElementLabel()->hasNonDefaultValuatedEdges())) {

    vector<pair<edge, float>> edgesMetricOrdered;
    for (auto &it : layerLODUnit.edgesLODVector) {
      if (it.lod < 5)
        continue;

      edge e(it.id);

      if (filteringProperty && filteringProperty->getEdgeValue(e)) {
        continue;
      }

      if (selectionProperty->getEdgeValue(e) == drawSelected) {
        if (!metric) {
          // Not metric ordered
          GlEdge glEdge(e.id, it.pos);
          glEdge.drawLabel(occlusionTest, inputData, it.lod, layerLODUnit.camera);
        } else {
          // Metric ordered
          edgesMetricOrdered.emplace_back(e, it.lod);
        }
      }
    }

    // If not Metric ordered : at this point selected edges are draw
    if (metric) {
      // Draw selected edges label with metric ordering

      GreatThanEdge lte;
      lte.metric = metric;
      sort(edgesMetricOrdered.begin(), edgesMetricOrdered.end(), lte);

      for (auto &it : edgesMetricOrdered) {
        GlEdge glEdge(it.first.id, graph->edgePos(edge(it.first.id)));
        glEdge.drawLabel(occlusionTest, inputData, it.second, layerLODUnit.camera);
      }
    }
  }
}
}
