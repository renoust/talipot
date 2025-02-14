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

#ifndef TALIPOT_PROPERTY_INTERFACE_H
#define TALIPOT_PROPERTY_INTERFACE_H

#include <string>
#include <iostream>
#include <functional>

#include <talipot/config.h>
#include <talipot/Observable.h>
//#include <talipot/Node.h>
#include <talipot/Edge.h>

namespace tlp {

struct DataMem;
struct node;

class Graph;
template <class itType>
struct Iterator;

//=============================================================
/**
 * @ingroup Graph
 * @brief PropertyInterface describes the interface of a graph property.
 *
 * The intent of a property is to hold a value for each node and edge (e.g. the degree of the
 * nodes).
 *
 * A property can be used in two different ways :
 * Either it is attached to a graph; and in this case creating and deleting the property is handled
 * by the graph
 * (@see Graph::getProperty()).
 *
 * Either is is detached from a graph, and you have to handle creation and deletion yourself.
 * This is most useful for some algorithms that need a temporary property, but do not want the
 * property to appear on the graph
 * after the computation.
 */
class TLP_SCOPE PropertyInterface : public Observable {
  friend class PropertyManager;

protected:
  // name of the property when registered as a property of a graph
  std::string name;
  // the graph for whom the property is registered
  Graph *graph;

public:
  PropertyInterface();

  ~PropertyInterface() override;

  /**
   * @brief Erases the value stored for the given node.
   * The new value for the node is the default value.
   */
  virtual void erase(const node) = 0;

  /**
   * @brief Erases the value stored for the given edge.
   * The new value for the edge is the default value.
   */
  virtual void erase(const edge) = 0;

  /**
   * @brief Copies the value of a node in another property to a node in this property.
   * @param destination The node whose value will be set.
   * @param source The node whose value to copy.
   * @param property The property from which to copy the source node value.
   * @param ifNotDefault If true, the copy will only be performed if the source node's value is not
   * the default value.
   * @return True if the copy was performed, false otherwise.
   */
  virtual bool copy(const node destination, const node source, PropertyInterface *property,
                    bool ifNotDefault = false) = 0;
  /**
   * @brief Copies the value of an edge in another property to an edge in this property.
   * @param destination The edge whose value will be set.
   * @param source The edge whose value to copy.
   * @param property The property from which to copy the source edge value.
   * @param ifNotDefault If true, the copy will only be performed if the source edge's value is not
   * the default value.
   * @return True if the copy was performed, false otherwise.
   */
  virtual bool copy(const edge destination, const edge source, PropertyInterface *property,
                    bool ifNotDefault = false) = 0;

  /**
   * @brief Copies the values of the source property to this property.
   * @param source The property from which to copy values.
   * @warning Be careful when using this method, if you are interested by observing the updates of
   * the values of the current property, because no event is sent for nodes/edges whose value is the
   * default value of the source property.
   */
  virtual void copy(PropertyInterface *source) = 0;

  /**
   * @brief Creates a property of the same type (e.g. tlp::DoubleProperty) in the graph.
   * The new property will not contain a copy of this property's values.
   * @param graph The Graph in which to create the new property.
   * @param name The name of the new property.
   * @return The newly created property.
   */
  virtual PropertyInterface *clonePrototype(Graph *graph, const std::string &name) const = 0;

  /**
   * @brief Gets a string describing the type of the property (e.g. "graph", "double", "layout",
   * "string", "integer", "color", "size").
   * @return The name of this property's type.
   */
  virtual const std::string &getTypename() const = 0;

  /**
   * @brief Gets the name of the property (e.g. viewLayout).
   * @return The name of this property.
   */
  const std::string &getName() const {
    return name;
  }

  /**
   * @brief Rename a property
   * @param the new name
   * @return returns true if the renaming succeeded.
   * It may fails if a property with the given name already exists
   */
  bool rename(const std::string &newName);
  /**
   * @cond DOXYGEN_HIDDEN
   * @brief Gets the graph on which this property has been defined.
   * This is an internal function and its behavior can change.
   * DO NOT USE UNLESS YOU KNOW WHAT YOU ARE DOING.
   *
   * Be wary that is might be a different graph that the one you used to get this property.
   * For instance:
   * @code
   * Graph* g = tlp::newGraph();
   * Graph*sub = g->addCloneSubGraph();
   *
   * IntegerProperty* prop = g->getIntegerProperty("test");
   * //prop->getGraph() returns g
   *
   * IntegerProperty* prop2 = sub->getIntegerProperty("test");
   * //prop2->getGraph() returns g
   * @endcode
   *
   * This is due to the inheritance system of the properties.
   *
   * @return The Graph this property is local to.
   * @endcond
   */
  tlp::Graph *getGraph() const {
    return graph;
  }

  /**
   * @brief Gets a string representation of the node default value.
   * @param n The node to get the value of.
   * @return A string representation of the node default value.
   */
  virtual std::string getNodeStringValue(const node n) const = 0;

  /**
   * @brief Gets a string representation of the edge default value.
   * @param e The edge to get the value of.
   * @return A string representation of the edge default value.
   */
  virtual std::string getEdgeStringValue(const edge e) const = 0;

  /**
   * @brief Sets a new value on the node, represented by the string parameter.
   * @param n The node on which to set the new value.
   * @param value A string representing the value to set on the node.
   * @return Whether the string was a correct representation for this property's type. If not, the
   * value is not set.
   */
  virtual bool setNodeStringValue(const node n, const std::string &value) = 0;

  /**
   * @brief Sets a new value on the edge, represented by the string parameter.
   * @param e The edge on which to set value on.
   * @param value A string representing the value to set on the edge.
   * @return Whether the string was a correct representation for this property's type. If not, the
   * value is not set.
   */
  virtual bool setEdgeStringValue(const edge e, const std::string &value) = 0;

  /**
   * @brief Gets a string representation of the node default value.
   * @return A string representation of the node default value.
   */
  virtual std::string getNodeDefaultStringValue() const = 0;

  /**
   * @brief Gets a string representation of the edge default value.
   * @return A string representation of the edge default value.
   */
  virtual std::string getEdgeDefaultStringValue() const = 0;

  /**
   * @brief Sets the value assigned as the default one to the future added nodes from a string
   * representation.
   *
   * @param value A string representing the new value to set on future added nodes.
   *
   * @return Whether the given string was a correct representation for this property's type. If not,
   * the default value is not set.
   */
  virtual bool setNodeDefaultStringValue(const std::string &value) = 0;

  /**
   * @brief Sets all the nodes value to the value represented by the string. For some types, some
   * parsing will be necessary (e.g. LayoutProperty).
   * All previous values are lost and the represented value is assigned as the default one to the
   * future added nodes.
   *
   * @param value A string representing the new value to set on all the nodes.
   *
   * @return Whether the given string was a correct representation for this property's type. If not,
   * the values are not set.
   */
  virtual bool setAllNodeStringValue(const std::string &value) = 0;

  /**
   * @brief Sets all the nodes value to the value represented by the string for a graph. For some
   * types, some parsing will be necessary (e.g. LayoutProperty).
   * Only the nodes from that graph will have their value modified in the property
   * and the default node value will not be modified.
   *
   * @deprecated use method setStringValueToGraphNodes instead
   *
   * @param value A string representing the new value to set on all the nodes.
   * @param graph A graph that defines the set of nodes.
   *
   * @warning If the provided graph is not a descendant of the one associated to that property
   * (including itself), no node value will be modified in it.
   *
   * @return Whether the given string was a correct representation for this property's type. If not,
   * the values are not set.
   */
  _DEPRECATED virtual bool setAllNodeStringValue(const std::string &value, const Graph *graph) = 0;

  /**
   * @brief Sets all the nodes value to the value represented by the string for a graph. For some
   * types, some parsing will be necessary (e.g. LayoutProperty).
   * Only the nodes from that graph will have their value modified in the property
   * and the default node value will not be modified.
   *
   * @param value A string representing the new value to set on all the nodes.
   * @param graph A graph that defines the set of nodes.
   *
   * @warning If the provided graph is not a descendant of the one associated to that property
   * (including itself), no node value will be modified in it.
   *
   * @return Whether the given string was a correct representation for this property's type. If not,
   * the values are not set.
   */
  virtual bool setStringValueToGraphNodes(const std::string &value, const Graph *graph) = 0;

  /**
   * @brief Sets the value assigned as the default one to the future added edges from a string
   * representation.
   *
   * @param value A string representing the new value to set on future added edges.
   *
   * @return Whether the given string was a correct representation for this property's type. If not,
   * the default value is not set.
   */
  virtual bool setEdgeDefaultStringValue(const std::string &value) = 0;

  /**
   * @brief Sets all the edges value to the value represented by the string. For some types, some
   * parsing will be necessary (e.g. LayoutProperty).
   * All previous values are lost and the represented value is assigned as the default one to the
   * future added edges.
   *
   * @param value A string representing the new value to set on all the edges.
   *
   * @return Whether the given string was a correct representation for this property's type. If not,
   * the values are not set.
   */
  virtual bool setAllEdgeStringValue(const std::string &value) = 0;

  /**
   * @brief Sets all the edges value to the value represented by the string for a graph. For some
   * types, some parsing will be necessary (e.g. LayoutProperty).
   * Only the edges from that graph will have their value modified in the property
   * and the default edge value will not be modified.
   *
   * @deprecated use method setStringValueToGraphEdges instead
   *
   * @param value A string representing the new value to set on all the edges.
   * @param graph A graph that defines the set of edges.
   *
   * @warning If the provided graph is not a descendant of the one associated to that property
   * (including itself), no edge value will be modified in it.
   *
   * @return Whether the given string was a correct representation for this property's type. If not,
   * the values are not set.
   */
  _DEPRECATED virtual bool setAllEdgeStringValue(const std::string &value, const Graph *graph) = 0;

  /**
   * @brief Sets all the edges value to the value represented by the string for a graph. For some
   * types, some parsing will be necessary (e.g. LayoutProperty).
   * Only the edges from that graph will have their value modified in the property
   * and the default edge value will not be modified.
   *
   * @param value A string representing the new value to set on all the edges.
   * @param graph A graph that defines the set of edges.
   *
   * @warning If the provided graph is not a descendant of the one associated to that property
   * (including itself), no edge value will be modified in it.
   *
   * @return Whether the given string was a correct representation for this property's type. If not,
   * the values are not set.
   */
  virtual bool setStringValueToGraphEdges(const std::string &value, const Graph *graph) = 0;

  /**
   * @brief Gets a pointer to the tlp::DataMem structure that contains the node default value.
   * @return The DataMem structure containing the node default value.
   * @warning The ownership of this pointer is given to the caller.
   */
  virtual DataMem *getNodeDefaultDataMemValue() const = 0;

  /**
   * @brief Gets a pointer to the tlp::DataMem structure that contains the edge default value.
   * @return The DataMem structure containing the edge default value.
   * @warning The ownership of this pointer is given to the caller.
   */
  virtual DataMem *getEdgeDefaultDataMemValue() const = 0;

  /**
   * @brief Sets all the nodes value to the value contained in the given DataMem structure.
   * All previous values are lost.
   * @param value The value to set on all the nodes.
   */
  virtual void setAllNodeDataMemValue(const DataMem *value) = 0;

  /**
   * @brief Sets all the edges value to the value contained in the given DataMem structure.
   * All previous values are lost.
   * @param value The value to set on all the edges.
   */
  virtual void setAllEdgeDataMemValue(const DataMem *v) = 0;

  /**
   * @brief Gets the node value, contained in a tlp::DataMem structure.
   * @param n The node to get the value of.
   * @return The value of the node, in a tlp::DataMem.
   *
   * @warning The ownership of this pointer is given to the caller.
   */
  virtual DataMem *getNodeDataMemValue(const node n) const = 0;

  /**
   * @brief Gets the edge value, contained in a tlp::DataMem structure.
   * @param n The edge to get the value of.
   * @return The value of the edge, in a tlp::DataMem.
   *
   * @warning The ownership of this pointer is given to the caller.
   */
  virtual DataMem *getEdgeDataMemValue(const edge e) const = 0;

  /**
   * @brief Returns the value in a DataMem if it is not default, otherwise returns nullptr.
   * @param n The node to get the value of.
   * @return The value of the node if it is not default, or nullptr.
   *
   * @warning The ownership of this pointer is given to the caller.
   */
  virtual DataMem *getNonDefaultDataMemValue(const node n) const = 0;

  /**
   * @brief Returns the value in a DataMem if it is not default, otherwise returns nullptr.
   * @param e The edge to get the value of.
   * @return The value of the edge if it is not default, or nullptr.
   *
   * @warning The ownership of this pointer is given to the caller.
   */
  virtual DataMem *getNonDefaultDataMemValue(const edge e) const = 0;

  /**
   * @brief Sets the node value.
   * @param n The node to set the value of.
   * @param value The value to set to this node.
   */
  virtual void setNodeDataMemValue(const node n, const DataMem *value) = 0;

  /**
   * @brief Sets the edge value.
   * @param e The edge to set the value of.
   * @param value The value to set to this edge.
   */
  virtual void setEdgeDataMemValue(const edge e, const DataMem *v) = 0;

  /**
   * @brief Gets an Iterator on all non-default valuated nodes.
   * When given a Graph as parameter, only nodes belonging to this graph are iterated over.
   * @return An Iterator over nodes whose value is not default.
   *
   * @warning The ownership of the iterator is given to the caller.
   */
  virtual tlp::Iterator<node> *getNonDefaultValuatedNodes(const Graph * = nullptr) const = 0;

  /**
   * @brief Returns whether the property has nodes with a non default value.
   * When given a Graph as parameter, only the nodes with a non default value belonging to
   * this graph are taken into account.
   * @return false if all nodes has the default value, true if not.
   *
   */
  virtual bool hasNonDefaultValuatedNodes(const Graph * = nullptr) const = 0;

  /**
   * @brief Returns the number of nodes with a non default value.
   * When given a Graph as parameter, only the number of nodes with a non default value belonging to
   * this graph is returned.
   * @return the number of nodes with a non default value.
   *
   */
  virtual unsigned int numberOfNonDefaultValuatedNodes(const Graph * = nullptr) const = 0;

  /**
   * @brief Gets an Iterator on all non-default valuated edges.
   * When given a Graph as parameter, only edges belonging to this graph are iterated over.
   * @return An Iterator over edges whose value is not default.
   *
   * @warning The ownership of the iterator is given to the caller.
   */
  virtual tlp::Iterator<edge> *getNonDefaultValuatedEdges(const Graph * = nullptr) const = 0;

  /**
   * @brief Returns whether the property has edges with a non default value.
   * When given a Graph as parameter, only the edges with a non default value belonging to
   * this graph are taken into account.
   * @return false if all edges has the default value, true if not.
   *
   */
  virtual bool hasNonDefaultValuatedEdges(const Graph * = nullptr) const = 0;

  /**
   * @brief Returns the number of edges with a non default value.
   * @return the number of edges with a non default value.
   *
   */
  virtual unsigned int numberOfNonDefaultValuatedEdges(const Graph * = nullptr) const = 0;

  /**
   * @brief Returns the size in bytes of a node's value.
   * @return the size of a node's value (0 means the size is not fixed)
   *
   */
  virtual unsigned int nodeValueSize() const = 0;

  /**
   * @brief Writes the nodes default value
   *
   */
  virtual void writeNodeDefaultValue(std::ostream &) const = 0;

  /**
   * @brief Writes the value of a node
   *
   */
  virtual void writeNodeValue(std::ostream &, node) const = 0;

  /**
   * @brief Reads the nodes default value
   *
   */
  virtual bool readNodeDefaultValue(std::istream &) = 0;

  /**
   * @brief Reads the value of a node
   *
   */
  virtual bool readNodeValue(std::istream &, node) = 0;

  /**
   * @brief Returns the size in bytes of an edge's value.
   * @return the size of a node's value (0 means the size is not fixed)
   *
   */
  virtual unsigned int edgeValueSize() const = 0;

  /**
   * @brief Writes the edges default value
   *
   */
  virtual void writeEdgeDefaultValue(std::ostream &) const = 0;

  /**
   * @brief Writes the value of an edge
   *
   */
  virtual void writeEdgeValue(std::ostream &, edge) const = 0;

  /**
   * @brief Reads the edges default value
   *
   */
  virtual bool readEdgeDefaultValue(std::istream &) = 0;

  /**
   * @brief Reads the value of an edge
   *
   */
  virtual bool readEdgeValue(std::istream &, edge) = 0;

  /**
   * @brief Sets the value of the metanode to a computed value.
   *
   * The value is computed by this property's MetaValueCalculator.
   * @param metaNode The metanode to compute a value on.
   * @param subgraph The subgraph pointed by the metanode.
   * @param metaGraph The graph who owns the meta node.
   */
  virtual void computeMetaValue(node metaNode, Graph *subgraph, Graph *metaGraph) = 0;

  /**
   * @brief Sets the value of the metaedge to a computed value.
   * @param metaEdge The meta edge to compute a value on.
   * @param it The edges represented by the meta edge.
   * @param metaGraph The graph who owns the meta edge.
   */
  virtual void computeMetaValue(edge metaEdge, tlp::Iterator<edge> *it, Graph *metaGraph) = 0;

  /**
   * @brief Base class for computing values on meta nodes and edges.
   */
  class MetaValueCalculator {
  public:
    virtual ~MetaValueCalculator() {}
  };

  /**
   * @brief Gets the MetaValueCalculator of this property.
   * @return The MetaValueCalculator of this property
   */
  MetaValueCalculator *getMetaValueCalculator() {
    return metaValueCalculator;
  }

  /**
   * @brief Sets the Calculator for meta nodes and edges.
   * @param calculator The object containing the logic for computing the meta values for the nodes
   * and edges.
   *
   * @warning The ownership of the MetaValueCalculator is not taken by the property.
   */
  virtual void setMetaValueCalculator(MetaValueCalculator *calculator) {
    metaValueCalculator = calculator;
  }

  /**
   * @brief Compares the value this property holds for the two given nodes.
   * @param n1 The first node to compare the value of.
   * @param n2 The second node to compare the value of.
   * @return 0 if the values are identical, a positive value if n1 is greater than n2, and a
   * negative value if n1 is less than n2.
   */
  virtual int compare(const node n1, const node n2) const = 0;

  /**
   * @brief Compares the value this property holds for the two given edges.
   * @param e1 The first edge to compare the value of.
   * @param e2 The second edge to compare the value of.
   * @return 0 if the values are identical, a positive value if e1 is greater than e2, and a
   * negative value if e1 is less than e2.
   */
  virtual int compare(const edge e1, const edge e2) const = 0;

protected:
  MetaValueCalculator *metaValueCalculator;

  // for notification of PropertyObserver
  void notifyBeforeSetNodeValue(const node n);
  void notifyAfterSetNodeValue(const node n);
  void notifyBeforeSetEdgeValue(const edge e);
  void notifyAfterSetEdgeValue(const edge e);
  void notifyBeforeSetAllNodeValue();
  void notifyAfterSetAllNodeValue();
  void notifyBeforeSetAllEdgeValue();
  void notifyAfterSetAllEdgeValue();
  void notifyDestroy();
  void notifyRename(const std::string &newName);
};

/**
 * @ingroup Graph
 * @brief VectorPropertyInterface describes the interface of a graph property whose holded value is
 * a vector (std::vector)
 *
 */
class TLP_SCOPE VectorPropertyInterface : public PropertyInterface {
public:
  VectorPropertyInterface() : PropertyInterface() {}

  ~VectorPropertyInterface() override {}

  /**
   * @brief split an input string into a vector of strings
   * @param str A string listing the elements of the vector to set on a node/edge.
   * @param vect An output vector containing the string elements
   * @param openChar an optional character opening the list of elements. Default value is '('; when
   * set to '\0' it indicates that there is no opening character.
   * @param sepChar an optional character separing the elements of the list. Default value is ','.
   * @param closeChar an optional character closing the list of elements. Default value is ')'; when
   * set to '\0' it indicates that there is no opening character.
   * @return Whether the string was a correct representation for this property's type.
   */
  virtual bool tokenize(const std::string &str, std::vector<std::string> &vect, char openChar = '(',
                        char sepChar = ',', char closeChar = ')') = 0;

  /**
   * @brief Sets a new vector represented by the vector of string parameter as the node value.
   * @param n The node on which to set the new value.
   * @param values A vector of strings listing the string representations of elements of the vector
   * to set on the node.
   * @return Whether the vector was a correct representation for this property's type. If not, the
   * value is not set.
   */
  virtual bool setNodeStringValueAsVector(const node n, const std::vector<std::string> &values) = 0;

  /**
   * @brief Sets a new vector represented by the vector of string parameter as the edge value.
   * @param e The edge on which to set the new value.
   * @param values A vector of strings listing the string representations of elements of the vector
   * to set on the edge.
   * @return Whether the vector was a correct representation for this property's type. If not, the
   * value is not set.
   */
  virtual bool setEdgeStringValueAsVector(const edge e, const std::vector<std::string> &values) = 0;

  /**
   * @brief Sets a new vector represented by the string parameter as the node value.
   * @param n The node on which to set the new value.
   * @param value A string listing the elements of the vector to set on the node.
   * @param openChar an optional character opening the list of elements. Default value is '('; when
   * set to '\0' it indicates that there is no opening character.
   * @param sepChar an optional character separing the elements of the list. Default value is ','.
   * @param closeChar an optional character closing the list of elements. Default value is ')'; when
   * set to '\0' it indicates that there is no opening character.
   * @return Whether the string was a correct representation for this property's type. If not, the
   * value is not set.
   */
  virtual bool setNodeStringValueAsVector(const node n, const std::string &value,
                                          char openChar = '(', char sepChar = ',',
                                          char closeChar = ')') = 0;

  /**
   * @brief Sets a new vector represented by the string parameter as the edge value.
   * @param e The edge on which to set value on.
   * @param value A string listing the elements of the vector to set on the edge.
   * @param openChar an optional character opening the list of elements. Default value is '('; when
   * set to '\0' it indicates that there is no opening character.
   * @param sepChar an optional character separing the elements of the list. Default value is ','.
   * @param closeChar an optional character closing the list of elements. Default value is ')'; when
   * set to '\0' it indicates that there is no opening character.
   * @return Whether the string was a correct representation for this property's type. If not, the
   * value is not set.
   */
  virtual bool setEdgeStringValueAsVector(const edge e, const std::string &value,
                                          char openChar = '(', char sepChar = ',',
                                          char closeChar = ')') = 0;
};

/**
 * @ingroup Observation
 * @brief Contains additional information about events on a property,
 * such as the property it happened on, the node/edge eventually concerned and such.
 * It also contains the detailed type of the event.
 */
class TLP_SCOPE PropertyEvent : public Event {
public:
  // be careful about the ordering of the constants
  // in the enum below because it is used in some assertions
  enum PropertyEventType {
    TLP_BEFORE_SET_NODE_VALUE = 0,
    TLP_AFTER_SET_NODE_VALUE,
    TLP_BEFORE_SET_ALL_NODE_VALUE,
    TLP_AFTER_SET_ALL_NODE_VALUE,
    TLP_BEFORE_SET_ALL_EDGE_VALUE,
    TLP_AFTER_SET_ALL_EDGE_VALUE,
    TLP_BEFORE_SET_EDGE_VALUE,
    TLP_AFTER_SET_EDGE_VALUE
  };
  PropertyEvent(const PropertyInterface &prop, PropertyEventType propEvtType,
                Event::EventType evtType = Event::TLP_MODIFICATION, unsigned int id = UINT_MAX)
      : Event(prop, evtType), evtType(propEvtType), eltId(id) {}

  PropertyInterface *getProperty() const {
    return static_cast<PropertyInterface *>(sender());
  }

  node getNode() const {
    assert(evtType < TLP_BEFORE_SET_ALL_NODE_VALUE);
    return node(eltId);
  }

  edge getEdge() const {
    assert(evtType > TLP_AFTER_SET_ALL_EDGE_VALUE);
    return edge(eltId);
  }

  PropertyEventType getType() const {
    return evtType;
  }

protected:
  PropertyEventType evtType;
  unsigned int eltId;
};
}

//================================================================================
// these functions allow to use tlp::PropertyInterface as a key in a hash-based data structure (e.g.
// hashmap).
//================================================================================

///@cond DOXYGEN_HIDDEN
namespace std {
template <>
struct TLP_SCOPE hash<const tlp::PropertyInterface *> {
  size_t operator()(const tlp::PropertyInterface *prop) const {
    return size_t(prop);
  }
};
template <>
struct TLP_SCOPE hash<tlp::PropertyInterface *> {
  size_t operator()(tlp::PropertyInterface *prop) const {
    return size_t(prop);
  }
};
} // namespace std
///@endcond

#endif // TALIPOT_PROPERTY_INTERFACE_H
