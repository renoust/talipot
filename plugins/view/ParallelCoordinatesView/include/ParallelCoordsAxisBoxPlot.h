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

#ifndef PARALLELCOORDSAXISBOXPLOT_H_
#define PARALLELCOORDSAXISBOXPLOT_H_

#include <string>

#include <tulip/GlSimpleEntity.h>
#include <tulip/GLInteractor.h>
#include <tulip/Color.h>

namespace tlp {

class QuantitativeParallelAxis;

class GlAxisBoxPlot : public GlSimpleEntity {

public:
  GlAxisBoxPlot(QuantitativeParallelAxis *axis, const Color &fillColor, const Color &outlineColor);
  ~GlAxisBoxPlot() override {}

  void draw(float lod, Camera *camera) override;

  void getXML(std::string &) override {}

  void setWithXML(const std::string &, unsigned int &) override {}

  void setHighlightRangeIfAny(Coord sceneCoords);

private:
  void drawLabel(const Coord &position, const std::string &labelName, Camera *camera);

  QuantitativeParallelAxis *axis;
  Coord bottomOutlierCoord;
  Coord firstQuartileCoord;
  Coord medianCoord;
  Coord thirdQuartileCoord;
  Coord topOutlierCoord;
  float boxWidth;
  Color fillColor, outlineColor;
  Coord *highlightRangeLowBound;
  Coord *highlightRangeHighBound;
};

class ParallelAxis;
class ParallelCoordinatesView;

class ParallelCoordsAxisBoxPlot : public GLInteractorComponent {

public:
  ParallelCoordsAxisBoxPlot();
  ~ParallelCoordsAxisBoxPlot() override;
  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *glMainWidget) override;
  bool compute(GlMainWidget *glMainWidget) override;
  void viewChanged(View *view) override;

private:
  void buildGlAxisPlot(std::vector<ParallelAxis *> currentAxis);
  void deleteGlAxisPlot();

  void initOrUpdateBoxPlots();

  ParallelCoordinatesView *parallelView;
  Graph *currentGraph;
  std::map<QuantitativeParallelAxis *, GlAxisBoxPlot *> axisBoxPlotMap;
  ParallelAxis *selectedAxis;
  unsigned int lastNbAxis;
};
} // namespace tlp

#endif /* PARALLELCOORDSBOXPLOT_H_ */
