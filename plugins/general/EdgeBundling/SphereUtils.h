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

#ifndef SPHERE_UTILS_H
#define SPHERE_UTILS_H

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>

float centerOnOriginAndScale(tlp::Graph *graph, tlp::LayoutProperty *layout, float dist);

void addSphereGraph(tlp::Graph *graph, double radius);

void moveBendsToSphere(tlp::Graph *graph, float ray, tlp::LayoutProperty *layout);

#endif // SPHERE_UTILS_H
