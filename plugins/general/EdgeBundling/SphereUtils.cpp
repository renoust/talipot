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

#include "SphereUtils.h"

#include <talipot/DrawingTools.h>
#include <talipot/SizeProperty.h>
#include <talipot/DoubleProperty.h>

using namespace tlp;
using namespace std;

float centerOnOriginAndScale(Graph *graph, LayoutProperty *layout, float dist) {
  graph->getProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(0, 0, 0));
  BoundingBox bb =
      tlp::computeBoundingBox(graph, graph->getProperty<LayoutProperty>("viewLayout"),
                              graph->getProperty<SizeProperty>("viewSize"),
                              graph->getProperty<DoubleProperty>("viewRotation"), nullptr);
  Coord move_coord = Coord((bb[0] + bb[1])) / (-2.f);
  layout->translate(move_coord, graph);
  float ray = (move_coord - bb[1]).norm();
  float scaleFactor = dist / ray;
  layout->scale(Coord(scaleFactor, scaleFactor, scaleFactor), graph);
  graph->getProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(0.1f, 0.1f, 0.1f));
  return sqrt(ray * ray / 2.);
}

void moveBendsToSphere(Graph *graph, float ray, LayoutProperty *layout) {
  for (auto e : graph->edges()) {
    vector<Coord> bends;
    bends = layout->getEdgeValue(e);

    for (size_t i = 0; i < bends.size(); ++i) {
      Coord c = bends[i];
      double dist = c.norm();
      c /= dist;
      c *= ray;
      bends[i] = c;
    }

    layout->setEdgeValue(e, bends);
  }

  for (auto n : graph->nodes()) {
    Coord c = layout->getNodeValue(n);
    c /= c.norm();
    c *= ray;
    layout->setNodeValue(n, c);
  }
}

static Coord getCoordFromPolar(double rayon, double a1, double a2) {
  a1 = a1 * M_PI / 2. / 90.;
  a2 = a2 * M_PI / 2. / 90.;
  float x, y, z;
  x = rayon * cos(a1) * sin(a2);
  y = rayon * sin(a1) * sin(a2);
  z = rayon * cos(a2);
  return Coord(x, y, z);
}

void addSphereGraph(Graph *graph, double radius) {
  LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
  double rho = 0;

  while (rho < 360) {
    double teta = 5;

    while (teta < 180) {
      node n = graph->addNode();
      Coord c = getCoordFromPolar(radius, rho, teta);
      layout->setNodeValue(n, c);
      teta += 5.;
    }

    rho += 5.;
  }

  node n = graph->addNode();
  Coord c = getCoordFromPolar(radius, 0, 0);
  layout->setNodeValue(n, c);
  n = graph->addNode();
  c = getCoordFromPolar(radius, 0, 180);
  layout->setNodeValue(n, c);
}
