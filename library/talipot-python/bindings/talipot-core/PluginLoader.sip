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
class PluginLoader {
%TypeHeaderCode
#include <talipot/PluginLoader.h>
%End

public:

  virtual ~PluginLoader();

  virtual void start(const std::string &path) = 0;

  virtual void numberOfFiles(int);

  virtual void loading(const std::string &filename) = 0;

  virtual void loaded(const tlp::Plugin* info, const std::list <tlp::Dependency>& dependencies) = 0;

  virtual void aborted(const std::string &filename,const  std::string &errormsg) = 0;

  virtual void finished(bool state,const std::string &msg) = 0;

};
};
