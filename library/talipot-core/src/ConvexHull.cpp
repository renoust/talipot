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

#include <talipot/ConvexHull.h>
#include <talipot/DrawingTools.h>

#include <list>
#include <map>
#include <algorithm>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

extern "C" {
#ifdef HAVE_REENTRANT_QHULL
#include <qhull_ra.h>
#else
#include <qhull_a.h>
#endif
}

using namespace std;
using namespace tlp;

//================================================================================

static bool runQHull(int dim, vector<double> &points, vector<vector<unsigned int>> &facets,
                     vector<vector<unsigned int>> &neighbors) {

  // Set default options for qhull convex hull
  // - Qt : triangulated output
  string qhullOptions = "Qt";
  // build qhull command
  string qhullCommand = "qhull ";
  qhullCommand += qhullOptions;

// run qhull convex hull computation
#ifdef HAVE_REENTRANT_QHULL
  qhT qh_qh;
  qhT *qh = &qh_qh;
  QHULL_LIB_CHECK
  qh_zero(qh, stderr);
  int qhullKo = qh_new_qhull(qh, dim, points.size() / dim, &points[0], false,
                             const_cast<char *>(qhullCommand.c_str()), nullptr, stderr);
#else
  int qhullKo = qh_new_qhull(dim, points.size() / dim, &points[0], false,
                             const_cast<char *>(qhullCommand.c_str()), nullptr, stderr);
#endif

  if (!qhullKo) {
    facetT *facet = nullptr;
    vertexT *vertex = nullptr, **vertexp;
    // iterate over generated facets
    std::map<unsigned int, unsigned int> faceIds;
    FORALLfacets {
      std::vector<unsigned int> facetV;
      std::vector<unsigned int> neighborsV;
      FOREACHvertex_(facet->vertices) {
#ifdef HAVE_REENTRANT_QHULL
        facetV.push_back(qh_pointid(qh, vertex->point));
#else
        facetV.push_back(qh_pointid(vertex->point));
#endif
      }
      faceIds[getid_(facet)] = facets.size();
      facets.push_back(facetV);
      facetT *neighbor, **neighborp;
      FOREACHneighbor_(facet) {
        neighborsV.push_back(getid_(neighbor));
      }
      neighbors.push_back(neighborsV);
    }

    for (size_t i = 0; i < neighbors.size(); ++i) {
      for (size_t j = 0; j < neighbors[i].size(); ++j) {
        neighbors[i][j] = faceIds[neighbors[i][j]];
      }
    }
  }

  int curlong, totlong;
// free memory allocated by qhull
#ifdef HAVE_REENTRANT_QHULL
  qh_freeqhull(qh, !qh_ALL);
  qh_memfreeshort(qh, &curlong, &totlong);
#else
  qh_freeqhull(!qh_ALL);
  qh_memfreeshort(&curlong, &totlong);
#endif

  return !qhullKo;
}

//================================================================================
void tlp::convexHull(const std::vector<Coord> &points, std::vector<unsigned int> &hull) {
  hull.clear();

  vector<double> pointsQHull;
  vector<vector<unsigned int>> facets;
  vector<vector<unsigned int>> neighbors;

  for (size_t i = 0; i < points.size(); ++i) {
    pointsQHull.push_back(points[i][0]);
    pointsQHull.push_back(points[i][1]);
  }

  // compute the convex hull
  if (runQHull(2, pointsQHull, facets, neighbors)) {

    // compute ordering on points defining the hull polygon
    hull.push_back(facets[0][0]);
    hull.push_back(facets[0][1]);
    unsigned int curFaceId = 0;
    float signedArea = 0;

    while (1) {
      unsigned int ridge = hull.back();
      unsigned int neighFaceAId = neighbors[curFaceId][0];
      unsigned int neighFaceBId = neighbors[curFaceId][1];

      if (facets[neighFaceAId][0] == ridge) {
        hull.push_back(facets[neighFaceAId][1]);
        curFaceId = neighFaceAId;
      } else if (facets[neighFaceAId][1] == ridge) {
        hull.push_back(facets[neighFaceAId][0]);
        curFaceId = neighFaceAId;
      } else if (facets[neighFaceBId][0] == ridge) {
        hull.push_back(facets[neighFaceBId][1]);
        curFaceId = neighFaceBId;
      } else {
        hull.push_back(facets[neighFaceBId][0]);
        curFaceId = neighFaceBId;
      }

      size_t hullSize = hull.size();

      if (hullSize > 1) {
        const Coord &point = points[hull[hullSize - 2]];
        const Coord &nextPoint = points[hull[hullSize - 1]];
        signedArea += (point[0] * nextPoint[1] - nextPoint[0] * point[1]);
      }

      if (hull.size() == facets.size()) {
        break;
      }
    }

    const Coord &point = points[hull.back()];

    const Coord &nextPoint = points[hull.front()];

    signedArea += (point[0] * nextPoint[1] - nextPoint[0] * point[1]);

    // hull points are in clockwise order, reverse the points vector in that case
    if (signedArea < 0) {
      std::reverse(hull.begin(), hull.end());
    }
  }
}

//================================================================================

void tlp::convexHull(const std::vector<Coord> &points,
                     std::vector<std::vector<unsigned int>> &convexHullFacets,
                     std::vector<std::vector<unsigned int>> &facetNeighbors) {

  convexHullFacets.clear();
  facetNeighbors.clear();
  vector<double> pointsQHull;

  // check if the layout is coplanar
  Mat3f invTransformMatrix;
  bool coPlanarLayout = isLayoutCoPlanar(points, invTransformMatrix);
  int dim = 3;

  if (!coPlanarLayout) {
    for (size_t i = 0; i < points.size(); ++i) {
      pointsQHull.push_back(points[i][0]);
      pointsQHull.push_back(points[i][1]);
      pointsQHull.push_back(points[i][2]);
    }
  } else {
    dim = 2;

    for (size_t i = 0; i < points.size(); ++i) {
      Coord p = Coord(invTransformMatrix * points[i]);
      pointsQHull.push_back(p[0]);
      pointsQHull.push_back(p[1]);
    }
  }

  runQHull(dim, pointsQHull, convexHullFacets, facetNeighbors);
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
