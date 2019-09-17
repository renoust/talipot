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

#include <iostream>
#include <cmath>

#include "SpiralLayout.h"

using namespace std;
using namespace tlp;

namespace {
inline double eq2D2(const double c) {
  return (4. - sqrt(16. - 16. * c)) / -8.;
}
} // namespace

namespace pocore {
//==============================================================
unsigned int SpiralLayout::unproject(const Vec2i &point) const {
  int x = point[0];
  int y = point[1];

  if (x == 0 && y == 0)
    return 0;

  int c = std::max(x, y);
  c = std::max(c, std::max(-x, -y));
  unsigned int t1 = (c - 1), t2 = t1 << 2;
  unsigned int p = t2 * t1 + t2 + 1; // p equal the number of if id in the disk

  if (x == c && y == c) { // last point
    p += 8 * c - 1;
  } else if (x == c) { // right
    p += 1 * c - y - 1;
  } else if (y == -c) { // bottom
    p += 3 * c - x - 1;
  } else if (x == -c) { // left
    p += 5 * c + y - 1;
  } else if (y == c) { // top
    p += 7 * c + x - 1;
  }

  return p;
}
//==============================================================
Vector<int, 2> SpiralLayout::project(const unsigned int _id) const {
  Vector<int, 2> point;

  if (_id == 0) {
    point[0] = 0;
    point[1] = 0;
  } else {
    int c = 0;

    if (_id == 1)
      c = 1;
    else {
      // c = (int)ceil(eq2D(4., 4., 1. - float(_id)));
      c = int(ceil(eq2D2(1. - double(_id))));
    }

    unsigned int t1 = (c - 1), t2 = t1 << 2;
    int p = _id - (t1 * t2 + t2 + 1);
    unsigned char k = p / (2 * c);

    switch (k) {
    case 0:
      point[0] = c;
      point[1] = c - 1 - p;
      break;

    case 1:
      point[0] = c - (p - 2 * c) - 1;
      point[1] = -c;
      break;

    case 2:
      point[0] = -c;
      point[1] = -c + (p - 4 * c) + 1;
      break;

    case 3:
    case 4:
      point[0] = -c + (p - 6 * c) + 1;
      point[1] = c;
      break;

    default:
      cerr << "[error] : " << int(k) << endl;
    }
  }

  return point;
}
} // namespace pocore
//==============================================================================
/*
 *  781
 *  602
 *  543
 *
 */
/*
bool testProject() {
  cerr << __PRETTY_FUNCTION__ << endl;
  int points[2];
  unsigned char color[4];
  compute(0, points, color);
  if (points[0]!=0 || points[1]!=0) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(1, points, color);
  if (points[0]!=1 || points[1]!=1) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(2, points, color);
  if (points[0]!=1 || points[1]!=0) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(3, points, color);
  if (points[0]!=1 || points[1]!=-1) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(4, points, color);
  if (points[0]!=0 || points[1]!=-1) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(5, points, color);
  if (points[0]!=-1 || points[1]!=-1) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(6, points, color);
  if (points[0]!=-1 || points[1]!=0) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(7, points, color);
  if (points[0]!=-1 || points[1]!=1) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(8, points, color);
  if (points[0]!=0 || points[1]!=1) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(9, points, color);
  if (points[0]!=2 || points[1]!=2) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(10, points, color);
  if (points[0]!=2 || points[1]!=1) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(11, points, color);
  if (points[0]!=2 || points[1]!=0) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(12, points, color);
  if (points[0]!=2 || points[1]!=-1) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(13, points, color);
  if (points[0]!=2 || points[1]!=-2) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  compute(25, points, color);
  if (points[0]!=3 || points[1]!=3) {
    cerr << "error : " << __LINE__ << ": (" << points[0] << "," << points[1] << ")" << endl;
  }
  return true;
}
bool testUnproject() {
  cerr << __PRETTY_FUNCTION__ << endl;
  int points[2];
  unsigned char color[4];
  for (unsigned int i = 0; i< 10000; ++i) {
    compute(i, points, color);
    if (inverseCompute(points) != i) {
      cerr << "error line : " << __LINE__ ;
    cerr << "/ inv :";
    cerr << inverseCompute(points) << " / pos : (" << points[0] << "," << points[1] << ") /";
    cerr << " i: " << i << endl;
    }
  }
  return true;
}
*/
