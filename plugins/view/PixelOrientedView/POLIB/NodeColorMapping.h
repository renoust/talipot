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

#ifndef NODE_COLOR_MAPPING_H
#define NODE_COLOR_MAPPING_H

#include <talipot/Graph.h>
#include <string>

#include "ColorFunction.h"
#include "potypes.h"

namespace pocore {

class NodeColorMapping : public ColorFunction {

public:
  NodeColorMapping(tlp::Graph *graph);

  RGBA getColor(const double &, const unsigned int itemId) const override;

private:
  tlp::Graph *graph;
};
} // namespace pocore

#endif // NODE_COLOR_MAPPING_H
