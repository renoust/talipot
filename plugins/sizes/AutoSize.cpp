/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <tulip/TulipPluginHeaders.h>
#include <cmath>

using namespace tlp;

/** \addtogroup size */

/// AutoSize.cpp - Compute size in order to prevent node-node overlapping
/**
 * This plugin compute the size of nodes and edges such that node-node overlapping does not exist
 * (if it is possible).
 * and edge sizes are proportional to node sizes.
 *
 *  \author David Auber Bordeaux University France: Email:auber@labri.fr
 */
class AutoSize : public SizeAlgorithm {
public:
  PLUGININFORMATION("Auto Sizing", "Auber", "04/05/2001",
                    "Resize the nodes and edges of a graph so that the graph gets easy to read. "
                    "The size of a node will depend on the number of its sons.",
                    "1.0", "Size")
  AutoSize(const tlp::PluginContext *context) : SizeAlgorithm(context) {}

  bool run() override {

    for (const node &n : graph->nodes())
      result->setNodeValue(n, getNodeValue(n));

    for (const edge &e : graph->edges())
      result->setEdgeValue(e, getEdgeValue(e));
    return true;
  }

private:
  Size getNodeValue(const node n) {
    LayoutProperty *entryLayout = graph->getProperty<LayoutProperty>("viewLayout");

    // Compute the minimal distance to one neighbour.
    Iterator<node> *itN = graph->getNodes();
    const Coord &tmp1 = entryLayout->getNodeValue(n);
    double dist = 1000;

    if (itN->hasNext()) {
      node itn = itN->next();

      while ((itn == n) && itN->hasNext())
        itn = itN->next();

      if (itn != n) {
        const Coord &tmp2 = entryLayout->getNodeValue(itn);
        dist = sqrt((tmp1.getX() - tmp2.getX()) * (tmp1.getX() - tmp2.getX()) +
                    (tmp1.getY() - tmp2.getY()) * (tmp1.getY() - tmp2.getY()) +
                    (tmp1.getZ() - tmp2.getZ()) * (tmp1.getZ() - tmp2.getZ()));
      } else {
        dist = 10;
      }
    }

    for (; itN->hasNext();) {
      node itn = itN->next();

      if (itn != n) {
        const Coord &tmp2 = entryLayout->getNodeValue(itn);
        double tmpDist = sqrt((tmp1.getX() - tmp2.getX()) * (tmp1.getX() - tmp2.getX()) +
                              (tmp1.getY() - tmp2.getY()) * (tmp1.getY() - tmp2.getY()) +
                              (tmp1.getZ() - tmp2.getZ()) * (tmp1.getZ() - tmp2.getZ()));

        if (tmpDist < dist)
          dist = tmpDist;
      }
    }

    delete itN;
    return Size(dist / 2, dist / 2, dist / 2);
  }

  Size getEdgeValue(const edge e) {
    Size s = result->getNodeValue(graph->source(e));
    Size t = result->getNodeValue(graph->target(e));
    Coord tmp(s.getW(), s.getH(), s.getD());
    Coord tmp2(t.getW(), t.getH(), t.getD());
    float sizes = tmp.norm();
    float sizet = tmp2.norm();
    return (Size(sizes / 16, sizet / 16, sizet / 4));
  }
};

PLUGIN(AutoSize)
