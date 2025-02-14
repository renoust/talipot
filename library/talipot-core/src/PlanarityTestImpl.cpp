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

#if defined(__GNUC__) && __GNUC__ >= 4 && \
    ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#include <talipot/Graph.h>
#include <talipot/PlanarityTestImpl.h>
#include <talipot/MutableContainer.h>
#include <talipot/StableIterator.h>

using namespace std;
using namespace tlp;

//=================================================================
PlanarityTestImpl::PlanarityTestImpl(Graph *sg) : sg(sg) {}
//=================================================================
bool PlanarityTestImpl::isPlanar(bool embedsg) {
  if (embedsg)
    embed = true;
  else
    embed = false;

  if (sg->isEmpty()) {
    return true;
  }

  init();
  int nbOfNodes = sg->numberOfNodes();
  preProcessing(sg);
  // list of terminal nodes for each component in T[V_G-L];
  map<node, list<node>> terminalNodes;
  bool planar = true;

  for (int i = 1; i < nbOfNodes && planar; i++) {
    list<node> listOfComponents;
    node n1 = nodeWithDfsPos.get(i);
    // finds all terminal nodes and their respective components in T[V_G \ L];
    findTerminalNodes(sg, n1, listOfComponents, terminalNodes);

    for (auto comp : listOfComponents) {

      if (terminalNodes[comp].size() > 0) {
        // creates a new c-node to represent current component;
        node newCNode = sg->addNode();
        dfsPosNum.set(newCNode.id, -(++totalCNodes)); // marks as c-node;

        // does comp contain an obstruction?
        if (findObstruction(sg, n1, terminalNodes[comp])) {
          planar = false;
          break;
        }

        // else, calculates partial embedding of G;
        if (embed) {
          calculatePartialEmbedding(sg, n1, newCNode, listBackEdges[comp], terminalNodes[comp]);
        }

        // calculates RBC, label_b, etc, for new c-node;
        setInfoForNewCNode(sg, n1, newCNode, terminalNodes[comp]);
      }
    }
  }

  // embeds root with all back-edges to root;
  if (planar && embedsg)
    embedRoot(sg, nbOfNodes);

  // restores G;
  for (auto n2 : stableIterator(sg->getNodes())) {

    if (isCNode(n2))
      sg->delNode(n2, true);
  }

  restore();

#ifndef NDEBUG

  if (planar && embedsg)
    assert(isPlanarEmbedding(sg));

#endif
  return planar;
}
//=================================================================
std::list<edge> PlanarityTestImpl::getObstructions() {
  return obstructionEdges;
}
//=================================================================
void PlanarityTestImpl::restore() {
  // update obstruction edges to select only original edges
  for (auto &e : obstructionEdges) {
    if (bidirectedEdges.find(e) != bidirectedEdges.end())
      e = bidirectedEdges[e];
  }

  for (const auto &it : bidirectedEdges) {
    sg->delEdge(it.first, true);
  }
}

//=================================================================
void PlanarityTestImpl::init() {
  numberOfNodesInG = sg->numberOfNodes();
  reversalEdge.clear();
  bidirectedEdges.clear();
  obstructionEdges.clear();
  RBC.clear();
  embedList.clear();
  activeCNode.clear();
  listBackEdgesOutW.clear();
  ptrItem.setAll(nullptr);
  dfsPosNum.setAll(0);
  nodeWithDfsPos.setAll(NULL_NODE);
  parent.setAll(NULL_NODE);
  largestNeighbor.setAll(0);
  labelB.setAll(0);
  nodeLabelB.setAll(NULL_NODE);
  neighborWTerminal.setAll(NULL_NODE);
  lastVisited.setAll(NULL_NODE);
  childrenInT0.clear();
  state.setAll(NOT_VISITED);
  totalCNodes = 0;
  T0EdgeIn.setAll(NULL_EDGE);
  listBackEdges.clear();
  hasBackEdge.setAll(false);
  lastNodeInQLinha = NULL_NODE;
  p0.setAll(NULL_NODE);
  cNodeOfPossibleK33Obstruction = NULL_NODE;
  counter.setAll(0);
  makeBidirected(sg);
}
//=================================================================
void PlanarityTestImpl::findTerminalNodes(Graph *sG, node n, list<node> &listOfComponents,
                                          map<node, list<node>> &terminalNodes) {
  // to remove an element from list terminal_nodes in constant time;
  map<node, node> terminalNodesItem;
  map<node, node> componentOf;
  list<node> traversedNodes; // to restore state[u] for all traversed node u;
  list<edge> listEdges;
  componentOf[n] = NULL_NODE;
  state.set(n.id, VISITED);
  traversedNodes.push_back(n);

  for (auto e : sG->getOutEdges(n)) {
    node target = sG->target(e);
    node neighborOfN = target;

    if (dfsPosNum.get(target.id) < dfsPosNum.get(n.id) && isBackEdge(sG, e)) {
      list<node> S; // helps calculate representant of 2-connected component;

      if (embed) {
        listEdges.push_back(edgeReversal(e));
      }

      node terminalNode = NULL_NODE;

      // looks for a terminal node, upward in T;
      while (state.get(target.id) != VISITED && state.get(target.id) != TERMINAL) {

        assert(target.isValid());

        if (isCNode(parent.get(target.id))) {
          node cNode = findActiveCNode(target, n, traversedNodes);
          int count = counter.get(cNode.id) + 1;
          counter.set(cNode.id, count);
        }

        // is target a candidate for a terminal node?
        if (terminalNode == NULL_NODE && labelB.get(target.id) > dfsPosNum.get(n.id)) {
          state.set(target.id, TERMINAL);
          terminalNode = target;
        } else
          state.set(target.id, VISITED);

        traversedNodes.push_back(target);
        S.push_front(target);
        target = parent.get(target.id);
      } // end while

      // root of the component;
      if (target == n) {
        node c = S.front();
        S.pop_front();
        componentOf[c] = c;
        listOfComponents.push_back(c);
      }

      while (!S.empty()) {
        node target = S.front();
        S.pop_front();
        componentOf[target] = componentOf[parent.get(target.id)];
      }

      // did we find a terminal node?
      if (terminalNode != NULL_NODE) {
        node c = componentOf[terminalNode];
        // add the terminal node found in the terminal_nodes list
        // of its respective component;
        terminalNodes[c].push_back(terminalNode);
        terminalNodesItem[terminalNode] = terminalNode;
        neighborWTerminal.set(terminalNode.id, neighborOfN);

        // did we mark as terminal a non terminal node?
        if (state.get(target.id) == TERMINAL) {
          state.set(target.id, VISITED);
          terminalNodes[c].remove(terminalNodesItem[target]);
          lastVisited.set(terminalNode.id, lastVisited.get(target.id));

        } else
          lastVisited.set(terminalNode.id, target);
      }
    }
  }

  // groups all back-edges in T_w by their respective 2-connected component;
  for (auto e : listEdges)
    listBackEdges[componentOf[sG->source(e)]].push_back(e);

  // restores state[target] for all traversed nodes target;
  for (auto n : traversedNodes)
    state.set(n.id, NOT_VISITED);
}
//=================================================================
bool PlanarityTestImpl::findObstruction(Graph *sG, node n, list<node> &terminalNodes) {
  cNodeOfPossibleK33Obstruction = NULL_NODE; // reset global variable;

  list<node> listTerminal = terminalNodes;

  switch (terminalNodes.size()) {
  case 1: {
    node t1 = listTerminal.front();
    listTerminal.pop_front();

    if (testObstructionFromTerminalNode(sG, n, t1, n)) {
      return true;
    }

    if (cNodeOfPossibleK33Obstruction != NULL_NODE) {
      node v = findNodeWithLabelBGreaterThanDfsN(false, sG, n, cNodeOfPossibleK33Obstruction);

      if (v != NULL_NODE) {
        if (embed)
          obstructionEdgesPossibleObstrConfirmed(sG, n, t1, v);

        return true;
      }
    }
  } break;

  case 2: {
    node t1 = listTerminal.front();
    listTerminal.pop_front();
    node t2 = listTerminal.front();
    listTerminal.pop_front();
    node m = lcaBetweenTermNodes(t1, t2);
    node t12 = lastPNode(t1, m), t22 = lastPNode(t2, m);

    if (testObstructionFromTerminalNode(sG, n, t1, t12)) {
      return true;
    }

    if (cNodeOfPossibleK33Obstruction != NULL_NODE) {
      if (embed) {
        node v = t2;

        if (isCNode(t2)) {
          t2 = activeCNodeOf(false, t2);
          v = parent.get(t2.id);
          nodeLabelB.set(v.id, nodeLabelB.get(t2.id));
          labelB.set(v.id, labelB.get(t2.id));
        }

        obstructionEdgesPossibleObstrConfirmed(sG, n, t1, v);
      }

      return true;
    }

    if (testObstructionFromTerminalNode(sG, n, t2, t22))
      return true;

    if (cNodeOfPossibleK33Obstruction != NULL_NODE) {
      if (embed) {
        node v = t1;

        if (isCNode(t1)) {
          t1 = activeCNodeOf(false, t1);
          v = parent.get(t1.id);
          nodeLabelB.set(v.id, nodeLabelB.get(t1.id));
          labelB.set(v.id, labelB.get(t1.id));
        }

        obstructionEdgesPossibleObstrConfirmed(sG, n, t2, v);
      }

      return true;
    }

    node jl, jr;

    if (isCNode(m) && testCNodeCounter(sG, m, n, t12, t22, jl, jr)) {
      if (embed)
        obstructionEdgesCNodeCounter(sG, m, n, jl, jr, t1, t2);

      return true;
    }

    // ne still need to test if there is a node v in path Q_linha
    // from parent[m] to a child of w, upward in T, has a descendant
    // u which is not in Q_linha and label_b[u] > dfspos_num[w];
    node v = findNodeWithLabelBGreaterThanDfsN(true, sG, n, m);

    if (v != NULL_NODE) { // v is p-node;
      if (embed) {
        int countMin = 0;
        node vv = NULL_NODE, mm = NULL_NODE;

        if (isCNode(m)) {
          vv = lcaBetween(v, parent.get(m.id), p0);

          if (vv == parent.get(m.id)) {
            labelB.set((parent.get(m.id)).id, labelB.get(v.id));
            nodeLabelB.set((parent.get(m.id)).id, nodeLabelB.get(v.id));
            v = parent.get(m.id);
          }

          int min = labelB.get(t1.id);
          min = std::min(min, labelB.get(t2.id));
          min = std::min(min, labelB.get(v.id));

          if (min == labelB.get(t1.id))
            countMin++;

          if (min == labelB.get(t2.id))
            countMin++;

          if (min == labelB.get(v.id))
            countMin++;

          node n1 = t1, n2 = t2;

          if (isCNode(t1))
            n1 = parent.get(t1.id);

          if (isCNode(t2))
            n2 = parent.get(t2.id);

          mm = lcaBetween(n1, n2, p0);

          if (mm == t1 || mm == t2) {
            if (mm == t2) {
              swapNode(t1, t2); // makes t1 == t12;
              swapNode(t12, t22);
            }
          } else
            mm = NULL_NODE;
        }

        if (t1 == t12 && t2 == t22 && countMin >= 2 && vv == parent.get(m.id)) // m is c-node;
          obstructionEdgesK5(sG, n, m, t1, t2, NULL_NODE);
        else if (mm != NULL_NODE) { // m is c-node;
          if (t2 != t22 || vv != parent.get(m.id)) {
            node q = t22, k = parent.get(m.id);

            if (vv != parent.get(m.id)) {
              q = parent.get(m.id);
              k = v;
            }

            obstructionEdgesCountMin23(sG, n, m, t1, t2, NULL_NODE, q, k);
          } else
            obstructionEdgesCountMin1(sG, n, m, t1, t2, NULL_NODE);
        } else
          obstructionEdgesT0(sG, n, t1, t2, NULL_NODE, v);

        return true;
      }
    }
  } break;

  default: {
    // if 3 or more terminal nodes, G has an obstruction;
    if (embed) {
      node t1 = listTerminal.front();
      listTerminal.pop_front();
      node t2 = listTerminal.front();
      listTerminal.pop_front();
      node t3 = listTerminal.front();
      listTerminal.pop_front();
      node cNode, q;
      int countMin, countF;
      calcInfo3Terminals(t1, t2, t3, countMin, countF, cNode, q);

      if (countF == 3 && countMin >= 2)
        obstructionEdgesK5(sG, n, cNode, t1, t2, t3); // K5, bcycle;
      else if (countF == 3)
        obstructionEdgesCountMin1(sG, n, cNode, t1, t2, t3); // bcycle;
      else if (cNode != NULL_NODE)
        obstructionEdgesCountMin23(sG, n, cNode, t1, t2, t3, q, NULL_NODE); // bcycle;
      else
        obstructionEdgesT0(sG, n, t1, t2, t3, NULL_NODE); // T0;
    }

    return true;
  }
  }

  return false;
}

//=================================================================
void PlanarityTestImpl::setInfoForNewCNode(Graph *sG, node n, node newCNode,
                                           list<node> &terminalNodes) {
  labelB.set(newCNode.id, dfsPosNum.get(n.id));

  if (embed) // needed to calculate obstruction edges;
    nodeLabelB.set(newCNode.id, NULL_NODE);

  neighborWTerminal.set(newCNode.id, NULL_NODE);
  parent.set(newCNode.id, n);
  state.set(newCNode.id, NOT_VISITED);
  counter.set(newCNode.id, 0);
  calculateNewRBC(sG, newCNode, n, terminalNodes);

  if (lastNodeInQLinha != NULL_NODE) { // see find_node_with_label_b_greater_than_dfs_n;
    parent.set(lastNodeInQLinha.id, newCNode);
    lastNodeInQLinha = NULL_NODE; // resets global variable;
  }

  RBC[newCNode].push(n);

  BmdLink<node> *first = RBC[newCNode].firstItem();
  BmdLink<node> *last = RBC[newCNode].lastItem();
  assert(first != nullptr);
  assert(last != nullptr);
  activeCNode[first] = newCNode;
  activeCNode[last] = newCNode;
}
//=================================================================
