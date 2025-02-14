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

#ifndef TALIPOT_INTEGER_PROPERTY_H
#define TALIPOT_INTEGER_PROPERTY_H

#include <talipot/PropertyTypes.h>
#include <talipot/AbstractProperty.h>
#include <talipot/MinMaxProperty.h>
#include <talipot/NumericProperty.h>

namespace tlp {

class Graph;
class PropertyContext;

typedef MinMaxProperty<tlp::IntegerType, tlp::IntegerType, tlp::NumericProperty>
    IntegerMinMaxProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps an integer value to graph elements.
 */
class TLP_SCOPE IntegerProperty : public IntegerMinMaxProperty {

public:
  IntegerProperty(Graph *, const std::string &n = "");

  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }
  void setNodeValue(const node n, tlp::StoredType<int>::ReturnedConstValue v) override;
  void setEdgeValue(const edge e, tlp::StoredType<int>::ReturnedConstValue v) override;
  void setAllNodeValue(tlp::StoredType<int>::ReturnedConstValue v) override;

  void setValueToGraphNodes(tlp::StoredType<int>::ReturnedConstValue v,
                            const Graph *graph) override;
  void setAllEdgeValue(tlp::StoredType<int>::ReturnedConstValue v) override;
  void setValueToGraphEdges(tlp::StoredType<int>::ReturnedConstValue v,
                            const Graph *graph) override;

  int compare(const node n1, const node n2) const override;
  int compare(const edge e1, const edge e2) const override;

  // NumericProperty interface
  double getNodeDoubleValue(const node n) const override {
    return getNodeValue(n);
  }
  double getNodeDoubleDefaultValue() const override {
    return getNodeDefaultValue();
  }
  double getNodeDoubleMin(const Graph *g = nullptr) override {
    return getNodeMin(g);
  }
  double getNodeDoubleMax(const Graph *g = nullptr) override {
    return getNodeMax(g);
  }
  double getEdgeDoubleValue(const edge e) const override {
    return getEdgeValue(e);
  }
  double getEdgeDoubleDefaultValue() const override {
    return getEdgeDefaultValue();
  }
  double getEdgeDoubleMin(const Graph *g = nullptr) override {
    return getEdgeMin(g);
  }
  double getEdgeDoubleMax(const Graph *g = nullptr) override {
    return getEdgeMax(g);
  }

  void nodesUniformQuantification(unsigned int) override;

  void edgesUniformQuantification(unsigned int) override;

  NumericProperty *copyProperty(Graph *g) override {
    IntegerProperty *newProp = new IntegerProperty(g);
    newProp->copy(this);

    return newProp;
  }

  _DEPRECATED void setAllNodeValue(tlp::StoredType<int>::ReturnedConstValue v,
                                   const Graph *graph) override;
  _DEPRECATED void setAllEdgeValue(tlp::StoredType<int>::ReturnedConstValue v,
                                   const Graph *graph) override;

protected:
  void clone_handler(
      AbstractProperty<tlp::IntegerType, tlp::IntegerType, tlp::NumericProperty> &) override;

private:
  // override Observable::treatEvent
  void treatEvent(const Event &) override;
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<int> value to graph elements.
 */
class TLP_SCOPE IntegerVectorProperty
    : public AbstractVectorProperty<tlp::IntegerVectorType, tlp::IntegerType> {
public:
  IntegerVectorProperty(Graph *g, const std::string &n = "")
      : AbstractVectorProperty<IntegerVectorType, tlp::IntegerType>(g, n) {}
  // redefinition of some PropertyInterface methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }
};
#ifdef _MSC_VER
template class AbstractVectorProperty<IntegerVectorType, IntegerType>;
#endif
}
#endif // TALIPOT_INTEGER_PROPERTY_H
///@endcond
