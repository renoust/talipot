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

#ifndef HISTOGRAMVIEWNAVIGATOR_H_
#define HISTOGRAMVIEWNAVIGATOR_H_

#include <tulip/GLInteractor.h>

namespace tlp {

class Histogram;
class HistogramView;

class HistogramViewNavigator : public GLInteractorComponent {

public:
  HistogramViewNavigator();
  ~HistogramViewNavigator() override;

  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *) override {
    return false;
  }
  bool compute(GlMainWidget *) override {
    return false;
  }
  void viewChanged(View *view) override;

private:
  Histogram *getOverviewUnderPointer(const Coord &sceneCoord) const;

  HistogramView *histoView;
  Histogram *selectedHistoOverview;
};
} // namespace tlp

#endif /* HISTOGRAMVIEWNAVIGATOR_H_ */
