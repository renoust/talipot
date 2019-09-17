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

#ifndef Tulip_GLDRAWSCENEVISITOR_H
#define Tulip_GLDRAWSCENEVISITOR_H

#include <unordered_map>
#include <tulip/GlSceneVisitor.h>

namespace tlp {

class GlGraphInputData;

class TLP_GL_SCOPE GlDrawSceneVisitor : public GlSceneVisitor {

public:
  GlDrawSceneVisitor(std::unordered_map<unsigned int, float> *lod, GlGraphInputData *inputData)
      : inputData(inputData) {
    lodMap = lod;
  }

  virtual void visit(GlSimpleEntity *entity);
  virtual void visit(GlComplexeEntity *entity);
  virtual void visit(GlLayout *layout);

private:
  std::unordered_map<unsigned int, float> *lodMap;
  GlGraphInputData *inputData;
};
} // namespace tlp

#endif // Tulip_GLDRAWSCENEVISITOR_H
///@endcond
