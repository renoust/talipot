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
///@cond DOXYGEN_HIDDEN

#ifndef OGDF_LAYOUT_PLUGIN_BASE
#define OGDF_LAYOUT_PLUGIN_BASE

#include <tulip/tulipconf.h>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/PropertyAlgorithm.h>

#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/module/LayoutModule.h>

#include "TulipToOGDF.h"

class TLP_OGDF_SCOPE OGDFLayoutPluginBase : public tlp::LayoutAlgorithm {
public:
  OGDFLayoutPluginBase(const tlp::PluginContext *context, LayoutModule *ogdfLayoutAlgo);
  ~OGDFLayoutPluginBase() override;

  bool run() override;

protected:
  virtual void beforeCall() {}
  virtual void callOGDFLayoutAlgorithm(ogdf::GraphAttributes &gAttributes);
  virtual void afterCall() {}

  void transposeLayoutVertically();

  TulipToOGDF *tlpToOGDF;
  ogdf::LayoutModule *ogdfLayoutAlgo;
};

#endif // OGDF_LAYOUT_PLUGIN_BASE
       ///@endcond