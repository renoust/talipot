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

#ifndef SCATTERPLOTTRENDLINE_H_
#define SCATTERPLOTTRENDLINE_H_

#include <tulip/GLInteractor.h>

namespace tlp {

class ScatterPlot2DView;

class ScatterPlotTrendLine : public GLInteractorComponent {

public:
  ScatterPlotTrendLine();
  ~ScatterPlotTrendLine() override;

  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *glMainWidget) override;
  bool compute(GlMainWidget *glMainWidget) override;
  void viewChanged(View *view) override;

private:
  ScatterPlot2DView *scatterView;
  float a, b;
};
} // namespace tlp

#endif /* SCATTERPLOTTRENDLINE_H_ */
