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
#include <tulip/GlSimpleEntity.h>

#include <tulip/GlComposite.h>

using namespace std;

namespace tlp {
GlSimpleEntity::~GlSimpleEntity() {
  for (std::vector<GlComposite *>::iterator it = parents.begin(); it != parents.end(); ++it) {
    (*it)->deleteGlEntity(this, false);
  }
}

void GlSimpleEntity::addParent(GlComposite *composite) {
  parents.push_back(composite);
}

void GlSimpleEntity::setVisible(bool visible) {
  if (this->visible == visible)
    return;

  this->visible = visible;

  for (vector<GlComposite *>::iterator it = parents.begin(); it != parents.end(); ++it) {
    (*it)->notifyModified(this);
  }
}
void GlSimpleEntity::removeParent(GlComposite *composite) {
  for (std::vector<GlComposite *>::iterator it = parents.begin(); it != parents.end(); ++it) {
    if ((*it) == composite) {
      parents.erase(it);
      return;
    }
  }
}
} // namespace tlp
