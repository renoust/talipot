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

#ifndef HISTOGRAM_METRIC_MAPPING_H
#define HISTOGRAM_METRIC_MAPPING_H

#include <QCursor>
#include <QMouseEvent>

#include <talipot/config.h>
#include <talipot/GLInteractor.h>
#include <talipot/TlpTools.h>
#include <talipot/GlSimpleEntity.h>
#include <talipot/GlCircle.h>
#include <talipot/GlLabel.h>
#include <talipot/GlPolyQuad.h>
#include <talipot/ColorScale.h>
#include <talipot/GlColorScale.h>
#include <talipot/ColorScaleConfigDialog.h>

#include <map>

#include "HistogramView.h"
#include "SizeScaleConfigDialog.h"
#include "GlyphScaleConfigDialog.h"

namespace tlp {

class GlEditableCurve : public GlSimpleEntity {

public:
  GlEditableCurve(const Coord &startPoint, const Coord &endPoint, const Color &curveColor);
  GlEditableCurve(const GlEditableCurve &curve);
  ~GlEditableCurve() override {}

  void setXAxisScale(GlQuantitativeAxis *xAxis) {
    this->xAxis = xAxis;
  }
  bool pointBelong(const Coord &point);
  void addCurveAnchor(const Coord &point);
  Coord *getCurveAnchorAtPointIfAny(const Coord &point, Camera *camera);
  void removeCurveAnchor(const Coord &curveAnchor);
  Coord translateCurveAnchorToPoint(const Coord &curveAnchor, const Coord &targetPoint);
  float getYCoordForX(const float xCoord);
  void updateSize(const Coord &newMinPoint, const Coord &newMaxPoint);
  void resetCurve();

  void draw(float lod, Camera *camera) override;
  void getXML(std::string &) override {}
  void setWithXML(const std::string &, unsigned int &) override {}

  Coord getMinPoint() const {
    return minPoint;
  }
  Coord getMaxPoint() const {
    return maxPoint;
  }

  Coord getFirstCurvePoint() const {
    return startPoint;
  }
  void setCurveStartPoint(const Coord &startPoint) {
    this->startPoint = startPoint;
  }

  Coord getLastCurvePoint() const {
    return endPoint;
  }
  void setLastCurvePoint(const Coord &endPoint) {
    this->endPoint = endPoint;
  }

  std::vector<Coord> getCurvePoints() const {
    return curvePoints;
  }
  void setCurvePoints(const std::vector<Coord> &curvePoints) {
    this->curvePoints = curvePoints;
  }

  Color getCurveColor() const {
    return curveColor;
  }
  void setCurveColor(const Color &color) {
    curveColor = color;
  }

private:
  void init();

  Coord startPoint, endPoint;
  Coord minPoint, maxPoint;
  std::vector<Coord> curvePoints;
  Color curveColor;
  GlCircle basicCircle;
  GlQuantitativeAxis *xAxis;
};

class GlSizeScale : public GlSimpleEntity {

public:
  enum Orientation { Horizontal, Vertical };

  GlSizeScale(const float minSize, const float maxSize, const Coord &baseCoord, const float length,
              const float thickness, const Color &color, Orientation orientation);
  ~GlSizeScale() override;

  float getSizeAtPos(const Coord &pos);

  void draw(float lod, Camera *camera) override;

  void translate(const Coord &move) override;

  void getXML(std::string &) override {}

  void setWithXML(const std::string &, unsigned int &) override {}

  Coord getBaseCoord() const {
    return baseCoord;
  }

  float getThickness() const {
    return thickness;
  }

  float getLength() const {
    return length;
  }

  float getMinSize() const {
    return minSize;
  }
  float getMaxSize() const {
    return maxSize;
  }

  void setMinSize(const float minSize) {
    this->minSize = minSize;
  }
  void setMaxSize(const float maxSize) {
    this->maxSize = maxSize;
  }

  void setColor(const Color &color) {
    this->color = color;
  }

private:
  float minSize, maxSize;
  Coord baseCoord;
  float length, thickness;
  Color color;
  Orientation orientation;
  GlPolyQuad *polyquad;
  GlLabel *minLabel, *maxLabel;
};

class GlGlyphScale : public GlSimpleEntity {

public:
  enum Orientation { Horizontal, Vertical };

  GlGlyphScale(const Coord &baseCoord, const float length, Orientation orientation);
  ~GlGlyphScale() override;

  void setGlyphsList(const std::vector<int> &glyphsList);

  int getGlyphAtPos(const Coord &pos);

  void draw(float lod, Camera *camera) override;

  void translate(const Coord &move) override;

  void getXML(std::string &) override {}

  void setWithXML(const std::string &, unsigned int &) override {}

  Coord getBaseCoord() const {
    return baseCoord;
  }

  float getLength() const {
    return length;
  }

private:
  GlGraphRenderingParameters glyphGraphRenderingParameters;
  Graph *glyphGraph;
  LayoutProperty *glyphGraphLayout;
  SizeProperty *glyphGraphSize;
  ColorProperty *glyphGraphColor;
  IntegerProperty *glyphGraphShape;
  GlGraphInputData *glyphGraphInputData;
  Coord baseCoord;
  float length;
  Orientation orientation;
  std::map<std::pair<float, float>, int> glyphScaleMap;
  float size;
};

class HistogramMetricMapping : public GLInteractorComponent {

  enum MappingType { VIEWCOLOR_MAPPING, VIEWBORDERCOLOR_MAPPING, SIZE_MAPPING, GLYPH_MAPPING };

public:
  HistogramMetricMapping();
  HistogramMetricMapping(const HistogramMetricMapping &histogramMetricMapping);
  ~HistogramMetricMapping() override;

  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *glMainWidget) override;
  bool compute(GlMainWidget *glMainWidget) override;
  void viewChanged(View *view) override;

  bool pointerUnderScale(const Coord &sceneCoords);
  void updateGraphWithMapping(Graph *graph, LayoutProperty *histogramLayout);
  void updateMapping(GlQuantitativeAxis *histoXAxis, unsigned int nbHistogramBins);

private:
  void initInteractor();

protected:
  HistogramView *histoView;
  GlEditableCurve *curve;
  bool curveDragStarted;
  Coord *selectedAnchor;
  ColorScale *colorScale, dialogColorScale;
  GlColorScale *glColorScale;
  GlSizeScale *glSizeScale;
  GlGlyphScale *glGlyphScale;
  ColorScaleConfigDialog *colorScaleConfigDialog;
  SizeScaleConfigDialog *sizeScaleConfigDialog;
  GlyphScaleConfigDialog *glyphScaleConfigDialog;
  float lastXAxisLength;
  GlQuantitativeAxis *histoXAxis;
  GlPolyQuad *mappinqPolyQuad;
  float scaleAxisOffset;

  Graph *glyphMappingGraph;
  GlGraphRenderingParameters glyphMapppingGraphRenderingParameters;
  GlGraphInputData *glyphMappingGraphInputData;

  MappingType mappingType;

  QMenu *popupMenu;
  QMenu *colorMappingMenu;
  QAction *viewColorMappingAction;
  QAction *viewBorderColorMappingAction;
  QAction *sizeMapping;
  QAction *glyphMapping;

  std::map<MappingType, std::vector<Coord>> curveShapeForMapping;
};
}

#endif // HISTOGRAM_METRIC_MAPPING_H
