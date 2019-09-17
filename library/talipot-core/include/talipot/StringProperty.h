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

#ifndef TALIPOT_STRING_PROPERTY_H
#define TALIPOT_STRING_PROPERTY_H

#include <talipot/PropertyTypes.h>
#include <talipot/AbstractProperty.h>

namespace tlp {

class PropertyContext;

typedef AbstractProperty<tlp::StringType, tlp::StringType> AbstractStringProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::string value to graph elements.
 */
class TLP_SCOPE StringProperty : public AbstractStringProperty {
public:
  StringProperty(Graph *g, const std::string &n = "");

  // redefinition of some PropertyInterface methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }
  // the value size of a StringVectorProperty is not fixed
  unsigned int nodeValueSize() const override {
    return 0;
  }
  unsigned int edgeValueSize() const override {
    return 0;
  }
  int compare(const node n1, const node n2) const override;
  int compare(const edge e1, const edge e2) const override;
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<std::string> value to graph elements.
 */
class TLP_SCOPE StringVectorProperty
    : public AbstractVectorProperty<tlp::StringVectorType, tlp::StringType> {
public:
  StringVectorProperty(Graph *g, const std::string &n = "")
      : AbstractVectorProperty<StringVectorType, tlp::StringType>(g, n) {}

  // redefinition of some PropertyInterface methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }
  // the value size of a StringVectorProperty is not fixed
  unsigned int nodeValueSize() const override {
    return 0;
  }
  unsigned int edgeValueSize() const override {
    return 0;
  }
};
/*\@}*/
} // namespace tlp
#endif // TALIPOT_STRING_PROPERTY_H
