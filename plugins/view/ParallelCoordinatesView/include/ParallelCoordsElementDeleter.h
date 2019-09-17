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

#ifndef PARALLELCOORDSELEMENTDELETER_H_
#define PARALLELCOORDSELEMENTDELETER_H_

#include <tulip/GLInteractor.h>

namespace tlp {

class ParallelCoordsElementDeleter : public GLInteractorComponent {
public:
  ParallelCoordsElementDeleter() {}
  ~ParallelCoordsElementDeleter() override {}
  bool eventFilter(QObject *, QEvent *) override;
};
} // namespace tlp

#endif /* PARALLELCOORDSELEMENTDELETER_H_ */
