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

#include "NominalParallelAxis.h"
#include "AxisConfigDialogs.h"
#include "ParallelCoordinatesGraphProxy.h"
#include "ParallelTools.h"

#include <tulip/GlNominativeAxis.h>
#include <tulip/StringProperty.h>
#include <tulip/Iterator.h>

using namespace std;

namespace tlp {

NominalParallelAxis::NominalParallelAxis(const Coord &base_coord, const float height,
                                         const float axisAreaWidth,
                                         ParallelCoordinatesGraphProxy *graph,
                                         const std::string &propertyName, const Color &axisColor,
                                         const float rotationAngle,
                                         const GlAxis::CaptionLabelPosition captionPosition)
    : ParallelAxis(
          new GlNominativeAxis(propertyName, base_coord, height, GlAxis::VERTICAL_AXIS, axisColor),
          axisAreaWidth, rotationAngle, captionPosition),
      graphProxy(graph) {
  glNominativeAxis = static_cast<GlNominativeAxis *>(glAxis);
  setLabels();
  ParallelAxis::redraw();
}

void NominalParallelAxis::setLabels() {

  vector<string> labels;

  for (unsigned int dataId : graphProxy->getDataIterator()) {
    string labelName =
        graphProxy->getPropertyValueForData<StringProperty, StringType>(getAxisName(), dataId);

    if (std::find(labels.begin(), labels.end(), labelName) == labels.end()) {
      labels.push_back(labelName);
    }
  }

  if (labelsOrder.empty() || (labelsOrder.size() != labels.size())) {
    labelsOrder = labels;
  }

  glNominativeAxis->setAxisGraduationsLabels(labelsOrder, GlAxis::RIGHT_OR_ABOVE);
}

Coord NominalParallelAxis::getPointCoordOnAxisForData(const unsigned int dataIdx) {
  string propertyValue(
      graphProxy->getPropertyValueForData<StringProperty, StringType>(getAxisName(), dataIdx));
  Coord axisPointCoord(glNominativeAxis->getAxisPointCoordForValue(propertyValue));

  if (rotationAngle != 0.0f) {
    rotateVector(axisPointCoord, rotationAngle, Z_ROT);
  }

  return axisPointCoord;
}

void NominalParallelAxis::showConfigDialog() {
  NominalAxisConfigDialog dialog(this);
  dialog.exec();
}

const set<unsigned int> &NominalParallelAxis::getDataInSlidersRange() {

  dataSubset.clear();
  map<string, unsigned int> labelsInRange;
  vector<string>::iterator it;

  for (it = labelsOrder.begin(); it != labelsOrder.end(); ++it) {
    Coord labelCoord = glNominativeAxis->getAxisPointCoordForValue(*it);

    if (labelCoord.getY() >= bottomSliderCoord.getY() &&
        labelCoord.getY() <= topSliderCoord.getY()) {
      labelsInRange[*it] = 1;
    }
  }

  for (unsigned int dataId : graphProxy->getDataIterator()) {
    string labelName =
        graphProxy->getPropertyValueForData<StringProperty, StringType>(getAxisName(), dataId);

    if (labelsInRange.find(labelName) != labelsInRange.end()) {
      dataSubset.insert(dataId);
    }
  }

  return dataSubset;
}

void NominalParallelAxis::updateSlidersWithDataSubset(const set<unsigned int> &dataSubset) {
  float rotAngleBak = rotationAngle;
  rotationAngle = 0.0f;
  set<unsigned int>::iterator it;
  Coord max(getBaseCoord());
  Coord min(getBaseCoord() + Coord(0.0f, getAxisHeight()));

  for (it = dataSubset.begin(); it != dataSubset.end(); ++it) {
    Coord labelCoord(getPointCoordOnAxisForData(*it));

    if (labelCoord.getY() < min.getY()) {
      min = labelCoord;
    }

    if (labelCoord.getY() > max.getY()) {
      max = labelCoord;
    }
  }

  bottomSliderCoord = min;
  topSliderCoord = max;
  rotationAngle = rotAngleBak;
}

void NominalParallelAxis::redraw() {
  setLabels();
  ParallelAxis::redraw();
}
} // namespace tlp
