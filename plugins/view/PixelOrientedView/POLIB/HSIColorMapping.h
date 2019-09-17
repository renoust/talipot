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

#ifndef HSICOLORMAPPING_H_
#define HSICOLORMAPPING_H_

#include "HSIColorSpace.h"

#include "ColorFunction.h"

namespace pocore {

class HSIColorMapping : public ColorFunction {

public:
  HSIColorMapping(const HSI &from = HSI(1.5, 1.0, 0.4), const HSI &to = HSI(1.0, 1.0, 1.0));

  // value must be normalized
  RGBA getColor(const double &value, const unsigned int) const override;

private:
  HSIColorScale hsiColorScale;
};
} // namespace pocore
#endif /* HSICOLORMAPPING_H_ */
