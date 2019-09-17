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

#ifndef PARALLELCOORDSELEMENTSHOWINFO_H_
#define PARALLELCOORDSELEMENTSHOWINFO_H_

#include <tulip/MouseShowElementInfo.h>

namespace tlp {

class ParallelCoordsElementShowInfo : public MouseShowElementInfo {
public:
  bool pick(int x, int y, SelectedEntity &selectedEntity) override;
};
} // namespace tlp
#endif /* PARALLELCOORDSELEMENTSHOWINFO_H_ */
