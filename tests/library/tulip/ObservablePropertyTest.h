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
#ifndef TLPOBSERVABLEPROPERTYTEST
#define TLPOBSERVABLEPROPERTYTEST

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/AbstractProperty.h>

#include "CppUnitIncludes.h"

class ObservablePropertyTest : public CppUnit::TestFixture {
private:
  tlp::Graph *graph;
  tlp::PropertyInterface *props[7];

public:
  void setUp() override;
  void tearDown() override;

  void testAddObserver();
  void testSynchronousSetNodeValue();
  void testAsynchronousSetNodeValue();
  void testSynchronousSetAllNodeValue();
  void testAsynchronousSetAllNodeValue();
  void testSynchronousSetEdgeValue();
  void testAsynchronousSetEdgeValue();
  void testSynchronousSetAllEdgeValue();
  void testAsynchronousSetAllEdgeValue();
  void testSynchronousDelete();
  void testAsynchronousDelete();
  void testRemoveObserver();
  void testObserverWhenRemoveObservable();
  void testNoPropertiesEventsAfterGraphClear();

  void setNodeValue(tlp::PropertyInterface *, const char *, bool, bool, bool = true);
  void setEdgeValue(tlp::PropertyInterface *, const char *, bool, bool, bool = true);
  void addObservers();

  static CppUnit::Test *suite();
};

#endif
