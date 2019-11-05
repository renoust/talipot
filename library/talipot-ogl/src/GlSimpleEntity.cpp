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

#include <talipot/GlSimpleEntity.h>

#include <talipot/GlComposite.h>

#include <algorithm>

using namespace std;

namespace tlp {
GlSimpleEntity::~GlSimpleEntity() {
  for (auto parent : parents) {
    parent->deleteGlEntity(this, false);
  }
}

void GlSimpleEntity::addParent(GlComposite *composite) {
  parents.push_back(composite);
}

void GlSimpleEntity::setVisible(bool visible) {
  if (this->visible == visible)
    return;

  this->visible = visible;

  for (auto parent : parents) {
    parent->notifyModified(this);
  }
}

void GlSimpleEntity::removeParent(GlComposite *composite) {
  auto it = find(parents.begin(), parents.end(), composite);

  if (it != parents.end()) {
    parents.erase(it);
  }
}

}
