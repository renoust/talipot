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

#ifndef DOUBLEPROPERTYTEST_H_
#define DOUBLEPROPERTYTEST_H_

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>

#include "CppUnitIncludes.h"

class DoublePropertyTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(DoublePropertyTest);
  CPPUNIT_TEST(testAnonymousDoublePropertyMaxUpdate);
  CPPUNIT_TEST(testDoublePropertyMinUpdate);
  CPPUNIT_TEST(testDoublePropertyMaxUpdate);
  CPPUNIT_TEST(testDoublePropertyMinUpdateFromString);
  CPPUNIT_TEST(testDoublePropertyMaxUpdateFromString);
  CPPUNIT_TEST(testDoublePropertySubGraphMin);
  CPPUNIT_TEST(testDoublePropertyInfValue);
  CPPUNIT_TEST(testDoublePropertySetAllValue);
  CPPUNIT_TEST(testDoublePropertySetDefaultValue);
  CPPUNIT_TEST(testStaticDoublePropertyCopyFrom);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() override;
  void tearDown() override;
  void testAnonymousDoublePropertyMaxUpdate();
  void testDoublePropertyMinUpdate();
  void testDoublePropertyMaxUpdate();
  void testDoublePropertyMinUpdateFromString();
  void testDoublePropertyMaxUpdateFromString();
  void testDoublePropertySubGraphMin();
  void testDoublePropertySubGraphMax();
  void testDoublePropertyInfValue();
  void testDoublePropertySetAllValue();
  void testDoublePropertySetDefaultValue();
  void testStaticDoublePropertyCopyFrom();

private:
  tlp::Graph *graph;
  tlp::node n1, n2, n3, n4;
  tlp::edge e1, e2;
};

#endif /* DOUBLEPROPERTYTEST_H_ */
