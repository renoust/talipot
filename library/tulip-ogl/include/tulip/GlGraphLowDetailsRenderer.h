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

#ifndef Tulip_GLGRAPHLOWDETAILSRENDERER_H
#define Tulip_GLGRAPHLOWDETAILSRENDERER_H

#include <tulip/OpenGlIncludes.h>

#include <vector>

#include <tulip/Observable.h>
#include <tulip/GlGraphRenderer.h>

namespace tlp {

class Graph;
class GlScene;
class LayoutProperty;
class ColorProperty;
class SizeProperty;
class BooleanProperty;

/** \brief Class to display graph with very simple and very fast renderer
 *
 * Very high performance renderer
 * This class display graph with :
 *  - Nodes : quads
 *  - Edges : lines
 * Warning : this renderer doesn't work for selection
 *
 * See GlGraphRenderer documentation for functions documentations
 */
class TLP_GL_SCOPE GlGraphLowDetailsRenderer : public GlGraphRenderer, public Observable {

public:
  GlGraphLowDetailsRenderer(const GlGraphInputData *inputData);

  ~GlGraphLowDetailsRenderer() override;

  void draw(float lod, Camera *camera) override;

protected:
  void initEdgesArray();
  void initTexArray(unsigned int glyph, Vec2f tex[4]);
  void initNodesArray();

  void addObservers();
  void removeObservers();
  void updateObservers();
  void treatEvent(const Event &ev) override;

  GlScene *fakeScene;

  bool buildVBO;

  std::vector<Vec2f> points;
  std::vector<Color> colors;
  std::vector<GLuint> indices;

  std::vector<Vec2f> quad_points;
  std::vector<Color> quad_colors;
  std::vector<GLuint> quad_indices;

  Graph *observedGraph;
  LayoutProperty *observedLayoutProperty;
  ColorProperty *observedColorProperty;
  SizeProperty *observedSizeProperty;
  BooleanProperty *observedSelectionProperty;
};
} // namespace tlp

#endif
///@endcond
