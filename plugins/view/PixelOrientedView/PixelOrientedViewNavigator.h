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

#ifndef PIXELORIENTEDOVERVIEWSELECTOR_H_
#define PIXELORIENTEDOVERVIEWSELECTOR_H_

#include <tulip/GLInteractor.h>

#include "PixelOrientedOverview.h"
#include "PixelOrientedView.h"

namespace tlp {

class PixelOrientedViewNavigator : public GLInteractorComponent {

public:
  PixelOrientedViewNavigator();
  ~PixelOrientedViewNavigator() override;

  bool eventFilter(QObject *, QEvent *) override;
  void viewChanged(View *view) override;

private:
  PixelOrientedOverview *getOverviewUnderPointer(Coord &sceneCoord);

  PixelOrientedView *pixelView;
  PixelOrientedOverview *selectedOverview;
};
} // namespace tlp

#endif /* PIXELORIENTEDOVERVIEWSELECTOR_H_ */
