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

#ifndef WITHPARAMETERTEST_H
#define WITHPARAMETERTEST_H

#include <tulip/WithParameter.h>

#include "CppUnitIncludes.h"

class WithParameterTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(WithParameterTest);
  CPPUNIT_TEST(testBuildDefaultDataSet);
  CPPUNIT_TEST_SUITE_END();

public:
  void testBuildDefaultDataSet();
};

class ParameteredClass : public tlp::WithParameter {
public:
  ParameteredClass();
};

#endif // WITHPARAMETERTEST_H
