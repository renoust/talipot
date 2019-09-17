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
#ifndef TLPIMPORTEXPORT
#define TLPIMPORTEXPORT

#include <string>
#include <tulip/Graph.h>
#include <tulip/TlpTools.h>

#include "CppUnitIncludes.h"

class TlpImportExportTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TlpImportExportTest);
  CPPUNIT_TEST(testImport);
  CPPUNIT_TEST(testSave);
  CPPUNIT_TEST(testExport);
  CPPUNIT_TEST(testExportCluster);
  CPPUNIT_TEST(testExportAttributes);
  CPPUNIT_TEST_SUITE_END();

public:
  void testLoad();
  void testImport();
  void testSave();
  void testExport();
  void testExportCluster();
  void testExportAttributes();
};

#endif
