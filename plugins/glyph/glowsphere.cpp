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
#include <tulip/TulipViewSettings.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/OpenGlIncludes.h>
#include <tulip/AroundTexturedSphere.h>

using namespace std;
using namespace tlp;

namespace tlp {

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a sphere with a glow halo colored with the "viewColor"
 * node property value.
 */
class GlowSphere : public AroundTexturedSphere {
public:
  GLYPHINFORMATION("3D - Glow Sphere", "Patrick Mary", "24/01/2012", "Glow Sphere", "1.0",
                   NodeShape::GlowSphere)
  GlowSphere(const tlp::PluginContext *context = nullptr)
      : AroundTexturedSphere(context, "radialGradientTexture.png", 128) {}
  ~GlowSphere() override {}
};

PLUGIN(GlowSphere)

class EEGlowSphere : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("3D - Glow Sphere extremity", "Patrick Mary", "24/01/2012",
                   "Glow Sphere for edge extremities", "1.0", EdgeExtremityShape::GlowSphere)
  EEGlowSphere(const tlp::PluginContext *context = nullptr) : EdgeExtremityGlyph(context) {}
  ~EEGlowSphere() override {}
  void draw(edge e, node n, const Color &glyphColor, const Color & /* borderColor */,
            float /* lod */) override {
    glDisable(GL_LIGHTING);
    AroundTexturedSphere::drawGlyph(
        glyphColor, edgeExtGlGraphInputData->getElementSize()->getNodeValue(n),
        edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e),
        edgeExtGlGraphInputData->parameters->getTexturePath(), "radialGradientTexture.png", 128);
  }
};

PLUGIN(EEGlowSphere)

} // end of namespace tlp
