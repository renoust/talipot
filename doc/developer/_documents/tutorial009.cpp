#include <iostream>
#include <set>
#include <talipot/TlpTools.h>
#include <talipot/Graph.h>
#include <talipot/Observable.h>
#include "ListenerClass.h"

/**
 * Tutorial 009
 *
 * Create a graph and observe its operations
 *
 */

using namespace std;
using namespace tlp;

int main() {
  // initialize the Talipot libs
  initTalipotLib();

  // create an empty graph
  Graph *graph = newGraph();

  // add an element to the graph and create the listener
  ListenerClass::launchListenerClass(graph);

  // add the nodes
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  // add the edge
  edge e0 = graph->addEdge(n0, n1);

  // run the actions to observe
  graph->reverse(e0);
  graph->delNode(n0);

  delete graph;
  return EXIT_SUCCESS;
}
