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

#ifndef Tulip_GLENTITY_H
#define Tulip_GLENTITY_H

#include <tulip/tulipconf.h>

namespace tlp {

class GlSceneVisitor;
class GlLayout;

/**
 * Abstract class used to represent all entity of a scene
 */
class TLP_GL_SCOPE GlEntity {

public:
  virtual ~GlEntity() {}

  /**
   * Accpet visitor function
   */
  virtual void acceptVisitor(GlSceneVisitor *visitor) = 0;
};
} // namespace tlp

#endif // Tulip_GLENTITY_H
///@endcond
