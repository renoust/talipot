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

namespace tlp {

class PluginsManager /Abstract/ {

public:

  static std::list<std::string> availablePlugins() /TypeHint="List[str]"/;

  static const tlp::Plugin& pluginInformation(const std::string& name);

  static bool pluginExists(const std::string& pluginName);

  static const tlp::ParameterDescriptionList& getPluginParameters(const std::string& name);

  static std::string getPluginLibrary(const std::string& name);

  static void removePlugin(const std::string &name);

};

};
