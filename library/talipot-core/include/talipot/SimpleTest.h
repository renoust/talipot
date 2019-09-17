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

#ifndef TALIPOT_SIMPLE_TEST_H
#define TALIPOT_SIMPLE_TEST_H

#include <vector>

#include <talipot/config.h>

namespace tlp {

class Graph;
struct edge;

/**
 * @ingroup Checks
 * @brief Performs test to check if a graph is Simple.
 * An undirected graph is simple if it has no loops and no more than one
 * edge between any unordered pair of vertices.
 * A directed graph is simple if has no loops and no more than one
 * edge between any ordered pair of vertices.
 **/
class TLP_SCOPE SimpleTest {
public:
  /**
   * @brief Checks if the graph is simple (i.e. it contains no self loops or parallel edges).
   *
   * @param graph The graph to check.
   * @param directed Whether the graph shall be considered directed or not.
   * @return bool True if the graph is simple, false otherwise.
   **/
  static bool isSimple(const Graph *graph, const bool directed = false);

  /**
   * Makes the graph  simple by removing self loops and parallel edges if any.
   * The removed edges are stored in the removed vector.
   */
  /**
   * @brief Makes the graph simple, by removing self loops and parallel edges if any.
   *
   * @param graph The graph to make simple.
   * @param removed The edges that were removed to make the graph simple.
   * @param directed Whether the graph shall be considered directed or not.
   * @return void
   **/
  static void makeSimple(Graph *graph, std::vector<edge> &removed, const bool directed = false);

  /**
   * Performs simple test and stores found parallel edges in the multipleEdges vector
   * and found self loops in the loops vector.
   * Returns true if the graph is simple, false otherwise.
   */
  /**
   * @brief Checks if the graph is simple, and stores parallel edges and self loops in different
   *vectors.
   *
   * @param graph The graph to check for simplicity.
   * @param multipleEdges The parallel edges that need to be removed to make the graph simple.
   *Defaults to 0.
   * @param loops The self loops that need to be removed to make the graph simple. Defaults to 0.
   * @param directed Whether the graph shall be considered directed or not.
   * @return bool True if the graph is simple, false otherwise.
   **/
  static bool simpleTest(const Graph *graph, std::vector<edge> *multipleEdges = nullptr,
                         std::vector<edge> *loops = nullptr, const bool directed = false);
};
} // namespace tlp
#endif // TALIPOT_SIMPLE_TEST_H
