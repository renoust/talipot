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

%ModuleHeaderCode
#include <talipot/MapIterator.h>
namespace tlp {
typedef tlp::Iterator<tlp::node> IteratorNode;
typedef tlp::Iterator<tlp::edge> IteratorEdge;
}
%End

namespace tlp {

typedef tlp::Iterator<tlp::node> IteratorNode;
typedef tlp::Iterator<tlp::edge> IteratorEdge;

tlp::edge nextFaceEdge(tlp::Graph* g, tlp::edge source, tlp::node target);

class NodeMapIterator : tlp::IteratorNode {
%TypeHeaderCode
#include <talipot/MapIterator.h>
%End

public :

  NodeMapIterator(tlp::Graph *sg, tlp::node source, tlp::node target);
  ~NodeMapIterator();

};

class EdgeMapIterator : tlp::IteratorEdge {
%TypeHeaderCode
#include <talipot/MapIterator.h>
%End

public :

  EdgeMapIterator(tlp::Graph *sg, tlp::edge source, tlp::node target);
  ~EdgeMapIterator();

};

};

