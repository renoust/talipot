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

#include <talipot/ColorProperty.h>
#include <talipot/BooleanProperty.h>
#include <talipot/SizeProperty.h>
#include <talipot/StringProperty.h>
#include <talipot/IntegerProperty.h>
#include <talipot/DoubleProperty.h>
#include <talipot/ConversionIterator.h>

#include "ParallelCoordinatesGraphProxy.h"
#include "ParallelTools.h"

using namespace std;

namespace tlp {

ParallelCoordinatesGraphProxy::ParallelCoordinatesGraphProxy(Graph *g, const ElementType location)
    : GraphDecorator(g), graphColorsChanged(false), dataLocation(location),
      unhighlightedEltsColorAlphaValue(20) {
  dataColors = graph_component->getColorProperty("viewColor");
  dataColors->addObserver(this);
  originalDataColors = new ColorProperty(graph_component);
  *originalDataColors = *(graph_component->getColorProperty("viewColor"));
}

ParallelCoordinatesGraphProxy::~ParallelCoordinatesGraphProxy() {
  dataColors->removeObserver(this);
  Observable::holdObservers();
  *dataColors = *originalDataColors;
  delete originalDataColors;
  originalDataColors = nullptr;
  Observable::unholdObservers();
}

unsigned int ParallelCoordinatesGraphProxy::getNumberOfSelectedProperties() const {
  return selectedProperties.size();
}

bool ParallelCoordinatesGraphProxy::selectedPropertiesisEmpty() const {
  return selectedProperties.empty();
}

vector<string> ParallelCoordinatesGraphProxy::getSelectedProperties() {
  vector<string> selectedPropertiesTmp;

  // check if one of the selected properties has not been deleted by an undo operation
  for (const auto &p : selectedProperties) {
    if (existProperty(p)) {
      selectedPropertiesTmp.push_back(p);
    }
  }

  selectedProperties = selectedPropertiesTmp;
  return selectedProperties;
}

void ParallelCoordinatesGraphProxy::setSelectedProperties(const vector<string> &properties) {
  selectedProperties = properties;
}

void ParallelCoordinatesGraphProxy::removePropertyFromSelection(const std::string &propertyName) {
  auto it = find(selectedProperties.begin(), selectedProperties.end(), propertyName);
  if (it != selectedProperties.end()) {
    selectedProperties.erase(it);
  }
}

ElementType ParallelCoordinatesGraphProxy::getDataLocation() const {
  return dataLocation;
}

unsigned int ParallelCoordinatesGraphProxy::getDataCount() const {
  if (getDataLocation() == NODE) {
    return numberOfNodes();
  } else {
    return numberOfEdges();
  }
}

Color ParallelCoordinatesGraphProxy::getDataColor(const unsigned int dataId) {
  return getPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId);
}

string ParallelCoordinatesGraphProxy::getDataTexture(const unsigned int dataId) {
  return getPropertyValueForData<StringProperty, StringType>("viewTexture", dataId);
}

bool ParallelCoordinatesGraphProxy::isDataSelected(const unsigned int dataId) {
  return getPropertyValueForData<BooleanProperty, BooleanType>("viewSelection", dataId);
}

void ParallelCoordinatesGraphProxy::setDataSelected(const unsigned int dataId,
                                                    const bool dataSelected) {
  setPropertyValueForData<BooleanProperty, BooleanType>("viewSelection", dataId, dataSelected);
}

Size ParallelCoordinatesGraphProxy::getDataViewSize(const unsigned int dataId) {
  return getPropertyValueForData<SizeProperty, SizeType>("viewSize", dataId);
}

string ParallelCoordinatesGraphProxy::getDataLabel(const unsigned int dataId) {
  return getPropertyValueForData<StringProperty, StringType>("viewLabel", dataId);
}

void ParallelCoordinatesGraphProxy::resetSelection() {
  setPropertyValueForAllData<BooleanProperty, BooleanType>("viewSelection", false);
}

void ParallelCoordinatesGraphProxy::deleteData(const unsigned int dataId) {
  if (getDataLocation() == NODE) {
    delNode(node(dataId));
  } else {
    delEdge(edge(dataId));
  }
}

Iterator<unsigned int> *ParallelCoordinatesGraphProxy::getDataIterator() {
  if (getDataLocation() == NODE) {
    return conversionIterator<unsigned int>(getNodes(), nodeToId);
  } else {
    return conversionIterator<unsigned int>(getEdges(), edgeToId);
  }
}

Iterator<unsigned int> *ParallelCoordinatesGraphProxy::getSelectedDataIterator() {
  BooleanProperty *viewSelection = static_cast<BooleanProperty *>(getProperty("viewSelection"));

  if (getDataLocation() == NODE) {
    return conversionIterator<unsigned int>(viewSelection->getNodesEqualTo(true, graph_component),
                                            nodeToId);
  } else {
    return conversionIterator<unsigned int>(viewSelection->getEdgesEqualTo(true, graph_component),
                                            edgeToId);
  }
}

Iterator<unsigned int> *ParallelCoordinatesGraphProxy::getUnselectedDataIterator() {
  BooleanProperty *viewSelection = static_cast<BooleanProperty *>(getProperty("viewSelection"));

  if (getDataLocation() == NODE) {
    return conversionIterator<unsigned int>(viewSelection->getNodesEqualTo(false, graph_component),
                                            nodeToId);
  } else {
    return conversionIterator<unsigned int>(viewSelection->getEdgesEqualTo(false, graph_component),
                                            edgeToId);
  }
}

void ParallelCoordinatesGraphProxy::addOrRemoveEltToHighlight(const unsigned int eltId) {
  if (isDataHighlighted(eltId)) {
    highlightedElts.erase(eltId);
  } else {
    highlightedElts.insert(eltId);
  }
}

void ParallelCoordinatesGraphProxy::unsetHighlightedElts() {
  highlightedElts.clear();
}

void ParallelCoordinatesGraphProxy::resetHighlightedElts(const set<unsigned int> &highlightedData) {
  highlightedElts.clear();

  for (auto d : highlightedData) {
    addOrRemoveEltToHighlight(d);
  }
}

bool ParallelCoordinatesGraphProxy::isDataHighlighted(const unsigned int dataId) {
  return highlightedElts.find(dataId) != highlightedElts.end();
}

bool ParallelCoordinatesGraphProxy::highlightedEltsSet() const {
  return !highlightedElts.empty();
}

void ParallelCoordinatesGraphProxy::selectHighlightedElements() {
  // initialize selection
  BooleanProperty *selectionProp = getProperty<BooleanProperty>("viewSelection");
  selectionProp->setAllNodeValue(false);
  selectionProp->setAllEdgeValue(false);

  for (auto d : highlightedElts) {
    setDataSelected(d, true);
  }
}

void ParallelCoordinatesGraphProxy::setSelectHighlightedElements(bool val) {
  // add/remove elements to/from selection
  for (auto d : highlightedElts) {
    setDataSelected(d, val);
  }
}

void ParallelCoordinatesGraphProxy::colorDataAccordingToHighlightedElts() {

  static bool lastHighlightedElementsSet = false;

  if (originalDataColors == nullptr) {
    return;
  }

  graphColorsChanged = false;

  // If new colors have been set for the graph elements, backup the change to restore the correct
  // ones when unhighlighting
  if (highlightedEltsSet()) {

    for (unsigned int dataId : getDataIterator()) {
      Color currentColor = getPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId);
      Color originalColor;

      if (getDataLocation() == NODE) {
        originalColor = originalDataColors->getNodeValue(node(dataId));
      } else {
        originalColor = originalDataColors->getEdgeValue(edge(dataId));
      }

      if (!isDataHighlighted(dataId) && currentColor.getA() != unhighlightedEltsColorAlphaValue) {
        if (getDataLocation() == NODE) {
          originalDataColors->setNodeValue(node(dataId),
                                           Color(currentColor.getR(), currentColor.getG(),
                                                 currentColor.getB(), originalColor.getA()));
        } else {
          originalDataColors->setEdgeValue(edge(dataId),
                                           Color(currentColor.getR(), currentColor.getG(),
                                                 currentColor.getB(), originalColor.getA()));
        }

        Color newColor = getOriginalDataColor(dataId);
        newColor.setA(unhighlightedEltsColorAlphaValue);
        setPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId, newColor);
      }

      if (highlightedEltsSet() && isDataHighlighted(dataId) && currentColor != originalColor) {
        if (getDataLocation() == NODE) {
          originalDataColors->setNodeValue(node(dataId),
                                           Color(currentColor.getR(), currentColor.getG(),
                                                 currentColor.getB(), originalColor.getA()));
        } else {
          originalDataColors->setEdgeValue(edge(dataId),
                                           Color(currentColor.getR(), currentColor.getG(),
                                                 currentColor.getB(), originalColor.getA()));
        }

        setPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId,
                                                          getOriginalDataColor(dataId));
      }
    }

    lastHighlightedElementsSet = true;
  } else if (lastHighlightedElementsSet) {
    *(graph_component->getColorProperty("viewColor")) = *originalDataColors;
    lastHighlightedElementsSet = false;
  } else {
    *originalDataColors = *dataColors;
  }
}

Color ParallelCoordinatesGraphProxy::getOriginalDataColor(const unsigned int dataId) {
  if (getDataLocation() == NODE) {
    return originalDataColors->getNodeValue(node(dataId));
  } else {
    return originalDataColors->getEdgeValue(edge(dataId));
  }
}

void ParallelCoordinatesGraphProxy::removeHighlightedElement(const unsigned int dataId) {
  highlightedElts.erase(dataId);
}

void ParallelCoordinatesGraphProxy::update(std::set<Observable *>::iterator begin,
                                           std::set<Observable *>::iterator end) {
  (void)begin;
  (void)end;
  graphColorsChanged = true;
}
}
