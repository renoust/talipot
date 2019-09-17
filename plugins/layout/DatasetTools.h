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
#ifndef DATASETTOOLS_H
#define DATASETTOOLS_H

#include <tulip/TulipPluginHeaders.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include "Orientation.h"

void addOrientationParameters(tlp::LayoutAlgorithm *pLayout);
tlp::DataSet setOrientationParameters(int orientation);
void addOrthogonalParameters(tlp::LayoutAlgorithm *pLayout);
void addSpacingParameters(tlp::LayoutAlgorithm *pLayout);
void getSpacingParameters(tlp::DataSet *dataSet, float &nodeSpacing, float &layerSpacing);
void addNodeSizePropertyParameter(tlp::LayoutAlgorithm *pLayout, bool inout = false);
bool getNodeSizePropertyParameter(tlp::DataSet *dataSet, tlp::SizeProperty *&sizes);

orientationType getMask(tlp::DataSet *dataSet);
bool hasOrthogonalEdge(tlp::DataSet *dataSet);

#endif
