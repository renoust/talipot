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

#include "CliqueEnumeration.h"

#include <talipot/SimpleTest.h>
#include <talipot/Graph.h>

#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_map>

PLUGIN(CliqueEnumeration)

using namespace tlp;
using namespace std;

//================================================================================
CliqueEnumeration::CliqueEnumeration(tlp::PluginContext *context)
    : Algorithm(context), minsize(0), cliqueid(0) {
  addInParameter<unsigned int>("minimum size", "Clique minimum size", "0");
  addOutParameter<unsigned int>("#cliques created", "Number of cliques (subgraphs) created");
}

//================================================================================
void CliqueEnumeration::addClique(const vector<node> &clique) {
  stringstream ss;
  ss << "clique_" << cliqueid++;
  graph->inducedSubGraph(clique, graph, ss.str());
}
//================================================================================
void CliqueEnumeration::getNeighborhood(const node u, set<node> &neigh) {
  neigh.clear();
  for (auto v : graph->getInOutNodes(u))
    neigh.insert(v);
}
//================================================================================
node CliqueEnumeration::choosePivot(const set<node> &C) {
  node pivot;
  unsigned int maxinter = 0;

  for (auto c : C) {
    unsigned int inter = 0;
    for (auto v : graph->getInOutNodes(c)) {
      if (C.find(v) != C.end())
        inter++;
    }

    if (inter >= maxinter)
      pivot = c;
  }

  return pivot;
}

//================================================================================
void CliqueEnumeration::maxCliquePivot(set<node> &P, const vector<node> &R, set<node> &X) {

  set<node> C(P);
  C.insert(X.begin(), X.end());

  if (C.empty()) {
    if (R.size() >= minsize)
      addClique(R);
  } else {
    tlp::node p = choosePivot(C);
    set<node> neighp;
    getNeighborhood(p, neighp);
    set<node> tovisit;

    for (auto v : P) {
      if (neighp.find(v) == neighp.end())
        tovisit.insert(v);
    }

    for (auto v : tovisit) {
      set<node> neighx;
      getNeighborhood(v, neighx);
      set<node> newP;
      set_intersection(P.begin(), P.end(), neighx.begin(), neighx.end(),
                       inserter(newP, newP.begin()));
      vector<node> newR(R);
      newR.push_back(v);
      set<node> newX;
      set_intersection(X.begin(), X.end(), neighx.begin(), neighx.end(),
                       inserter(newX, newX.begin()));
      maxCliquePivot(newP, newR, newX);
      P.erase(v);
      X.insert(v);
    }
  }
}
//================================================================================

//================================================================================
struct DegreeOrderingElem {
  DegreeOrderingElem() : deg(0) {}
  DegreeOrderingElem(tlp::node n, unsigned int deg) : n(n), deg(deg) {}
  bool operator==(const DegreeOrderingElem &b) const {
    return n == b.n;
  }
  bool operator!=(const DegreeOrderingElem &b) const {
    return n != b.n;
  }
  tlp::node n;
  unsigned int deg;
};

//================================================================================
struct LessDegreeOrdering {
  bool operator()(const DegreeOrderingElem *u, const DegreeOrderingElem *v) const {
    if (u->deg == v->deg)
      return u->n.id < v->n.id;

    return u->deg < v->deg;
  }
};
//================================================================================
void CliqueEnumeration::getDegenerateOrdering(vector<node> &ordering) {
  ordering.clear();
  tlp::Graph *sub = graph->addCloneSubGraph();
  std::unordered_map<tlp::node, DegreeOrderingElem *> degrees;
  set<DegreeOrderingElem *, LessDegreeOrdering> sortednodes;
  for (auto n : sub->nodes()) {
    DegreeOrderingElem *elem = new DegreeOrderingElem(n, sub->deg(n));
    degrees[n] = elem;
    sortednodes.insert(elem);
  }

  while (!sortednodes.empty()) {
    auto it = sortednodes.begin();
    node n = (*it)->n;
    ordering.push_back(n);
    delete (*it);
    sortednodes.erase(it);
    for (auto v : sub->getInOutNodes(n)) {
      DegreeOrderingElem *elem = degrees.find(v)->second;
      sortednodes.erase(elem);
      elem->deg = elem->deg - 1;
      sortednodes.insert(elem);
    }
    sub->delNode(n);
  }

  graph->delSubGraph(sub);
}

//================================================================================
bool CliqueEnumeration::run() {
  if (dataSet != nullptr)
    dataSet->get("minimum size", minsize);

  vector<node> ordering;
  getDegenerateOrdering(ordering);

  for (unsigned int i = 0; i < ordering.size(); ++i) {
    set<node> neighu;
    getNeighborhood(ordering[i], neighu);
    set<node> P, X;
    vector<node> R;
    R.push_back(ordering[i]);

    for (unsigned int j = 0; j <= i; ++j)
      if (neighu.find(ordering[j]) != neighu.end())
        X.insert(ordering[j]);

    for (unsigned int j = i + 1; j < ordering.size(); ++j)
      if (neighu.find(ordering[j]) != neighu.end())
        P.insert(ordering[j]);

    maxCliquePivot(P, R, X);
  }

  if (dataSet != nullptr)
    dataSet->set("#cliques created", cliqueid);

  return true;
}
//================================================================================
bool CliqueEnumeration::check(string &erreurMsg) {
  if (!tlp::SimpleTest::isSimple(graph)) {
    erreurMsg = "The graph should be simple.";
    return false;
  }

  return true;
}
//================================================================================
