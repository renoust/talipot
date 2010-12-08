/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef GRAPHOBSERVABLE_H
#define GRAPHOBSERVABLE_H
#include <list>
#include "tulip/Node.h"
#include "tulip/Edge.h"
#include <algorithm>

namespace tlp {

class Graph;
class PropertyInterface;
class ObservableGraph;
//=========================================================

/** \addtogroup graphs */ 
/*@{*/
/// Observer for Graph
/**
 * The Observer pattern is described in the lecture notes and pp293-304 
 * of Design Patterns by Gamma, Helm, Johnson, and Vlissides. It is a 
 * framework for handling state dependency between observer and observed 
 * object.
 */
class TLP_SCOPE GraphObserver {
  std::list<ObservableGraph *> observables;
  bool updateObservables;
 public:
  GraphObserver(bool manageObservables = true)
   :updateObservables(manageObservables) {}
  virtual ~GraphObserver();
  virtual void addNode(Graph *, const node ){}
  virtual void addEdge(Graph *, const edge ){}
  virtual void beforeSetEnds(Graph *, const edge){}
  virtual void afterSetEnds(Graph *, const edge){}
  virtual void delNode(Graph *,const node ){}
  virtual void delEdge(Graph *,const edge ){}
  virtual void reverseEdge(Graph *,const edge ){}
  virtual void destroy(Graph *){}
  virtual void addSubGraph(Graph *, Graph *){}
  virtual void delSubGraph(Graph *, Graph *){}
  virtual void addLocalProperty(Graph*, const std::string&){}
  virtual void delLocalProperty(Graph*, const std::string&){}
  virtual void beforeSetAttribute(Graph*, const std::string&) {}
  virtual void afterSetAttribute(Graph*, const std::string&) {}
  virtual void removeAttribute(Graph*, const std::string&) {}

  void addObservable(ObservableGraph *);
  void removeObservable(ObservableGraph *);
};
/*@}*/
}

#ifndef DOXYGEN_NOTFOR_DEVEL
/*namespace std {
  template <>
    struct less<tlp::GraphObserver *> {
    size_t operator()(const tlp::GraphObserver * obs1,const tlp::GraphObserver *obs2) const {
      return (unsigned long)obs1<(unsigned long)obs2;
    }
  };
  }*/
#endif // DOXYGEN_NOTFOR_DEVEL

namespace tlp {

/** \addtogroup graphs */ 
/*@{*/
/**
 */
/// Observable object for graph
class  TLP_SCOPE ObservableGraph {
  friend class GraphObserver;

 public:
  virtual ~ObservableGraph() {removeGraphObservers();}
  /**
   * Register a new observer
   */
  void addGraphObserver(GraphObserver *) const;
  /**
   * Returns the number of observers
   */
  unsigned int countGraphObservers();
  /**
   * Remove an observer
   */
  void removeGraphObserver(GraphObserver *) const;
  /**
   * Remove all observers
   */
  void removeGraphObservers();

 protected:
  void notifyAddNode(Graph *,const node n);
  void notifyAddEdge(Graph *,const edge e);
  void notifyBeforeSetEnds(Graph *, const edge e);
  void notifyAfterSetEnds(Graph *,const edge e);
  void notifyDelNode(Graph *,const node n);
  void notifyDelEdge(Graph *,const edge e);
  void notifyReverseEdge(Graph *,const edge e);
  void notifyAddSubGraph(Graph*, Graph*);
  void notifyDelSubGraph(Graph*, Graph*);
  void notifyAddLocalProperty(Graph*, const std::string&);
  void notifyDelLocalProperty(Graph*, const std::string&);
  void notifyBeforeSetAttribute(Graph*, const std::string&);
  void notifyAfterSetAttribute(Graph*, const std::string&);
  void notifyRemoveAttribute(Graph*, const std::string&);
  void notifyDestroy(Graph*);
  void removeOnlyGraphObserver(GraphObserver *) const;
  mutable std::list<GraphObserver*> observers;
};
/*@}*/

inline unsigned int ObservableGraph::countGraphObservers() { 
  return observers.size(); 
}

inline void ObservableGraph::removeGraphObserver(GraphObserver *item) const {
  if (!observers.empty() && std::find(observers.begin(), observers.end(), item) != observers.end()) {
	  observers.remove(item);
	  item->removeObservable((ObservableGraph*)this);
  }
}

inline void ObservableGraph::removeOnlyGraphObserver(GraphObserver *item) const {
	if (!observers.empty() && std::find(observers.begin(), observers.end(), item) != observers.end()) {
		observers.remove(item);
	}
}

}

#endif

