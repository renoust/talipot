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
#ifndef INTEGERPROPERTYMINMAXSUBGRAPHTEST_H_
#define INTEGERPROPERTYMINMAXSUBGRAPHTEST_H_

#include <tulip/Graph.h>
#include <tulip/IntegerProperty.h>

#include "CppUnitIncludes.h"

class IntegerPropertyMinMaxSubgraphTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(IntegerPropertyMinMaxSubgraphTest);
  CPPUNIT_TEST(testIntegerPropertyRootGraphMin);
  CPPUNIT_TEST(testIntegerPropertyRootGraphMax);
  CPPUNIT_TEST(testIntegerPropertySubgraphMin);
  CPPUNIT_TEST(testIntegerPropertySubgraphMax);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() override;
  void tearDown() override;
  void testIntegerPropertyRootGraphMin();
  void testIntegerPropertyRootGraphMax();
  void testIntegerPropertySubgraphMin();
  void testIntegerPropertySubgraphMax();

private:
  tlp::Graph *graph, *subGraph;
};

#endif /* INTEGERPROPERTYMINMAXSUBGRAPHTEST_H_ */
