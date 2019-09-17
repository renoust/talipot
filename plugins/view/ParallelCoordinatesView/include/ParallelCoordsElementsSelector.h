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

#ifndef PARALLELCOORDSELEMENTSSELECTOR_H_
#define PARALLELCOORDSELEMENTSSELECTOR_H_

#include "ParallelCoordsGlEntitiesSelector.h"

namespace tlp {

class ParallelCoordsElementsSelector : public ParallelCoordsGlEntitiesSelector {
public:
  ParallelCoordsElementsSelector(Qt::MouseButton button = Qt::LeftButton,
                                 Qt::KeyboardModifier modifier = Qt::NoModifier)
      : ParallelCoordsGlEntitiesSelector(button, modifier) {}
  ~ParallelCoordsElementsSelector() override {}
  void unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView) override;
  void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y,
                               bool addToSelection) override;
  void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h,
                               bool addToSelection) override;
};
} // namespace tlp

#endif /* PARALLELCOORDSELEMNTSSELECTOR_H_ */
