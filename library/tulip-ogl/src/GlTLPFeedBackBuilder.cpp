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
#include <tulip/GlTLPFeedBackBuilder.h>

namespace tlp {

void GlTLPFeedBackBuilder::passThroughToken(GLfloat *data) {
  if (!needData) {

    switch (int(*data)) {

    case TLP_FB_COLOR_INFO:
      inColorInfo = true;
      needData = true;
      break;

    case TLP_FB_BEGIN_ENTITY:
      inGlEntity = true;
      needData = true;
      break;

    case TLP_FB_END_ENTITY:
      assert(inGlEntity);
      inGlEntity = false;
      endGlEntity();
      break;

    case TLP_FB_BEGIN_GRAPH:
      inGlGraph = true;
      needData = true;
      break;

    case TLP_FB_END_GRAPH:
      assert(inGlGraph);
      inGlGraph = false;
      endGlGraph();
      break;

    case TLP_FB_BEGIN_NODE:
      inNode = true;
      needData = true;
      break;

    case TLP_FB_END_NODE:
      assert(inNode);
      inNode = false;
      endNode();
      break;

    case TLP_FB_BEGIN_EDGE:
      inEdge = true;
      needData = true;
      break;

    case TLP_FB_END_EDGE:
      assert(inEdge);
      inEdge = false;
      endEdge();
      break;

    default:
      assert(false);
      break;
    }

  } else {
    if (!inColorInfo) {
      needData = false;

      if (inGlEntity) {
        beginGlEntity(*data);
      } else if (inEdge) {
        beginEdge(*data);
      } else if (inNode) {
        beginNode(*data);
      } else if (inGlGraph) {
        beginGlGraph(*data);
      }
    } else {
      dataBuffer.push_back(*data);

      if (dataBuffer.size() >= 12) {
        GLfloat *tmp = new GLfloat[dataBuffer.size()];

        for (unsigned int i = 0; i < dataBuffer.size(); i++)
          tmp[i] = dataBuffer[i];

        colorInfo(tmp);
        dataBuffer.clear();
        needData = false;
        inColorInfo = false;
      }
    }
  }
}
} // namespace tlp
