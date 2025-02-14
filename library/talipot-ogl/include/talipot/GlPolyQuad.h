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

///@cond DOXYGEN_HIDDEN

#ifndef TALIPOT_GL_POLY_QUAD_H
#define TALIPOT_GL_POLY_QUAD_H

#include <talipot/config.h>
#include <talipot/Coord.h>
#include <talipot/Color.h>
#include <talipot/GlSimpleEntity.h>

#include <vector>
#include <string>

namespace tlp {

/**
 * @ingroup OpenGL
 * \brief General class used to render a connected group of quadrilaterals (textured or not) that
 * shares edges as GlEntity
 *
 * This generic class is used  to render a connected group of quadrilaterals (textured or not) that
 * shares edges as GlEntity
 */
class TLP_GL_SCOPE GlPolyQuad : public GlSimpleEntity {

public:
  /**
   * Default Constructor for initializing an empty polyquad
   * Use the addQuadEdge method to set the quads edges
   *
   * \param textureName The absolute path of the texture image file to use
   *
   *
   */
  GlPolyQuad(const std::string &textureName = "", const bool outlined = false,
             const int outlineWidth = 1, const Color &outlineColor = Color(0, 0, 0));

  /**
   * Constructor for building a polyquad with spefific colors for each edges
   *
   * Pay attention to the order of the edges point in the polyQuadEdges vector. Indeed, to draw the
   * following polyquad
   *
   *                    v2
   *          v0+--------+--------+ v4
   *            |        |        |
   *            |        |        |
   *            |        |        |
   *          v1+--------+--------+ v5
   *                    v3
   *
   * The content of the polyQuadEdges vector should be {v0, v1, v2, v3, v4, v5} or {v1, v0, v3, v2,
   * v5, v4}
   *
   * \param polyQuadEdges A vector containing the coordinates of the quad edges, its size must be  a
   * multiple of 2 because an edge is defined by 2 points
   * \param polyQuadEdgesColor A vector containing the edges's colors, its size must be equal to the
   * number of edges defined by the polyQuadEdges vector
   * \param  textureName The absolute path of the texture image file to use
   */
  GlPolyQuad(const std::vector<Coord> &polyQuadEdges, const std::vector<Color> &polyQuadEdgesColor,
             const std::string &textureName = "", const bool outlined = false,
             const int outlineWidth = 1, const Color &outlineColor = Color(0, 0, 0));

  /**
   * Constructor for building a polyquad with a single color
   *
   * \param polyQuadEdges A vector containing the coordinates of the quad edges, its size must be  a
   * multiple of 2 because an edge is defined by 2 points
   * \param polyQuadColor The polyquad color
   * \param  textureName The absolute path of the texture image file to use
   */
  GlPolyQuad(const std::vector<Coord> &polyQuadEdges, const Color &polyQuadColor,
             const std::string &textureName = "", const bool outlined = false,
             const int outlineWidth = 1, const Color &outlineColor = Color(0, 0, 0));

  /**
   * Method to add a polyquad edge
   *
   * \param edgeStart The first end of the edge
   * \param edgeEnd The other end of the edge
   * \param edgeColor The edge's color
   *
   */
  void addQuadEdge(const Coord &edgeStart, const Coord &edgeEnd, const Color &edgeColor);

  /**
   * Virtual function used to draw the polyquad.
   */
  void draw(float lod, Camera *camera) override;

  /**
   * Method to set the polyquad color (all edges share the same color)
   */
  void setColor(const Color &color);

  /**
   * Method to set the polyquad outline color
   */
  void setOutlineColor(const Color &color) {
    outlineColor = color;
  }

  /**
   * Method to toggle polyquad outline
   */
  void setOutlined(const bool outline) {
    outlined = outline;
  }

  /**
   * Method to set the polyquad outline width
   */
  void setOutlineWidth(const int width) {
    outlineWidth = width;
  }

  /**
   *  Method to translate entity
   */
  void translate(const Coord &move) override;

  /**
   * Function to export data in outString (in XML format)
   */
  void getXML(std::string &outString) override;

  /**
   * Function to set data with inString (in XML format)
   */
  void setWithXML(const std::string &inString, unsigned int &currentPosition) override;

private:
  std::vector<Coord> polyQuadEdges; // vector which contains polyquad edges, an edge being defined
                                    // by a pair of Coord
  std::vector<Color> polyQuadEdgesColors; // vector which contains polyquad edges colors
  std::string textureName;
  bool outlined;
  int outlineWidth;
  Color outlineColor;
};
}
#endif // TALIPOT_GL_POLY_QUAD_H
///@endcond
