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

#ifndef TALIPOT_GL_GRAPH_STATIC_DATA_H
#define TALIPOT_GL_GRAPH_STATIC_DATA_H

#include <talipot/config.h>

#include <string>

namespace tlp {

class TLP_GL_SCOPE GlGraphStaticData {

public:
  static const int edgeShapesCount;
  static int edgeShapeIds[];
  static std::string labelPositionNames[];

  static std::string edgeShapeName(int id);
  static int edgeShapeId(const std::string &name);

  static std::string labelPositionName(int id);
  static int labelPositionId(const std::string &name);
};
} // namespace tlp

#endif // TALIPOT_GL_GRAPH_STATIC_DATA_H
///@endcond
