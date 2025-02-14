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

#include <talipot/GraphTools.h>
#include "TreeLeaf.h"
#include "DatasetTools.h"
#include "Orientation.h"

PLUGIN(TreeLeaf)

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // uniform layer spacing
    "If the layer spacing is uniform, the spacing between two consecutive layers will be the "
    "same."};

void TreeLeaf::computeLevelHeights(tlp::Graph *tree, tlp::node n, unsigned int depth,
                                   OrientableSizeProxy *oriSize) {
  if (levelHeights.size() == depth)
    levelHeights.push_back(0);

  float nodeHeight = oriSize->getNodeValue(n).getH();

  if (nodeHeight > levelHeights[depth])
    levelHeights[depth] = nodeHeight;

  for (auto on : tree->getOutNodes(n))
    computeLevelHeights(tree, on, depth + 1, oriSize);
}

float TreeLeaf::dfsPlacement(tlp::Graph *tree, tlp::node n, float x, float y, unsigned int depth,
                             OrientableLayout *oriLayout, OrientableSizeProxy *oriSize) {
  float minX = 0;
  float maxX = 0;
  float nodeWidth = oriSize->getNodeValue(n).getW();

  if (tree->outdeg(n) == 0) {
    oriLayout->setNodeValue(n, OrientableCoord(oriLayout, x + nodeWidth / 2, y, 0));
    return x + nodeWidth;
  }

  Iterator<node> *itN = tree->getOutNodes(n);

  float layerSpacing = minLayerSpacing;

  if (uniformLayerDistance == false) {
    if (depth < levelHeights.size() - 1) {
      layerSpacing += nodeSpacing;
      layerSpacing = max(minLayerSpacing, (levelHeights[depth] + levelHeights[depth + 1]) / 2);
    }
  }

  if (itN->hasNext()) {
    node itn = itN->next();
    minX = x;
    maxX = x = dfsPlacement(tree, itn, x, y + layerSpacing, depth + 1, oriLayout, oriSize);

    if (minX + nodeWidth > maxX)
      maxX = minX + nodeWidth;
  }

  for (; itN->hasNext();) {
    node itn = itN->next();
    x += nodeSpacing;
    x = dfsPlacement(tree, itn, x, y + layerSpacing, depth + 1, oriLayout, oriSize);

    if (x > maxX)
      maxX = x;

    if (x < minX)
      minX = x;
  }

  delete itN;
  x = (minX + maxX) / 2;
  oriLayout->setNodeValue(n, OrientableCoord(oriLayout, x, y, 0));
  return maxX;
}

TreeLeaf::TreeLeaf(const tlp::PluginContext *context) : LayoutAlgorithm(context) {
  addNodeSizePropertyParameter(this);
  addOrientationParameters(this);
  addInParameter<bool>("uniform layer spacing", paramHelp[0], "true");
  addSpacingParameters(this);
}

TreeLeaf::~TreeLeaf() {}

bool TreeLeaf::run() {
  orientationType mask = getMask(dataSet);
  OrientableLayout oriLayout(result, mask);
  SizeProperty *size;

  if (!getNodeSizePropertyParameter(dataSet, size))
    size = graph->getSizeProperty("viewSize");

  uniformLayerDistance = true;

  if (dataSet != nullptr)
    dataSet->get("uniform layer spacing", uniformLayerDistance);

  OrientableSizeProxy oriSize(size, mask);
  getSpacingParameters(dataSet, nodeSpacing, minLayerSpacing);

  if (pluginProgress)
    pluginProgress->showPreview(false);

  Graph *tree = TreeTest::computeTree(graph, pluginProgress);

  if (pluginProgress && pluginProgress->state() != TLP_CONTINUE) {
    TreeTest::cleanComputedTree(graph, tree);
    return pluginProgress->state() != TLP_CANCEL;
  }

  node root = tree->getSource();

  if (!root.isValid())
    // graph is empty
    return true;

  computeLevelHeights(tree, root, 0, &oriSize);

  // check if the specified layer spacing is greater
  // than the max of the minimum layer spacing of the tree
  if (uniformLayerDistance == true) {
    for (unsigned int i = 0; i < levelHeights.size() - 1; ++i) {
      float layerSpacing = (levelHeights[i] + levelHeights[i + 1]) / 2 + nodeSpacing;

      if (layerSpacing > minLayerSpacing)
        minLayerSpacing = layerSpacing;
    }
  }

  dfsPlacement(tree, root, 0, 0, 0, &oriLayout, &oriSize);

  TreeTest::cleanComputedTree(graph, tree);

  return true;
}
