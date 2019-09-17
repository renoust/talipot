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

#ifndef TULIP_CONNECTEDTEST_H
#define TULIP_CONNECTEDTEST_H

#include <set>
#include <vector>

#include <tulip/tulipconf.h>

namespace tlp {

class Graph;
struct node;
struct edge;

/**
 * @ingroup Checks
 * @brief @brief Performs a test of connexity on the graph, and provides a function to make a graph
 *connected.
 * From Wikipedia: "A graph is said to be connected if every pair of vertices in the graph are
 *connected." (i.e. there is a path between every pair of vertices).
 **/
class TLP_SCOPE ConnectedTest {
public:
  /**
   * @brief Checks if a graph is connected (i.e. there is a path between every pair of vertices).
   *
   * @param graph The graph to check.
   * @return bool True if the graph is connected, false otherwise.
   **/
  static bool isConnected(const Graph *const graph);

  /**
   * @brief If the graph is not connected, adds edges to make it connected.
   *
   * @param graph The graph to make connected.
   * @param addedEdges The edges that were added to make it connected.
   * @return void
   **/
  static void makeConnected(Graph *graph, std::vector<edge> &addedEdges);

  /**
   * @brief Gets the number of connected components in the graph.
   *
   * @param graph The graph in which to count the number of connected components.
   * @return unsigned int The number of connected componments.
   **/
  static unsigned int numberOfConnectedComponents(const Graph *const graph);

  /**
   * @brief Computes the sets of connected components and stores the result in the components
   *vector.
   *
   * @param graph The graph on which to compute connected components.
   * @param components The components that were found. It is passed as a reference to avoid copying
   *the data when returning.
   * @return void
   * @note The components parameter can be returned with c++11 thanks to move constructors without
   *performance loss, change this function once c++11 compilers are used.
   **/
  static void computeConnectedComponents(const Graph *graph,
                                         std::vector<std::vector<node>> &components);

  /**
   * @brief This function is defined for compatibility purpose only.
   * @warning This function is deprecated, use the one above.
   **/
  static _DEPRECATED void computeConnectedComponents(const Graph *graph,
                                                     std::vector<std::set<node>> &components);

private:
  /**
   * @brief Makes the graph connected.
   *
   * @param graph The graph to make connected.
   * @param toLink The nodes that need to be linked so the graph is connected.
   * @return void
   **/
  static void connect(const Graph *const, std::vector<node> &toLink);
};
} // namespace tlp

#endif
