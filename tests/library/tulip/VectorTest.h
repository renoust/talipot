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
#ifndef TLPBIEXTCLUSTEROPERATION
#define TLPBIEXTCLUSTEROPERATION

#include <string>
#include <tulip/Graph.h>
#include <tulip/Vector.h>

#include "CppUnitIncludes.h"

class VectorTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(VectorTest);
  CPPUNIT_TEST(testExternalOperation);
  CPPUNIT_TEST(testInternalOperation);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() override {}
  void tearDown() override {}
  void testExternalOperation();
  void testInternalOperation();
};

#endif
