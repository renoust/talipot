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

// This implementation of HSI Color Space is from the following paper : "Issues in Visualizing Large
// Databases"
// D. A. Keim, H.-P. Kriegel

#ifndef HSI_H_
#define HSI_H_

#include "potypes.h"

namespace pocore {

class HSI {

public:
  HSI(double hue, double saturation, double intensity);
  HSI(const RGBA &c);
  double value(double hue_phase) const;
  RGBA getRGBA() const;

  double hue, saturation, intensity;
};

class HSIColorScale {

public:
  HSIColorScale(const HSI &from, const HSI &to);
  HSI operator[](double f) const;

private:
  HSI foot;
  double hue_range, sat_range, int_range;
};
} // namespace pocore
#endif /* HSI_H_ */
