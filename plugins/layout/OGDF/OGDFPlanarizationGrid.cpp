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

#include <ogdf/planarity/PlanarizationGridLayout.h>
#include <ogdf/planarity/FastPlanarSubgraph.h>
#include <ogdf/planarity/MaximalPlanarSubgraphSimple.h>
#include <ogdf/planarity/FixedEmbeddingInserter.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>

#include "talipot2ogdf/OGDFLayoutPluginBase.h"

#include <talipot/StringCollection.h>

#define ELT_PLANARSUBGRAPH "Planar subgraph module"
#define ELT_PLANARSUBGRAPHLIST "FastPlanarSubgraph;MaximalPlanarSubgraphSimple"
#define ELT_FASTPLANAR 0
#define ELT_MAXIMALPLANAR 1

#define ELT_EDGEINSERTION "Edge insertion module"
#define ELT_EDGEINSERTIONLIST "FixedEmbeddingInserter;VariableEmbeddingInserter"
#define ELT_FIXEDEMBEDDING 0
#define ELT_VARIABLEMBEDDING 1

// comments below have been extracted from OGDF/src/planarity/PlanarizationGridLayout.cpp
/** \addtogroup layout */

/**
 * Implements planarization with grid layout.
 *
 * \author Carsten Gutwenger
 *
 * \par License:
 * This is part of the Open Graph Drawing Framework (OGDF).
 *
 * Copyright (C). All rights reserved.
 * See README.txt in the root directory of the OGDF installation for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation
 * and appearing in the files LICENSE_GPL_v2.txt and
 * LICENSE_GPL_v3.txt included in the packaging of this file.
 *
 * \par
 * In addition, as a special exception, you have permission to link
 * this software with the libraries of the COIN-OR Osi project
 * (http://www.coin-or.org/projects/Osi.xml), all libraries required
 * by Osi, and all LP-solver libraries directly supported by the
 * COIN-OR Osi project, and distribute executables, as long as
 * you follow the requirements of the GNU General Public License
 * in regard to all of the software in the executable aside from these
 * third-party libraries.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/

static const char *paramHelp[] = {
    // page ratio
    "Sets the option pageRatio."};

class OGDFPlanarizationGrid : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Planarization Grid (OGDF)", "Carsten Gutwenger", "12/11/2007",
                    "The planarization grid layout algorithm applies the planarization approach "
                    "for crossing minimization, combined with the topology-shape-metrics approach "
                    "for orthogonal planar graph drawing. It produces drawings with few crossings "
                    "and is suited for small to medium sized sparse graphs. It uses a planar grid "
                    "layout algorithm to produce a drawing on a grid.",
                    "1.0", "Planar")
  OGDFPlanarizationGrid(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::PlanarizationGridLayout()) {
    addInParameter<double>("page ratio", paramHelp[0], "1.1");
  }

  ~OGDFPlanarizationGrid() override {}

  void beforeCall() override {
    ogdf::PlanarizationGridLayout *pgl =
        static_cast<ogdf::PlanarizationGridLayout *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      double dval = 0;

      if (dataSet->get("page ratio", dval))
        pgl->pageRatio(dval);
    }
  }
};

PLUGIN(OGDFPlanarizationGrid)
