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

#include <string>
#include <iostream>

#include "IntegerPropertyMinMaxUptodateTest.h"

using namespace tlp;
using namespace std;

const string integerPropertyName = "integer property test";

const int originalMin = 5;
const int originalMax = 10;

const int newMin = 1;
const string newStringMin = "1";
const int newMax = 15;
const string newStringMax = "15";

CPPUNIT_TEST_SUITE_REGISTRATION(IntegerPropertyMinMaxUpdateTest);

void IntegerPropertyMinMaxUpdateTest::setUp() {
  node n2, n3;
  graph = newGraph();
  graph->getLocalProperty<IntegerProperty>(integerPropertyName);
  n1 = graph->addNode();
  graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeValue(n1, originalMin);

  n2 = graph->addNode();
  graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeValue(n2, 6);

  n3 = graph->addNode();
  graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeValue(n3, 7);

  n4 = graph->addNode();
  graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeValue(n4, originalMax);
}

void IntegerPropertyMinMaxUpdateTest::tearDown() {
  delete graph;
}

void IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMinUpdate() {
  IntegerProperty *integerProperty = graph->getProperty<IntegerProperty>(integerPropertyName);
  int minNode;

  minNode = integerProperty->getNodeMin();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty min value before update", originalMin,
                               minNode);

  integerProperty->setNodeValue(n1, newMin);
  minNode = integerProperty->getNodeMin();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty min value after update", newMin, minNode);

  Graph *subGraph = graph->addSubGraph();
  node n2 = subGraph->addNode();
  integerProperty->setNodeValue(n2, 6);
  node n3 = subGraph->addNode();
  integerProperty->setNodeValue(n3, 9);
  CPPUNIT_ASSERT_EQUAL(newMin, integerProperty->getNodeMin());
  CPPUNIT_ASSERT_EQUAL(6, integerProperty->getNodeMin(subGraph));

  subGraph->delNode(n2);
  CPPUNIT_ASSERT_EQUAL(9, integerProperty->getNodeMin(subGraph));
  CPPUNIT_ASSERT_EQUAL(newMin, integerProperty->getNodeMin());
  graph->delNode(n1);
  CPPUNIT_ASSERT_EQUAL(9, integerProperty->getNodeMin(subGraph));
  CPPUNIT_ASSERT_EQUAL(6, integerProperty->getNodeMin());
}

void IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMaxUpdate() {
  IntegerProperty *integerProperty = graph->getProperty<IntegerProperty>(integerPropertyName);
  int maxNode;

  maxNode = integerProperty->getNodeMax();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty max value before update", originalMax,
                               maxNode);

  integerProperty->setNodeValue(n4, newMax);
  maxNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMax();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty max value after update", newMax, maxNode);

  Graph *subGraph = graph->addSubGraph();
  node n2 = subGraph->addNode();
  integerProperty->setNodeValue(n2, 6);
  node n3 = subGraph->addNode();
  integerProperty->setNodeValue(n3, 9);
  CPPUNIT_ASSERT_EQUAL(newMax, integerProperty->getNodeMax());
  CPPUNIT_ASSERT_EQUAL(9, integerProperty->getNodeMax(subGraph));

  subGraph->delNode(n3);
  CPPUNIT_ASSERT_EQUAL(newMax, integerProperty->getNodeMax());
  CPPUNIT_ASSERT_EQUAL(6, integerProperty->getNodeMax(subGraph));
  graph->delNode(n4);
  CPPUNIT_ASSERT_EQUAL(6, integerProperty->getNodeMax(subGraph));
  CPPUNIT_ASSERT_EQUAL(9, integerProperty->getNodeMax());
}

void IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMinUpdateFromString() {
  int minNode;

  minNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMin();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty min value before update", originalMin,
                               minNode);

  graph->getLocalProperty<IntegerProperty>(integerPropertyName)
      ->setNodeStringValue(n1, newStringMin);
  minNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMin();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty min value after update", newMin, minNode);
}

void IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMaxUpdateFromString() {
  int maxNode;

  maxNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMax();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty max value before update", originalMax,
                               maxNode);

  graph->getLocalProperty<IntegerProperty>(integerPropertyName)
      ->setNodeStringValue(n4, newStringMax);
  maxNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMax();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty max value after update", newMax, maxNode);
}
