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
#include <talipot/DoubleProperty.h>
#include <talipot/LayoutProperty.h>
#include <talipot/SizeProperty.h>
#include <talipot/GlTools.h>
#include <talipot/GlConvexHull.h>
#include <talipot/GlXMLTools.h>

using namespace std;

namespace tlp {

GlConvexHull::GlConvexHull(const vector<Coord> &points, const vector<Color> &fcolors,
                           const vector<Color> &ocolors, const bool filled, const bool outlined,
                           const string &name, const bool computeHull)
    :

      _points(points), _fillColors(fcolors), _outlineColors(ocolors), _filled(filled),
      _outlined(outlined), _name(name) {

  assert(points.size() >= 3);

  if (computeHull) {
    vector<unsigned int> convexHullIdxs;

    convexHull(_points, convexHullIdxs);
    // build new points
    vector<Coord> points;

    for (auto i : convexHullIdxs) {
      points.push_back(_points[i]);
      boundingBox.expand(_points[i]);
    }

    _points = points;
  }
}

void GlConvexHull::draw(float, Camera *) {
  glEnable(GL_BLEND);

  if (_filled) {

    if (_points.size() == 3)
      glBegin(GL_TRIANGLES);
    else if (_points.size() == 4)
      glBegin(GL_QUADS);
    else
      glBegin(GL_POLYGON);

    for (unsigned int i = 0; i < _points.size(); ++i) {
      if (i < _fillColors.size()) {
        setMaterial(_fillColors[i]);
      }

      //_points[i][2] = 0;
      glVertex3fv(reinterpret_cast<float *>(&_points[i]));
    }

    glEnd();
  }

  if (_outlined) {
    glBegin(GL_LINE_LOOP);

    for (unsigned int i = 0; i < _points.size(); ++i) {
      if (i < _outlineColors.size()) {
        setColor(_outlineColors[i]);
      }

      //_points[i][2] = 0;
      glVertex3fv(reinterpret_cast<float *>(&_points[i]));
    }

    glEnd();
  }
}

#define V_INC 30
static Color darkerColor(Color c) {
  Color dc;
  unsigned char v = c.getR();
  dc.setR((v > V_INC) ? (v - V_INC) : v);
  v = c.getG();
  dc.setG((v > V_INC) ? (v - V_INC) : v);
  v = c.getB();
  dc.setB((v > V_INC) ? (v - V_INC) : v);
  v = c.getA();
  dc.setA((v < (255 - V_INC)) ? (v + V_INC) : v);
  return dc;
}

ConvexHullItem *GlConvexHull::buildConvexHullsFromHierarchy(Graph *graph,
                                                            std::vector<Color> fColors,
                                                            std::vector<Color> oColors,
                                                            bool deducedFromChilds, Graph *root,
                                                            unsigned int depth) {
  ConvexHullItem *convexHullItem = new ConvexHullItem;
  convexHullItem->_graph = graph;
  graph->getAttributes().get("name", convexHullItem->name);

  if (convexHullItem->name.empty()) {
    std::stringstream s;
    s << graph->getId();
    convexHullItem->name = s.str();
  }

  if (root == nullptr)
    root = graph;

  if (fColors.size() == 0) {
    // use default colors
    fColors.push_back(Color(255, 148, 169, 200));
    fColors.push_back(Color(153, 250, 255, 200));
    fColors.push_back(Color(255, 152, 248, 200));
    fColors.push_back(Color(157, 152, 255, 200));
    fColors.push_back(Color(255, 220, 0, 200));
    fColors.push_back(Color(252, 255, 158, 200));
  }

  if (oColors.size() == 0) {
    // use default color
    oColors.push_back(Color(100, 100, 100, 120));
  }

  // build convex hulls from subgraphs
  for (Graph *sg : graph->subGraphs()) {

    ConvexHullItem *child =
        buildConvexHullsFromHierarchy(sg, fColors, oColors, deducedFromChilds, root, depth + 1);

    // add all
    convexHullItem->children.push_back(child);
  }

  // filled and outline colors determination
  Color fColor = fColors[depth % fColors.size()];
  Color oColor = oColors[depth % oColors.size()];
  unsigned int i = depth / fColors.size();

  while (i > 0) {
    fColor = darkerColor(fColor);
    i--;
  }

  i = depth / oColors.size();

  while (i > 0) {
    oColor = darkerColor(oColor);
    i--;
  }

  // build args to create a GlConvexHull for this graph
  // vectors of Color
  vector<Color> filledColors;
  vector<Color> outColors;
  filledColors.push_back(fColor);
  outColors.push_back(oColor);

  if (depth) {
    // compute this graph convex hull
    vector<Coord> gConvexHull;

    // no subgraphs
    // the convex hull will be build directly with the graph nodes and edges
    // if there is some
    if (graph->numberOfNodes()) {
      LayoutProperty *layout = root->getLayoutProperty("viewLayout");
      SizeProperty *size = root->getSizeProperty("viewSize");
      DoubleProperty *rot = root->getDoubleProperty("viewRotation");
      node n;
      // the variable below will be used to compute
      // the box around the bends of edges
      float bendsl = FLT_MAX;
      for (auto n : graph->nodes()) {
        // get node coordinates
        const Coord &point = layout->getNodeValue(n);
        // get size of bounding box
        const Size &box = size->getNodeValue(n);
        // get box rotation in degree
        double alpha = rot->getNodeValue(n);
        // convert in radian
        alpha = M_PI * alpha / 180.0;
        // get half height of bounding box
        float hh = box.getH() / 2.0;
        // get half width of bounding box
        float hw = box.getW() / 2.0;
        // add 10%
        float hl;

        if (hh / 10. < hw / 10.)
          hl = hh / 10.;
        else
          hl = hw / 10.;

        hh += hl;
        hw += hl;

        if (bendsl > hl)
          bendsl = hl;

        // add points of rotated bounding box
        float cosA = cos(alpha);
        float sinA = sin(alpha);
        Coord vect = {0, 0, -0.01f};
        vect.setX(-hw * cosA + hh * sinA);
        vect.setY(-hw * sinA - hh * cosA);
        gConvexHull.push_back(point + vect);
        vect.setX(-hw * cosA - hh * sinA);
        vect.setY(-hw * sinA + hh * cosA);
        gConvexHull.push_back(point + vect);
        vect.setX(hw * cosA - hh * sinA);
        vect.setY(hw * sinA + hh * cosA);
        gConvexHull.push_back(point + vect);
        vect.setX(hw * cosA + hh * sinA);
        vect.setY(hw * sinA - hh * cosA);
        gConvexHull.push_back(point + vect);
      }
      // add bends of edges
      for (auto e : graph->edges()) {
        // get bends of the edge
        std::vector<Coord> bends = layout->getEdgeValue(e);
        unsigned int nbBends = bends.size();

        for (unsigned int i = 0; i < nbBends; i++) {
          Coord point = bends[i];
          double x = point.getX(), y = point.getY();
          point.setX(x - bendsl);
          point.setY(y - bendsl);
          gConvexHull.push_back(point);
          point.setY(y + bendsl);
          gConvexHull.push_back(point);
          point.setX(x + bendsl);
          gConvexHull.push_back(point);
          point.setY(y - bendsl);
          gConvexHull.push_back(point);
        }
      }
      // add a GlConvexHull for this graph in front of convexHulls
      string name;
      graph->getAttribute("name", name);
      convexHullItem->hull =
          new GlConvexHull(gConvexHull, filledColors, outColors, true, true, name);
      convexHullItem->hull->_graph = graph;
    }

    //}
  }

  return convexHullItem;
}
//====================================================
void GlConvexHull::translate(const Coord &move) {
  boundingBox.translate(move);

  for (auto &p : _points) {
    p += move;
  }
}
//====================================================
void GlConvexHull::getXML(string &outString) {

  GlXMLTools::createProperty(outString, "type", "GlConvexHull", "GlEntity");

  GlXMLTools::getXML(outString, "points", _points);
  GlXMLTools::getXML(outString, "fillColors", _fillColors);
  GlXMLTools::getXML(outString, "outlineColor", _outlineColors);
  GlXMLTools::getXML(outString, "filled", _filled);
  GlXMLTools::getXML(outString, "outlined", _outlined);
}
//====================================================
void GlConvexHull::setWithXML(const string &inString, unsigned int &currentPosition) {
  GlXMLTools::setWithXML(inString, currentPosition, "points", _points);
  GlXMLTools::setWithXML(inString, currentPosition, "fillColors", _fillColors);
  GlXMLTools::setWithXML(inString, currentPosition, "outlineColor", _outlineColors);
  GlXMLTools::setWithXML(inString, currentPosition, "filled", _filled);
  GlXMLTools::setWithXML(inString, currentPosition, "outlined", _outlined);
}
}
