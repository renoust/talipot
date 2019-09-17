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
#ifndef TLPTESTALGORITHMTEST
#define TLPTESTALGORITHMTEST

#include "CppUnitIncludes.h"

namespace tlp {
class Graph;
}

class TestAlgorithmTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestAlgorithmTest);
  CPPUNIT_TEST(testSimple);
  CPPUNIT_TEST(testFreeTree);
  CPPUNIT_TEST(testTree);
  CPPUNIT_TEST(testAcyclic);
  CPPUNIT_TEST(testConnected);
  CPPUNIT_TEST(testBiconnected);
  CPPUNIT_TEST(testTriconnected);
  CPPUNIT_TEST_SUITE_END();

private:
  tlp::Graph *graph;

public:
  void setUp() override;
  void tearDown() override;
  void testSimple();
  void testFreeTree();
  void testTree();
  void testAcyclic();
  void testConnected();
  void testBiconnected();
  void testTriconnected();
};

#endif
