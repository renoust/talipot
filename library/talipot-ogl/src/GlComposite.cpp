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

#include <talipot/GlComposite.h>

#include <talipot/GlLayer.h>
#include <talipot/GlScene.h>
#include <talipot/GlGraphComposite.h>
#include <talipot/GlXMLTools.h>

#include <algorithm>

using namespace std;

namespace tlp {
//============================================================
GlComposite::GlComposite(bool deleteComponentsInDestructor)
    : deleteComponentsInDestructor(deleteComponentsInDestructor) {}
//============================================================
GlComposite::~GlComposite() {
  reset(deleteComponentsInDestructor);
}
//============================================================
void GlComposite::addLayerParent(GlLayer *layer) {
  layerParents.push_back(layer);

  for (auto entity : _sortedElements) {
    GlComposite *composite = dynamic_cast<GlComposite *>(entity);

    if (composite) {
      composite->addLayerParent(layer);
    }
  }
}
//============================================================
void GlComposite::removeLayerParent(GlLayer *layer) {
  auto it = find(layerParents.begin(), layerParents.end(), layer);
  if (it != layerParents.end()) {
    layerParents.erase(it);
  }

  for (auto entity : _sortedElements) {
    GlComposite *composite = dynamic_cast<GlComposite *>(entity);

    if (composite) {
      composite->removeLayerParent(layer);
    }
  }
}
//============================================================
void GlComposite::reset(bool deleteElems) {
  vector<GlSimpleEntity *> toTreat;

  for (const auto &it : elements) {
    // Push elements to treat in a vector as deleting elements in the loop
    // invalidate the current iterator.
    toTreat.push_back(it.second);
  }

  for (auto entity : toTreat) {
    for (auto l : layerParents) {
      if (l->getScene())
        l->getScene()->notifyDeletedEntity(entity);
    }

    entity->removeParent(this);

    for (auto l : layerParents) {
      GlComposite *composite = dynamic_cast<GlComposite *>(entity);

      if (composite)
        composite->removeLayerParent(l);
    }

    if (deleteElems)
      delete entity;
  }

  elements.clear();
  _sortedElements.clear();

  for (auto l : layerParents) {
    if (l->getScene())
      l->getScene()->notifyModifyLayer(l->getName(), l);
  }
}
//============================================================
void GlComposite::addGlEntity(GlSimpleEntity *entity, const string &key) {
  assert(entity != nullptr);

  bool doSceneTreatment = false;

  if (elements.find(key) == elements.end()) {
    elements[key] = entity;
    _sortedElements.push_back(entity);
    doSceneTreatment = true;
  } else {
    if (elements[key] != entity) {
      _sortedElements.remove(elements[key]);
      _sortedElements.push_back(entity);
      elements[key] = entity;
      doSceneTreatment = true;
    }
  }

  if (doSceneTreatment) {
    entity->addParent(this);

    GlComposite *composite;
    composite = dynamic_cast<GlComposite *>(entity);

    for (auto l : layerParents) {
      if (composite)
        composite->addLayerParent(l);

      if (l->getScene())
        l->getScene()->notifyModifyLayer(l->getName(), l);
    }
  }

  GlGraphComposite *graphComposite = dynamic_cast<GlGraphComposite *>(entity);

  if (graphComposite) {
    for (auto l : layerParents) {
      l->glGraphCompositeAdded(graphComposite);
    }
  }
}
//============================================================
void GlComposite::deleteGlEntity(const string &key, bool informTheEntity) {
  if (elements.count(key) == 0)
    return;

  GlSimpleEntity *entity = elements[key];

  if (informTheEntity)
    entity->removeParent(this);

  if (informTheEntity) {
    GlComposite *composite = dynamic_cast<GlComposite *>(entity);

    if (composite) {
      for (auto l : layerParents) {
        composite->removeLayerParent(l);
      }
    }
  }

  GlGraphComposite *glGraphComposite = dynamic_cast<GlGraphComposite *>(entity);

  if (glGraphComposite) {
    for (auto l : layerParents) {
      l->glGraphCompositeRemoved(glGraphComposite);
    }
  }

  _sortedElements.remove(elements[key]);
  elements.erase(key);

  for (auto l : layerParents) {
    if (l->getScene()) {
      l->getScene()->notifyModifyLayer(l->getName(), l);
      l->getScene()->notifyDeletedEntity(entity);
    }
  }
}
//============================================================
void GlComposite::deleteGlEntity(GlSimpleEntity *entity, bool informTheEntity) {
  for (const auto &it : elements) {
    if (entity == it.second) {

      if (informTheEntity) {
        entity->removeParent(this);

        GlComposite *composite = dynamic_cast<GlComposite *>(entity);

        if (composite) {
          for (auto l : layerParents) {
            composite->removeLayerParent(l);
          }
        }
      }

      _sortedElements.remove(it.second);
      elements.erase(it.first);

      for (auto l : layerParents) {
        if (l->getScene()) {
          l->getScene()->notifyModifyLayer(l->getName(), l);
          l->getScene()->notifyDeletedEntity(entity);
        }
      }

      return;
    }
  }
}
//============================================================
string GlComposite::findKey(GlSimpleEntity *entity) {
  for (const auto &it : elements) {
    if (entity == it.second) {
      return it.first;
    }
  }

  return "";
}
//============================================================
GlSimpleEntity *GlComposite::findGlEntity(const string &key) {
  auto ite = elements.find(key);

  if (ite == elements.end())
    return nullptr;

  return ite->second;
}
//============================================================
void GlComposite::translate(const Coord &move) {
  for (const auto &it : elements) {
    it.second->translate(move);
  }
}
//============================================================
void GlComposite::notifyModified(GlSimpleEntity *entity) {
  if (!layerParents.empty())
    layerParents[0]->getScene()->notifyModifyEntity(entity);
}
//============================================================
void GlComposite::getXML(string &outString) {
  string name;

  GlXMLTools::createProperty(outString, "type", "GlComposite", "GlEntity");

  GlXMLTools::beginChildNode(outString);

  for (auto entity : _sortedElements) {
    name = findKey(entity);
    GlXMLTools::beginChildNode(outString, "GlEntity");
    GlXMLTools::createProperty(outString, "name", name);
    GlXMLTools::beginDataNode(outString);
    GlXMLTools::getXML(outString, "visible", entity->isVisible());
    GlXMLTools::getXML(outString, "stencil", entity->getStencil());
    entity->getXML(outString);
    GlXMLTools::endDataNode(outString);
    GlXMLTools::endChildNode(outString, "GlEntity");
  }

  GlXMLTools::endChildNode(outString);
}
//============================================================
void GlComposite::setWithXML(const string &inString, unsigned int &currentPosition) {

  string childName = GlXMLTools::enterChildNode(inString, currentPosition);
  assert(childName == "children");

  childName = GlXMLTools::enterChildNode(inString, currentPosition);

  while (!childName.empty()) {

    map<string, string> properties = GlXMLTools::getProperties(inString, currentPosition);

    assert(properties.count("name") != 0);
    assert(properties.count("type") != 0);

    GlSimpleEntity *entity = GlXMLTools::createEntity(properties["type"]);

    if (entity) {
      bool visible;
      int stencil;

      GlXMLTools::enterDataNode(inString, currentPosition);

      GlXMLTools::setWithXML(inString, currentPosition, "visible", visible);
      GlXMLTools::setWithXML(inString, currentPosition, "stencil", stencil);
      entity->setWithXML(inString, currentPosition);
      entity->setVisible(visible);
      entity->setStencil(stencil);
      addGlEntity(entity, properties["name"]);

      GlXMLTools::leaveDataNode(inString, currentPosition);
    }

    GlXMLTools::leaveChildNode(inString, currentPosition, childName);
    childName = GlXMLTools::enterChildNode(inString, currentPosition);
  }

  GlXMLTools::leaveChildNode(inString, currentPosition, "children");
}
}
