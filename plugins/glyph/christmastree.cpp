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

#include <talipot/StringProperty.h>
#include <talipot/ColorProperty.h>
#include <talipot/Size.h>
#include <talipot/Coord.h>
#include <talipot/Glyph.h>
#include <talipot/EdgeExtremityGlyph.h>
#include <talipot/GlTextureManager.h>
#include <talipot/GlGraphInputData.h>
#include <talipot/Graph.h>
#include <talipot/GlTools.h>
#include <talipot/ViewSettings.h>
#include <talipot/IconicFont.h>
#include <talipot/GlLabel.h>
#include <talipot/GlGraphRenderingParameters.h>
#include <talipot/FontAwesome.h>

using namespace std;
using namespace tlp;

void drawTree(const Color &color, const Color &outlineColor, const float outlineSize,
              const std::string &texture) {
  static GlLabel label;
  label.setFontNameSizeAndColor(IconicFont::getTTFLocation(FontAwesome::Solid::Tree), 18, color);
  label.setPosition(Coord(0, 0, 0));
  label.setSize(Size(1, 1, 0));
  label.setUseLODOptimisation(false);
  label.setScaleToSize(true);
  label.setOutlineColor(outlineColor);
  label.setOutlineSize(outlineSize);
  label.setTextureName(texture);
  label.setText(IconicFont::getIconUtf8String(FontAwesome::Solid::Tree));
  label.draw(100);
}

/** \addtogroup glyph */

/// A 2D glyph.
/** This glyph draws a christmas tree special for christmas release of 2008
 */
class ChristmasTree : public NoShaderGlyph {
public:
  GLYPHINFORMATION("2D - ChristmasTree", "Morgan Mathiaut", "16/12/2008", "Christmas tree", "1.0",
                   NodeShape::ChristmasTree)
  ChristmasTree(const tlp::PluginContext *context = nullptr);
  void draw(node n, float lod) override;
};
PLUGIN(ChristmasTree)

ChristmasTree::ChristmasTree(const tlp::PluginContext *context) : NoShaderGlyph(context) {}
void ChristmasTree::draw(node n, float) {
  const tlp::Color &nodeColor = glGraphInputData->getElementColor()->getNodeValue(n);
  const tlp::Color &nodeBorderColor = glGraphInputData->getElementBorderColor()->getNodeValue(n);
  float nodeBorderWidth = glGraphInputData->getElementBorderWidth()->getNodeValue(n);
  const std::string &nodeTexture = glGraphInputData->parameters->getTexturePath() +
                                   glGraphInputData->getElementTexture()->getNodeValue(n);

  drawTree(nodeColor, nodeBorderColor, nodeBorderWidth, nodeTexture);
}
