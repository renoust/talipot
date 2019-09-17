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
#ifndef RECTANGLEPACKINGFONCTIONS_H
#define RECTANGLEPACKINGFONCTIONS_H

#include <vector>
#include <tulip/PluginProgress.h>
#include "RectanglePacking.h"

/**
 *This function is used for packing rectangles. It is used when we want to
 *reduce the complexity restricting the number of rectangles packed in an
 *optimal way.
 *@param v a vector including the rectangles to pack.
 *@param quality give the complexity of the algorithm.
 *@param progress pointer on an instance of the PluginProgress class.
 *@return true if the computation has not stopped due to the PluginProgress.
 */
bool RectanglePackingLimitRectangles(std::vector<tlp::Rectangle<float>> &v, const char *quality,
                                     tlp::PluginProgress *progress);

/**
 *This function is used for packing rectangles. It is used when we want to
 *reduce the complexity restricting the number of tested positions for each
 *rectangle to pack.
 *@param v a vector including the rectangles to pack.
 *@param quality give the complexity of the algorithm.
 *@param progress pointer on an instance of the PluginProgress class.
 *@return true if the computation has not stopped due to the PluginProgress.
 */
bool RectanglePackingLimitPositions(std::vector<tlp::Rectangle<float>> &v, const char *quality,
                                    tlp::PluginProgress *progress);

#endif
