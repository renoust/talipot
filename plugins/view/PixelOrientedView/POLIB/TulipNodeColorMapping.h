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

#ifndef TULIPNODECOLORMAPPING_H_
#define TULIPNODECOLORMAPPING_H_

#include <tulip/Graph.h>
#include <string>

#include "ColorFunction.h"
#include "potypes.h"

namespace pocore {

class TulipNodeColorMapping : public ColorFunction {

public:
  TulipNodeColorMapping(tlp::Graph *graph);

  RGBA getColor(const double &, const unsigned int itemId) const override;

private:
  tlp::Graph *graph;
};
} // namespace pocore

#endif /* TULIPNODECOLORMAPPING_H_ */
