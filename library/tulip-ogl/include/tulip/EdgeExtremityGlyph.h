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

#ifndef EDGEEXTREMITYGLYPH_H_
#define EDGEEXTREMITYGLYPH_H_

#include <tulip/Edge.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Matrix.h>
#include <tulip/Plugin.h>

namespace tlp {

class Color;

static const std::string EEGLYPH_CATEGORY = "Edge extremity";

typedef Matrix<float, 4> MatrixGL;
class GlGraphInputData;

class TLP_GL_SCOPE EdgeExtremityGlyph : public Plugin {
public:
  std::string category() const override {
    return EEGLYPH_CATEGORY;
  }
  std::string icon() const override {
    return ":/tulip/gui/icons/32/plugin_glyph.png";
  }

  EdgeExtremityGlyph(const tlp::PluginContext *context);
  ~EdgeExtremityGlyph() override;
  virtual void draw(edge e, node n, const Color &glyphColor, const Color &borderColor,
                    float lod) = 0;
  void get2DTransformationMatrix(const Coord &src, const Coord &dest, const Size &glyphSize,
                                 MatrixGL &transformationMatrix, MatrixGL &scalingMatrix);
  void get3DTransformationMatrix(const Coord &src, const Coord &dest, const Size &glyphSize,
                                 MatrixGL &transformationMatrix, MatrixGL &scalingMatrix);

protected:
  GlGraphInputData *edgeExtGlGraphInputData;
};
} // namespace tlp

#endif /* EDGEEXTREMITYGLYPH_H_ */
///@endcond
