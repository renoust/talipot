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

#ifndef TIMEDECREASINGFUNCTIONSIMPLE_H_
#define TIMEDECREASINGFUNCTIONSIMPLE_H_

#include "TimeDecreasingFunction.h"

class TimeDecreasingFunctionSimple : public TimeDecreasingFunction {
public:
  TimeDecreasingFunctionSimple(double initialCoef);
  ~TimeDecreasingFunctionSimple() override;
  double computeCurrentTimeRate(unsigned int currentIteration, unsigned int maxIteration,
                                unsigned int inputSampleSize) override;

  double getInitialCoefficient() {
    return initialCoef;
  }

  void setInitialCoefficient(double coef) {
    initialCoef = coef;
  }

protected:
  double initialCoef;
};

#endif /* TIMEDECREASINGFUNCTIONSIMPLE_H_ */
