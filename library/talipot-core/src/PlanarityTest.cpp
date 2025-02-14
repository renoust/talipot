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

#include <set>

#include <talipot/Graph.h>
#include <talipot/PlanarityTest.h>
#include <talipot/PlanarityTestImpl.h>
#include <talipot/BiconnectedTest.h>

using namespace std;
using namespace tlp;
//=================================================================
class PlanarityTestListener : public Observable {
public:
  // override of Observable::treatEvent to remove the cached result for a graph if it is modified.
  void treatEvent(const Event &) override;

  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the map.
   **/
  std::unordered_map<const Graph *, bool> resultsBuffer;
};

void PlanarityTestListener::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_DEL_EDGE:
    case GraphEvent::TLP_DEL_NODE:

      if (resultsBuffer.find(graph) != resultsBuffer.end())
        if (resultsBuffer[graph])
          return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer.find(graph) != resultsBuffer.end())
        if (!resultsBuffer[graph])
          return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      break;
    }
  } else {

    Graph *graph = static_cast<Graph *>(evt.sender());

    if (evt.type() == Event::TLP_DELETE)
      resultsBuffer.erase(graph);
  }
}
//=================================================================
static PlanarityTestListener instance;
//=================================================================
bool PlanarityTest::isPlanar(Graph *graph) {
  auto itr = instance.resultsBuffer.find(graph);
  if (itr != instance.resultsBuffer.end())
    return itr->second;

  unsigned int nbOfNodes = graph->numberOfNodes();

  if (nbOfNodes == 0)
    return instance.resultsBuffer[graph] = true;

  // quick test
  if ((nbOfNodes >= 3) && (graph->numberOfEdges() > ((3 * nbOfNodes) - 6))) {
    graph->addListener(instance);
    return instance.resultsBuffer[graph] = false;
  }

  Observable::holdObservers();
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  instance.resultsBuffer[graph] = planarTest.isPlanar(true);

  for (auto e : addedEdges) {
    graph->delEdge(e, true);
  }

  Observable::unholdObservers();
  graph->addListener(instance);
  return instance.resultsBuffer[graph];
}
bool PlanarityTest::isPlanarEmbedding(const tlp::Graph *graph) {
  return PlanarityTestImpl::isPlanarEmbedding(graph);
}
//=================================================================
bool PlanarityTest::planarEmbedding(Graph *graph) {
  if (!PlanarityTest::isPlanar(graph))
    return false;

  Observable::holdObservers();
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  planarTest.isPlanar(true);

  for (auto e : addedEdges) {
    graph->delEdge(e, true);
  }

  Observable::unholdObservers();
  return true;
}
//=================================================================
list<edge> PlanarityTest::getObstructionsEdges(Graph *graph) {
  if (PlanarityTest::isPlanar(graph))
    return list<edge>();

  vector<edge> addedEdges;
  Observable::holdObservers();
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  planarTest.isPlanar(true);
  list<edge> tmpList = planarTest.getObstructions();
  for (auto e : addedEdges) {
    graph->delEdge(e, true);
  }
  Observable::unholdObservers();
  set<edge> tmpAdded(addedEdges.begin(), addedEdges.end());
  list<edge> result;

  for (auto e : tmpList) {
    if (tmpAdded.find(e) == tmpAdded.end())
      result.push_back(e);
  }

  return result;
}
