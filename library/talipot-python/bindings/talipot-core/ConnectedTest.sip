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
typedef std::vector<tlp::node> vectorNodes;
%End

typedef std::vector<tlp::node> vectorNodes;

namespace tlp {

class ConnectedTest {
%TypeHeaderCode
#include <talipot/ConnectedTest.h>
%End

public:

  static bool isConnected(const tlp::Graph *graph);
%Docstring
tlp.ConnectedTest.isConnected(graph)

Returns :const:`True` if the graph is connected (ie. it exists an undirected path
between each pair of nodes), :const:`False` otherwise.

:param graph: the graph on which to perform the connected test
:type graph: :class:`tlp.Graph`
:rtype: boolean
%End

//===========================================================================================

  static void makeConnected(tlp::Graph *graph, std::vector<tlp::edge>& addedEdges /Out/);
%Docstring
tlp.ConnectedTest.makeConnected(graph)

If the graph is not connected, adds edges in order to make the graph
connected. Returns a list of added edges.

:param graph: the graph to make connected
:type graph: :class:`tlp.Graph`
:rtype: list of :class:`tlp.edge`
%End

//===========================================================================================


  static unsigned int numberOfConnectedComponents(const tlp::Graph *graph);
%Docstring
tlp.ConnectedTest.numberOfConnectedComponents(graph)

Returns the number of connected components in the graph.

:param graph: the graph on which to compute the number of connected components
:type graph: :class:`tlp.Graph`
:rtype: integer
%End

//===========================================================================================

  static void computeConnectedComponents(tlp::Graph *graph, std::vector<vectorNodes>& components /Out, TypeHint="List[List[tlp.node]]"/) ;
%Docstring
tlp.ConnectedTest.computeConnectedComponents(graph)

Computes the sets of connected components. Returns a list of connected components.

:param graph: the graph on which to compute the sets of connected components
:type graph: :class:`tlp.Graph`
:rtype: list of list of :class:`tlp.node`
%End

private :

  ConnectedTest();
};

};
