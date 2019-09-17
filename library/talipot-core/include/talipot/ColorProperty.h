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

#ifndef TALIPOT_COLOR_PROPERTY_H
#define TALIPOT_COLOR_PROPERTY_H

#include <talipot/PropertyTypes.h>
#include <talipot/AbstractProperty.h>

namespace tlp {

class PropertyContext;

typedef AbstractProperty<tlp::ColorType, tlp::ColorType> AbstractColorProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps a tlp::Color value to graph elements.
 */
class TLP_SCOPE ColorProperty : public AbstractColorProperty {
public:
  ColorProperty(Graph *g, const std::string &n = "");
  // PropertyInterface inherited methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }

  int compare(const node n1, const node n2) const override;
  int compare(const edge e1, const edge e2) const override;
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<tlp::Color> value to graph elements.
 */
class TLP_SCOPE ColorVectorProperty
    : public AbstractVectorProperty<tlp::ColorVectorType, tlp::ColorType> {
public:
  ColorVectorProperty(Graph *g, const std::string &n = "")
      : AbstractVectorProperty<ColorVectorType, tlp::ColorType>(g, n) {}
  // PropertyInterface inherited methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }
};
} // namespace tlp
#endif // TALIPOT_COLOR_PROPERTY_H
