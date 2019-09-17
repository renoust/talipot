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

#include <iostream>
#include <cstdlib>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <tulip/TlpTools.h>
#ifndef NDEBUG
#include <tulip/PluginLoaderTxt.h>
#endif
#include <tulip/PluginLibraryLoader.h>

static void loadTulipPluginsFromDir(const std::string &pluginsDir,
                                    tlp::PluginLoader *pluginLoader = nullptr) {
  std::string oldTlpPluginsPath = tlp::TulipPluginsPath;
  tlp::TulipPluginsPath = pluginsDir;
  tlp::PluginLibraryLoader::loadPlugins(pluginLoader);
  tlp::TulipPluginsPath = oldTlpPluginsPath;
}

int main(int /* arg */, char ** /* argv */) {

  std::string tulipBuildDir = TULIP_BUILD_DIR;

  tlp::initTulipLib();
  tlp::PluginLoader *pLoader = nullptr;
#ifndef NDEBUG
  tlp::PluginLoaderTxt loader;
  pLoader = &loader;
#endif

  loadTulipPluginsFromDir(tulipBuildDir + "/plugins/clustering", pLoader);
  loadTulipPluginsFromDir(tulipBuildDir + "/plugins/colors", pLoader);
  loadTulipPluginsFromDir(tulipBuildDir + "/plugins/export", pLoader);
  loadTulipPluginsFromDir(tulipBuildDir + "/plugins/import", pLoader);
  loadTulipPluginsFromDir(tulipBuildDir + "/plugins/layout", pLoader);
  loadTulipPluginsFromDir(tulipBuildDir + "/plugins/layout/FastOverlapRemoval", pLoader);
  loadTulipPluginsFromDir(tulipBuildDir + "/plugins/metric", pLoader);
  loadTulipPluginsFromDir(tulipBuildDir + "/plugins/selection", pLoader);
  loadTulipPluginsFromDir(tulipBuildDir + "/plugins/sizes", pLoader);

  // CPPUNIT_NS::QtUi::TestRunner runner;
  CPPUNIT_NS::TestResult controller;
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener(&result);

  CPPUNIT_NS::TextUi::TestRunner runner;
  runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
  runner.run(controller);

  std::ofstream xmlFileOut("cpptestresults.xml");
  CPPUNIT_NS::XmlOutputter xmlOut(&result, xmlFileOut);
  xmlOut.write();

  CPPUNIT_NS::TextOutputter stdOut(&result, std::cout);
  stdOut.write();

  return result.wasSuccessful() ? EXIT_SUCCESS : EXIT_FAILURE;
}
