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

#ifndef SCATTER_PLOT2D_VIEW_NAVIGATOR_H
#define SCATTER_PLOT2D_VIEW_NAVIGATOR_H

#include <talipot/GLInteractor.h>
#include <talipot/Coord.h>

namespace tlp {

class ScatterPlot2DView;
class ScatterPlot2D;

class ScatterPlot2DViewNavigator : public GLInteractorComponent {

public:
  ScatterPlot2DViewNavigator();
  ~ScatterPlot2DViewNavigator() override;

  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *) override {
    return false;
  }
  bool compute(GlMainWidget *) override {
    return false;
  }
  void viewChanged(View *view) override;

private:
  ScatterPlot2D *getOverviewUnderPointer(const Coord &sceneCoord);

  ScatterPlot2DView *scatterPlot2dView;
  ScatterPlot2D *selectedScatterPlotOverview;

  GlMainWidget *glWidget;
};
} // namespace tlp

#endif // SCATTER_PLOT2D_VIEW_NAVIGATOR_H
