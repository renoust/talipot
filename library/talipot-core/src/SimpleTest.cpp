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

#include <vector>
#include <talipot/Graph.h>
#include <talipot/GraphTools.h>
#include <talipot/SimpleTest.h>
#include <talipot/MutableContainer.h>

using namespace std;
using namespace tlp;
//=================================================================
class SimpleTestListener : public Observable {
public:
  // override of Observable::treatEvent to remove the cached result for a graph if it is modified.
  void treatEvent(const Event &) override;

  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the map.
   **/
  std::unordered_map<const Graph *, bool> resultsBuffer;

  inline void deleteResult(Graph *graph) {
    resultsBuffer.erase(graph);
    graph->removeListener(this);
  }
};

//=================================================================
void SimpleTestListener::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer[graph])
        deleteResult(graph);

      break;

    case GraphEvent::TLP_DEL_EDGE:

      if (!resultsBuffer[graph])
        deleteResult(graph);

      break;

    case GraphEvent::TLP_REVERSE_EDGE:
      deleteResult(graph);
      break;

    default:
      break;
    }
  } else {

    Graph *graph = static_cast<Graph *>(evt.sender());

    if (evt.type() == Event::TLP_DELETE)
      deleteResult(graph);
  }
}
//=================================================================
static SimpleTestListener undirInstance;
static SimpleTestListener dirInstance;
//=================================================================
bool SimpleTest::isSimple(const tlp::Graph *graph, const bool directed) {
  SimpleTestListener *instance = nullptr;
  if (directed) {
    instance = &dirInstance;
  } else {
    instance = &undirInstance;
  }

  auto it = instance->resultsBuffer.find(graph);
  if (it != instance->resultsBuffer.end())
    return it->second;

  graph->addListener(instance);
  return instance->resultsBuffer[graph] = simpleTest(graph, nullptr, nullptr, directed);
}
//**********************************************************************
void SimpleTest::makeSimple(Graph *graph, vector<edge> &removed, const bool directed) {
  if (SimpleTest::isSimple(graph, directed))
    return;

  SimpleTest::simpleTest(graph, &removed, &removed, directed);

  for (edge e : removed) {
    graph->delEdge(e);
  }

  assert(SimpleTest::isSimple(graph, directed));
}
//=================================================================
bool SimpleTest::simpleTest(const tlp::Graph *graph, vector<edge> *multipleEdges,
                            vector<edge> *loops, const bool directed) {
  bool result = true;
  const bool computeAll = (loops != nullptr) || (multipleEdges != nullptr);
  const bool vDiff = loops != multipleEdges;
  MutableContainer<bool> visited;
  visited.setAll(false);

  auto getEdges = getEdgesIterator(directed ? DIRECTED : UNDIRECTED);

  for (auto current : graph->nodes()) {
    // Search for multiple edges and loops
    MutableContainer<bool> targeted;
    targeted.setAll(false);

    for (auto e : getEdges(graph, current)) {

      // check if edge has already been visited
      // Take care that in makeSimple (see above) we assume that edges
      // are only processed once
      if (visited.get(e.id))
        continue;

      // mark edge as already visited
      visited.set(e.id, true);
      node target = graph->opposite(e, current);
      bool loopFound = false;

      if (target == current) { // loop
        if (!computeAll) {
          result = false;
          break;
        }

        if (loops != nullptr) {
          loopFound = true;
          loops->push_back(e);
          result = false;
        }
      }

      if (targeted.get(target.id) == true) {
        if (!computeAll) {
          result = false;
          break;
        }

        if (multipleEdges != nullptr) {
          // e is not added in multipleEdges
          // if it is already a loop and loops == multipleEdges
          if (vDiff || !loopFound)
            multipleEdges->push_back(e);
          result = false;
        }
      } else
        targeted.set(target.id, true);
    }

    if (!computeAll && !result)
      break;
  }

  return result;
}
