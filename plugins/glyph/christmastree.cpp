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
#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Graph.h>
#include <tulip/GlTools.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/TulipIconicFont.h>
#include <tulip/GlLabel.h>
#include <tulip/GlGraphRenderingParameters.h>

using namespace std;
using namespace tlp;

void drawTree(const Color &color, const Color &outlineColor, const float outlineSize,
              const std::string &texture) {
  static GlLabel label;
  label.setFontNameSizeAndColor(TulipIconicFont::getTTFLocation("fa-tree"), 18, color);
  label.setPosition(Coord(0, 0, 0));
  label.setSize(Size(1, 1, 0));
  label.setUseLODOptimisation(false);
  label.setScaleToSize(true);
  label.setOutlineColor(outlineColor);
  label.setOutlineSize(outlineSize);
  label.setTextureName(texture);
  label.setText(TulipIconicFont::getIconUtf8String("fa-tree"));
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
