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

#ifndef TALIPOT_GL_COMPOSITE_H
#define TALIPOT_GL_COMPOSITE_H

#include <map>
#include <list>
#include <string>

#include <talipot/GlSimpleEntity.h>
#include <talipot/config.h>

namespace tlp {

/**
 * @ingroup OpenGL
 * \brief GlSimpleEntity used to aggregate other GlEntity
 *
 * This class provide basic container to manage other GlEntity
 * @see GlSimpleEntity
 */
class TLP_GL_SCOPE GlComposite : public GlSimpleEntity {

public:
  /**
   * @brief Constructor
   * @param deleteComponentsInDestructor if true : call delete on components when the GlComposite is
   * delete
   */
  GlComposite(bool deleteComponentsInDestructor = true);

  /**
   * @brief Destructor
   */
  ~GlComposite() override;

  /**
   * @brief Clear the composite
   *
   * If deleteElems is true, composite's entities are delete
   */
  void reset(bool deleteElems);
  /**
   * @brief Add new entity with name : key.
   *
   * The composite does not takes the entity's ownership, i.e. it is not its responsibility to
   * delete it.
   */
  void addGlEntity(GlSimpleEntity *entity, const std::string &key);
  /**
   * @brief Remove entity with name : key
   *
   * The entity is not deleted
   */
  void deleteGlEntity(const std::string &key, bool informTheEntity = true);
  /**
   * @brief Remove given entity
   *
   * The entity is not deleted
   */
  void deleteGlEntity(GlSimpleEntity *entity, bool informTheEntity = true);
  /**
   * @brief Find name of given entity
   */
  std::string findKey(GlSimpleEntity *entity);
  /**
   * @brief Find entity with name : key
   */
  GlSimpleEntity *findGlEntity(const std::string &key);
  /**
   * @brief Return map of entities in composite
   */
  const std::map<std::string, GlSimpleEntity *> &getGlEntities() const {
    return elements;
  }

  /**
   * @brief Set stencil number for all composite's children
   *
   * For more information on stencil  :
   * @see GlSimpleEntity
   */
  void setStencil(int stencil) override {
    this->stencil = stencil;

    for (std::list<GlSimpleEntity *>::iterator it = _sortedElements.begin();
         it != _sortedElements.end(); ++it) {
      (*it)->setStencil(stencil);
    }
  }

  /**
   * @brief Set if at the destruction of composite, components well be deleted
   */
  void setDeleteComponentsInDestructor(bool deleteComponentsInDestructor) {
    this->deleteComponentsInDestructor = deleteComponentsInDestructor;
  }

  /**
   * @brief translate the composite with children
   */
  void translate(const Coord &mouvement) override;

  /**
   * @brief Function to export data in outString (in XML format)
   */
  void getXML(std::string &outString) override;

  /**
   * @brief Function to set data with inString (in XML format)
   */
  void setWithXML(const std::string &inString, unsigned int &currentPosition) override;

  ///@cond DOXYGEN_HIDDEN

  /**
   * Function used to visit composite's children
   */
  void acceptVisitor(GlSceneVisitor *visitor) override {
    // visitor->visit(this);
    for (std::list<GlSimpleEntity *>::iterator it = _sortedElements.begin();
         it != _sortedElements.end(); ++it) {
      if ((*it)->isVisible()) {

#ifndef NDEBUG
        GlComposite *composite = dynamic_cast<GlComposite *>(*it);

        if (!composite && !(*it)->getBoundingBox().isValid()) {
          for (std::map<std::string, GlSimpleEntity *>::iterator itE = elements.begin();
               itE != elements.end(); ++itE) {
            if (itE->second == (*it)) {
              tlp::warning() << "Invalid bounding box for entity: " << itE->first << std::endl;
              assert(false);
            }
          }
        }

#endif

        (*it)->acceptVisitor(visitor);
      }
    }
  }

  /**
   * Add a layer parent to this entity
   */
  virtual void addLayerParent(GlLayer *layer);

  /**
   * Remove a layer parent to this entity
   */
  virtual void removeLayerParent(GlLayer *layer);

  /**
   * Call when a child of the composite is modified
   */
  void notifyModified(GlSimpleEntity *entity);

  /**
   * \attention This function do nothing, GlComposite is a GlSimpleEntity so draw function must be
   * define
   */
  void draw(float, Camera *) override {}

  ///@endcond

protected:
  std::map<std::string, GlSimpleEntity *> elements;
  std::list<GlSimpleEntity *>
      _sortedElements; // necessary to enable ordering of elements (for alpha blending)
  std::vector<GlLayer *> layerParents;
  bool deleteComponentsInDestructor;
};
} // namespace tlp
#endif // TALIPOT_GL_COMPOSITE_H
