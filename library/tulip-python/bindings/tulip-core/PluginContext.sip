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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

namespace tlp {

class PluginContext {
%TypeHeaderCode
#include <tulip/PluginContext.h>
%End

public:

  virtual ~PluginContext();

};

class AlgorithmContext : tlp::PluginContext {
%TypeHeaderCode
#include <tulip/PluginContext.h>
%End

public :

  AlgorithmContext (tlp::Graph* graph = NULL, tlp::DataSet* dataSet = NULL, tlp::PluginProgress* progress = NULL);

  ~AlgorithmContext ();

  tlp::Graph *graph;

  tlp::PluginProgress *pluginProgress;

  tlp::DataSet *dataSet;
};

};
