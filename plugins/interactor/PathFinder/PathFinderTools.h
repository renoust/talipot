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

#ifndef PATH_FINDER_TOOLS_H
#define PATH_FINDER_TOOLS_H

#include <talipot/Circle.h>
#include <talipot/Node.h>
#include <talipot/Edge.h>

namespace tlp {
class GlGraphInputData;
class SizeProperty;
class LayoutProperty;

/**
 * Computes the enclosing circle of a graph's node using the layout,size etc properties specified in
 * the input data.
 * @param circle The resulting circle.
 * @param inputData The graph's input data
 * @param n the node.
 * @return true if the circle has been successfully computed.
 */
bool getNodeEnclosingCircle(Circlef &circle, GlGraphInputData *inputData, node n);

/**
 * Computes the enclosing circle of a graph's edge bends using the layout,size etc properties
 * specified in the input data.
 * @param circle The resulting circle.
 * @param inputData The graph's input data
 * @param n the node.
 * @return true if the circle has been successfully computed.
 */
bool getEdgeEnclosingCircle(Circlef &circle, GlGraphInputData *inputData, edge n);
} // namespace tlp
#endif // PATH_FINDER_TOOLS_H
