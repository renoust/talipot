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

#ifndef DIFUSIONRATEFUNCTIONGAUSSIAN_H_
#define DIFUSIONRATEFUNCTIONGAUSSIAN_H_

#include "DiffusionRateFunction.h"

class DifusionRateFunctionGaussian : public DiffusionRateFunction {
public:
  DifusionRateFunctionGaussian(TimeDecreasingFunction *decreasingFunction);
  ~DifusionRateFunctionGaussian() override;
  double computeSpaceRate(unsigned int distance, unsigned int currentIteration,
                          unsigned int maxIteration, unsigned int inputSampleSize) override;

protected:
};

#endif /* DIFUSIONRATEFUNCTIONGAUSSIAN_H_ */
