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

#include <iomanip>
#include <fstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#include <talipot/StlIterator.h>
#include <talipot/TlpTools.h>
#include <talipot/Graph.h>
#include <talipot/GraphImpl.h>
#include <talipot/BooleanProperty.h>
#include <talipot/ColorProperty.h>
#include <talipot/DoubleProperty.h>
#include <talipot/IntegerProperty.h>
#include <talipot/GraphProperty.h>
#include <talipot/LayoutProperty.h>
#include <talipot/SizeProperty.h>
#include <talipot/StringProperty.h>
#include <talipot/DataSet.h>
#include <talipot/ExportModule.h>
#include <talipot/Algorithm.h>
#include <talipot/ImportModule.h>
#include <talipot/SimplePluginProgress.h>
#include <talipot/BoundingBox.h>
#include <talipot/DrawingTools.h>
#include <talipot/ViewSettings.h>
#include <talipot/VectorGraph.h>
#include <talipot/FontAwesome.h>

using namespace std;
using namespace tlp;

ostream &operator<<(ostream &os, const Graph *graph) {
  os << ";(nodes <node_id> <node_id> ...)" << endl;
  os << "(nodes ";

  if (graph->numberOfNodes()) {
    node beginNode, previousNode;
    beginNode = previousNode = graph->getOneNode();
    os << beginNode.id;
    unsigned int i = 0;

    for (auto current : graph->nodes()) {

      if (current.id == previousNode.id + 1) {
        previousNode = current;

        if (++i == graph->numberOfNodes())
          os << ".." << current.id;
      } else {
        if (previousNode != beginNode) {
          os << ".." << previousNode.id;
        }

        os << " " << current.id;
        beginNode = previousNode = current;
      }
    }
  }

  os << ")" << endl;
  os << ";(edge <edge_id> <source_id> <target_id>)" << endl;

  for (auto e : graph->edges()) {
    auto ends = graph->ends(e);
    os << "(edge " << e.id << " " << ends.first.id << " " << ends.second.id << ")" << endl;
  }

  return os;
}

static void setViewPropertiesDefaults(Graph *g) {

  const std::string shapes = "viewShape", colors = "viewColor", sizes = "viewSize",
                    metrics = "viewMetric", fonts = "viewFont",
                    fontAwesomeIcon = "viewFontAwesomeIcon", fontSizes = "viewFontSize",
                    borderWidth = "viewBorderWidth", borderColor = "viewBorderColor",
                    tgtShape = "viewTgtAnchorShape", srcShape = "viewSrcAnchorShape",
                    icon = "viewIcon", labelColor = "viewLabelColor",
                    labelBorderColor = "viewLabelBorderColor",
                    labelBorderWidth = "viewLabelBorderWidth", labelPosition = "viewLabelPosition",
                    label = "viewLabel", layout = "viewLayout", rotation = "viewRotation",
                    srcAnchorSize = "viewSrcAnchorSize", selection = "viewSelection",
                    texture = "viewTexture", tgtAnchorSize = "viewTgtAnchorSize";

  if (!g->existProperty(shapes)) {
    g->getIntegerProperty(shapes)->setAllNodeValue(ViewSettings::instance().defaultShape(NODE));
    g->getIntegerProperty(shapes)->setAllEdgeValue(ViewSettings::instance().defaultShape(EDGE));
  }

  if (!g->existProperty(colors)) {
    g->getColorProperty(colors)->setAllNodeValue(ViewSettings::instance().defaultColor(NODE));
    g->getColorProperty(colors)->setAllEdgeValue(ViewSettings::instance().defaultColor(EDGE));
  }

  if (!g->existProperty(sizes)) {
    g->getSizeProperty(sizes)->setAllNodeValue(ViewSettings::instance().defaultSize(NODE));
    g->getSizeProperty(sizes)->setAllEdgeValue(ViewSettings::instance().defaultSize(EDGE));
  }

  if (!g->existProperty(metrics)) {
    g->getDoubleProperty(metrics)->setAllNodeValue(0);
    g->getDoubleProperty(metrics)->setAllEdgeValue(0);
  }

  if (!g->existProperty(fonts)) {
    g->getStringProperty(fonts)->setAllNodeValue(ViewSettings::instance().defaultFontFile());
    g->getStringProperty(fonts)->setAllEdgeValue(ViewSettings::instance().defaultFontFile());
  }

  if (!g->existProperty(fontSizes)) {
    g->getIntegerProperty(fontSizes)->setAllNodeValue(ViewSettings::instance().defaultFontSize());
    g->getIntegerProperty(fontSizes)->setAllEdgeValue(ViewSettings::instance().defaultFontSize());
  }

  if (!g->existProperty(borderWidth)) {
    g->getDoubleProperty(borderWidth)
        ->setAllNodeValue(ViewSettings::instance().defaultBorderWidth(NODE));
    g->getDoubleProperty(borderWidth)
        ->setAllEdgeValue(ViewSettings::instance().defaultBorderWidth(EDGE));
  }

  if (!g->existProperty(borderColor)) {
    g->getColorProperty(borderColor)
        ->setAllNodeValue(ViewSettings::instance().defaultBorderColor(NODE));
    g->getColorProperty(borderColor)
        ->setAllEdgeValue(ViewSettings::instance().defaultBorderColor(EDGE));
  }

  if (!g->existProperty(tgtShape)) {
    g->getIntegerProperty(tgtShape)->setAllEdgeValue(
        ViewSettings::instance().defaultEdgeExtremityTgtShape());
  }

  if (!g->existProperty(srcShape)) {
    g->getIntegerProperty(srcShape)->setAllEdgeValue(
        ViewSettings::instance().defaultEdgeExtremitySrcShape());
  }

  if (!g->existProperty(labelColor)) {
    g->getColorProperty(labelColor)->setAllNodeValue(ViewSettings::instance().defaultLabelColor());
    g->getColorProperty(labelColor)->setAllEdgeValue(ViewSettings::instance().defaultLabelColor());
  }

  if (!g->existProperty(labelBorderColor)) {
    g->getColorProperty(labelBorderColor)
        ->setAllNodeValue(ViewSettings::instance().defaultLabelBorderColor());
    g->getColorProperty(labelBorderColor)
        ->setAllEdgeValue(ViewSettings::instance().defaultLabelBorderColor());
  }

  if (!g->existProperty(labelBorderWidth)) {
    g->getDoubleProperty(labelBorderWidth)
        ->setAllNodeValue(ViewSettings::instance().defaultLabelBorderWidth());
    g->getDoubleProperty(labelBorderWidth)
        ->setAllEdgeValue(ViewSettings::instance().defaultLabelBorderWidth());
  }

  if (!g->existProperty(labelPosition)) {
    g->getIntegerProperty(labelPosition)
        ->setAllNodeValue(ViewSettings::instance().defaultLabelPosition());
    g->getIntegerProperty(labelPosition)
        ->setAllEdgeValue(ViewSettings::instance().defaultLabelPosition());
  }

  if (!g->existProperty(layout)) {
    g->getLayoutProperty(layout)->setAllNodeValue(Coord(0, 0, 0));
    g->getLayoutProperty(layout)->setAllEdgeValue(std::vector<Coord>());
  }

  if (!g->existProperty(rotation)) {
    g->getDoubleProperty(rotation)->setAllNodeValue(0);
    g->getDoubleProperty(rotation)->setAllEdgeValue(0);
  }

  if (!g->existProperty(srcAnchorSize)) {
    g->getSizeProperty(srcAnchorSize)
        ->setAllEdgeValue(ViewSettings::instance().defaultEdgeExtremitySrcSize());
  }

  if (!g->existProperty(tgtAnchorSize)) {
    g->getSizeProperty(tgtAnchorSize)
        ->setAllEdgeValue(ViewSettings::instance().defaultEdgeExtremityTgtSize());
  }

  if (!g->existProperty(texture)) {
    g->getStringProperty(texture)->setAllNodeValue("");
    g->getStringProperty(texture)->setAllEdgeValue("");
  }

  if (!g->existProperty(label)) {
    g->getStringProperty(label)->setAllNodeValue("");
    g->getStringProperty(label)->setAllEdgeValue("");
  }

  if (!g->existProperty(selection)) {
    g->getBooleanProperty(selection)->setAllNodeValue(false);
    g->getBooleanProperty(selection)->setAllEdgeValue(false);
  }

  if (!g->existProperty(icon)) {
    g->getStringProperty(icon)->setAllNodeValue(FontAwesome::Solid::QuestionCircle);
    g->getStringProperty(icon)->setAllEdgeValue(FontAwesome::Solid::QuestionCircle);
  }
}

//=========================================================
Graph *tlp::newGraph() {
  Graph *g = new GraphImpl();
  setViewPropertiesDefaults(g);
  return g;
}
//=========================================================
Graph *tlp::loadGraph(const std::string &filename, PluginProgress *progress) {
  DataSet dataSet;
  std::string importPluginName = "TLP Import";

  list<string> importPlugins = PluginsManager::availablePlugins<ImportModule>();

  for (const string &pluginName : importPlugins) {
    const ImportModule &importPlugin =
        static_cast<const ImportModule &>(PluginsManager::pluginInformation(pluginName));
    list<string> extensions(importPlugin.fileExtensions());

    for (const string &ext : extensions)
      if (filename.rfind(ext) == (filename.size() - ext.size())) {
        importPluginName = importPlugin.name();
        break;
      }

    extensions = importPlugin.gzipFileExtensions();
    for (const string &ext : extensions)
      if (filename.rfind(ext) == (filename.size() - ext.size())) {
        importPluginName = importPlugin.name();
        break;
      }
  }

  dataSet.set("file::filename", filename);
  Graph *graph = tlp::importGraph(importPluginName, dataSet, progress);
  return graph;
}
//=========================================================
bool tlp::saveGraph(Graph *graph, const std::string &filename, PluginProgress *progress,
                    DataSet *data) {
  ostream *os;

  bool gzip = false;

  string exportPluginName;
  list<string> exportPlugins = PluginsManager::availablePlugins<ExportModule>();

  for (const string &pluginName : exportPlugins) {
    ExportModule *exportPlugin = PluginsManager::getPluginObject<ExportModule>(pluginName);
    string ext(exportPlugin->fileExtension());

    if (filename.rfind(ext) != string::npos &&
        filename.rfind(ext) == (filename.length() - ext.length())) {
      exportPluginName = exportPlugin->name();
      delete exportPlugin;
      break;
    } else {
      list<string> extensions(exportPlugin->gzipFileExtensions());

      for (const string &zext : exportPlugin->gzipFileExtensions()) {
        if (filename.rfind(zext) == filename.length() - zext.length()) {
          exportPluginName = exportPlugin->name();
          gzip = true;
          break;
        }
      }
      delete exportPlugin;
      if (gzip)
        break;
    }
  }

  if (exportPluginName.empty()) {
    string str("No suitable export plugin found. Exporting in TLP format.");

    if (progress)
      progress->setError(str);

    tlp::warning() << str << std::endl;
    exportPluginName = "TLP Export";
  }

  if (gzip) {
    os = tlp::getOgzstream(filename);
  } else {
    std::ios_base::openmode openMode = ios::out;

    if (exportPluginName == "TLPB Export") {
      openMode |= ios::binary;
    }

    os = tlp::getOutputFileStream(filename, openMode);
  }

  bool result;
  DataSet ds;

  if (data != nullptr)
    ds = *data;

  ds.set("file", filename);
  result = tlp::exportGraph(graph, *os, exportPluginName, ds, progress);
  delete os;
  return result;
}
//=========================================================
Graph *tlp::importGraph(const std::string &format, DataSet &dataSet, PluginProgress *progress,
                        Graph *graph) {

  if (!PluginsManager::pluginExists(format)) {
    tlp::warning() << __FUNCTION__ << ": import plugin \"" << format
                   << "\" does not exist (or is not loaded)" << endl;
    return nullptr;
  }

  bool newGraphP = false;

  if (graph == nullptr) {
    graph = tlp::newGraph();
    newGraphP = true;
  }

  PluginProgress *tmpProgress;
  bool deletePluginProgress = false;

  if (progress == nullptr) {
    tmpProgress = new SimplePluginProgress();
    deletePluginProgress = true;
  } else
    tmpProgress = progress;

  AlgorithmContext *tmp = new AlgorithmContext(graph, &dataSet, tmpProgress);
  ImportModule *newImportModule = PluginsManager::getPluginObject<ImportModule>(format, tmp);
  assert(newImportModule != nullptr);

  // ensure that the parsing of float or double does not depend on locale
  setlocale(LC_NUMERIC, "C");

  // If the import failed and we created the graph then delete the graph
  if (!newImportModule->importGraph()) {
    if (newGraphP)
      delete graph;

    graph = nullptr;
  } else {
    std::string filename;

    if (dataSet.get("file::filename", filename)) {
      graph->setAttribute("file", filename);
    }

    setViewPropertiesDefaults(graph);
  }

  if (deletePluginProgress)
    delete tmpProgress;

  delete newImportModule;
  dataSet = *tmp->dataSet;
  delete tmp;

  return graph;
}
//=========================================================
bool tlp::exportGraph(Graph *graph, std::ostream &outputStream, const std::string &format,
                      DataSet &dataSet, PluginProgress *progress) {
  if (!PluginsManager::pluginExists(format)) {
    tlp::warning() << __FUNCTION__ << ": export plugin \"" << format
                   << "\" does not exist (or is not loaded)" << endl;
    return false;
  }

  bool result;
  bool deletePluginProgress = false;
  PluginProgress *tmpProgress = nullptr;

  if (progress == nullptr) {
    tmpProgress = new SimplePluginProgress();
    deletePluginProgress = true;
  } else
    tmpProgress = progress;

  AlgorithmContext *context = new AlgorithmContext(graph, &dataSet, tmpProgress);
  ExportModule *newExportModule = PluginsManager::getPluginObject<ExportModule>(format, context);
  assert(newExportModule != nullptr);
  std::string filename;

  if (dataSet.get("file", filename)) {
    graph->setAttribute("file", filename);
  }

  result = newExportModule->exportGraph(outputStream);

  if (deletePluginProgress)
    delete tmpProgress;

  delete newExportModule;
  delete context;
  return result;
}
//=========================================================
static void removeFromGraph(Graph *g, const vector<node> &nodes, const std::vector<edge> &edges) {
  unsigned int nbNodes = nodes.size();
  unsigned int nbEdges = edges.size();

  // Clean properties
  for (auto p : g->getObjectProperties()) {
    for (unsigned int i = 0; i < nbNodes; i++)
      p->erase(nodes[i]);

    for (unsigned int i = 0; i < nbEdges; i++)
      p->erase(edges[i]);
  }

  // Remove edges
  for (unsigned int i = 0; i < nbEdges; i++)
    g->delEdge(edges[i]);

  // Remove nodes
  for (unsigned int i = 0; i < nbNodes; i++)
    g->delNode(nodes[i]);
}

void tlp::removeFromGraph(Graph *ioG, BooleanProperty *inSel) {
  if (!ioG)
    return;

  if (!inSel) {
    ::removeFromGraph(ioG, ioG->nodes(), ioG->edges());
    return;
  }

  vector<edge> edgeA;

  // get edges
  for (auto e : ioG->edges()) {
    if (inSel->getEdgeValue(e)) {
      // selected edge -> remove it !
      edgeA.push_back(e);
    } else {
      // unselected edge -> don't remove node ends !
      auto ends = ioG->ends(e);
      inSel->setNodeValue(ends.first, false);
      inSel->setNodeValue(ends.second, false);
    }
  }

  vector<node> nodeA;
  // get nodes
  for (auto n : ioG->nodes()) {
    if (inSel->getNodeValue(n)) {
      // selected node -> remove it !
      nodeA.push_back(n);
    }
  }

  ::removeFromGraph(ioG, nodeA, edgeA);
}

void tlp::copyToGraph(Graph *outG, const Graph *inG, BooleanProperty *inSel,
                      BooleanProperty *outSel) {
  if (outSel) {
    outSel->setAllNodeValue(false);
    outSel->setAllEdgeValue(false);
  }

  if (!outG || !inG)
    return;

  // extend the selection to edge ends
  if (inSel) {
    for (auto e : inSel->getNonDefaultValuatedEdges(inG)) {
      auto eEnds = inG->ends(e);
      inSel->setNodeValue(eEnds.first, true);
      inSel->setNodeValue(eEnds.second, true);
    }
  }

  // the selected nodes
  Iterator<node> *nodeIt = nullptr;
  // the number of selected nodes
  unsigned int nbSelNodes = 0;

  if (inSel) {
    nodeIt = inSel->getNonDefaultValuatedNodes(inG);
    if (!nodeIt->hasNext()) {
      delete nodeIt;
      return;
    }
    nbSelNodes = inSel->numberOfNonDefaultValuatedNodes(inG);
  } else {
    nodeIt = inG->getNodes();
    if (!nodeIt->hasNext()) {
      delete nodeIt;
      return;
    }
    nbSelNodes = inG->numberOfNodes();
  }

  // reserve space for nodes
  outG->reserveNodes(outG->numberOfNodes() + nbSelNodes);

  // get properties
  std::vector<std::pair<PropertyInterface *, PropertyInterface *>> properties;
  for (PropertyInterface *src : inG->getObjectProperties()) {
    if (dynamic_cast<GraphProperty *>(src) == nullptr) {
      const std::string &pName = src->getName();
      PropertyInterface *dst =
          outG->existProperty(pName) ? outG->getProperty(pName) : src->clonePrototype(outG, pName);
      properties.push_back(std::make_pair(src, dst));
    }
  }
  unsigned int nbProperties = properties.size();

  MutableContainer<node> nodeTrl;
  nodeTrl.setAll(node());
  // loop on nodes
  for (auto nIn : nodeIt) {
    // add outG corresponding node
    node nOut = outG->addNode();

    // select added node
    if (outSel)
      outSel->setNodeValue(nOut, true);

    // add to translation tab
    nodeTrl.set(nIn.id, nOut);

    // copy node properties
    for (unsigned int i = 0; i < nbProperties; ++i) {
      std::pair<PropertyInterface *, PropertyInterface *> &props = properties[i];
      props.second->copy(nOut, nIn, props.first);
    }
  }

  // get selected edges
  Iterator<edge> *edgeIt = nullptr;

  if (inSel) {
    edgeIt = inSel->getNonDefaultValuatedEdges(inG);
    outG->reserveEdges(outG->numberOfEdges() + inSel->numberOfNonDefaultValuatedEdges(inG));
  } else {
    edgeIt = inG->getEdges();
    outG->reserveEdges(outG->numberOfEdges() + inG->numberOfEdges());
  }

  // loop on edges
  for (auto eIn : edgeIt) {
    const pair<node, node> &eEnds = inG->ends(eIn);
    // add outG corresponding edge
    edge eOut = outG->addEdge(nodeTrl.get(eEnds.first.id), nodeTrl.get(eEnds.second.id));

    // select added edge
    if (outSel)
      outSel->setEdgeValue(eOut, true);

    // copy edge properties
    for (unsigned int i = 0; i < nbProperties; ++i) {
      std::pair<PropertyInterface *, PropertyInterface *> &props = properties[i];
      props.second->copy(eOut, eIn, props.first);
    }
  }
}

// define a class of Iterator to iterate the root graphs
class RootGraphsIterator : public tlp::Iterator<tlp::Graph *> {
  std::vector<Graph *> roots;
  Iterator<Graph *> *rootsIterator;

public:
  RootGraphsIterator() {
    const VectorGraph &ovg = tlp::Observable::getObservableGraph();

    // we iterate the observable graph nodes
    for (auto n : ovg.nodes()) {
      Graph *g = nullptr;

      if (tlp::Observable::isAlive(n) &&
          (g = dynamic_cast<Graph *>(tlp::Observable::getObject(n)))) {
        // check if g is a root graph
        if (g->getRoot() == g)
          roots.push_back(g);
      }
    }

    rootsIterator = stlIterator(roots);
  }

  ~RootGraphsIterator() override {
    delete rootsIterator;
  }

  Graph *next() override {
    return rootsIterator->next();
  }

  bool hasNext() override {
    return rootsIterator->hasNext();
  }
};

Iterator<Graph *> *tlp::getRootGraphs() {
  return new RootGraphsIterator();
}

bool Graph::applyAlgorithm(const std::string &algorithm, std::string &errorMessage,
                           DataSet *parameters, PluginProgress *progress) {
  if (!PluginsManager::pluginExists(algorithm)) {
    tlp::warning() << __FUNCTION__ << ": algorithm plugin \"" << algorithm
                   << "\" does not exist (or is not loaded)" << endl;
    return false;
  }

  bool result;
  bool deletePluginProgress = false;
  PluginProgress *tmpProgress;

  if (progress == nullptr) {
    tmpProgress = new SimplePluginProgress();
    deletePluginProgress = true;
  } else
    tmpProgress = progress;

  AlgorithmContext *context = new AlgorithmContext(this, parameters, tmpProgress);
  Algorithm *newAlgo = PluginsManager::getPluginObject<Algorithm>(algorithm, context);

  if ((result = newAlgo->check(errorMessage))) {
    result = newAlgo->run();

    if (!result)
      errorMessage = tmpProgress->getError();
  }

  delete newAlgo;
  delete context;

  if (deletePluginProgress)
    delete tmpProgress;

  return result;
}

//=========================================================
bool tlp::Graph::applyPropertyAlgorithm(const std::string &algorithm, PropertyInterface *prop,
                                        std::string &errorMessage, tlp::DataSet *parameters,
                                        tlp::PluginProgress *progress) {
  bool result;
  tlp::AlgorithmContext context;

  // check if this is a subgraph of prop->graph
  if (getRoot() != prop->getGraph()) {
    tlp::Graph *currentGraph = this;

    while (currentGraph->getSuperGraph() != currentGraph) {
      if (currentGraph == prop->getGraph())
        break;

      currentGraph = currentGraph->getSuperGraph();
    }

    if (currentGraph != prop->getGraph()) {
      errorMessage = "The property parameter does not belong to the graph";
#ifndef NDEBUG
      tlp::error() << __PRETTY_FUNCTION__ << ": " << errorMessage << std::endl;
#endif
      return false;
    }
  }

  auto it = circularCalls.find(algorithm);

  if (it != circularCalls.end() && (*it).second == prop) {
    errorMessage = std::string("Circular call of ") + __PRETTY_FUNCTION__;
#ifndef NDEBUG
    tlp::error() << errorMessage << std::endl;
#endif
    return false;
  }

  // nothing to do if the graph is empty
  if (isEmpty()) {
    errorMessage = "The graph is empty";
    return false;
  }

  tlp::PluginProgress *tmpProgress;

  if (progress == nullptr)
    tmpProgress = new tlp::SimplePluginProgress();
  else
    tmpProgress = progress;

  bool hasData = parameters != nullptr;

  if (!hasData)
    parameters = new tlp::DataSet();

  // add prop as result in dataset
  parameters->set("result", prop);

  context.pluginProgress = tmpProgress;
  context.graph = this;
  context.dataSet = parameters;

  tlp::Observable::holdObservers();
  circularCalls[algorithm] = prop;
  Algorithm *tmpAlgo = tlp::PluginsManager::getPluginObject<PropertyAlgorithm>(algorithm, &context);

  if (tmpAlgo != nullptr) {
    result = tmpAlgo->check(errorMessage);

    if (result) {
      result = tmpAlgo->run();

      if (!result)
        errorMessage = tmpProgress->getError();
    }

    delete tmpAlgo;
  } else {
    errorMessage = algorithm + " - No algorithm available with this name";
#ifndef NDEBUG
    tlp::error() << __PRETTY_FUNCTION__ << ": " << errorMessage << std::endl;
#endif
    result = false;
  }

  circularCalls.erase(algorithm);
  tlp::Observable::unholdObservers();

  if (progress == nullptr)
    delete tmpProgress;

  if (hasData)
    // remove result from dataset
    parameters->remove("result");
  else
    delete parameters;

  return result;
}

tlp::node Graph::getSource() const {
  for (auto source : this->nodes()) {
    if (indeg(source) == 0)
      return source;
  }

  return node();
}

tlp::node Graph::getSink() const {
  for (auto sink : this->nodes()) {
    if (outdeg(sink) == 0)
      return sink;
  }

  return node();
}

DataType *Graph::getAttribute(const std::string &name) const {
  return getAttributes().getData(name);
}

void Graph::setAttribute(const std::string &name, const DataType *value) {
  notifyBeforeSetAttribute(name);
  getNonConstAttributes().setData(name, value);
  notifyAfterSetAttribute(name);
}

void Graph::notifyAddNode(const node n) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODE, n));
}

void Graph::notifyDelNode(const node n) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_DEL_NODE, n));
}

void Graph::notifyAddEdge(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGE, e));
}

void Graph::notifyDelEdge(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_DEL_EDGE, e));
}

void Graph::notifyReverseEdge(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_REVERSE_EDGE, e));
}

void Graph::notifyBeforeSetEnds(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_SET_ENDS, e, Event::TLP_INFORMATION));
}

void Graph::notifyAfterSetEnds(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_SET_ENDS, e));
}

void Graph::notifyBeforeAddSubGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_ADD_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyBeforeAddDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyBeforeAddDescendantGraph(graph);
}

void Graph::notifyAfterAddSubGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_ADD_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyAfterAddDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyAfterAddDescendantGraph(graph);
}

void Graph::notifyBeforeDelSubGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyBeforeDelDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyBeforeDelDescendantGraph(graph);
}
void Graph::notifyAfterDelSubGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyAfterDelDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyAfterDelDescendantGraph(graph);
}

void Graph::notifyBeforeAddDescendantGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_ADD_DESCENDANTGRAPH, graph));
}
void Graph::notifyAfterAddDescendantGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_ADD_DESCENDANTGRAPH, graph));
}

void Graph::notifyBeforeDelDescendantGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_DESCENDANTGRAPH, graph));
}
void Graph::notifyAfterDelDescendantGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_DESCENDANTGRAPH, graph));
}

void Graph::notifyBeforeAddLocalProperty(const std::string &propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_ADD_LOCAL_PROPERTY, propName));
}
void Graph::notifyAddLocalProperty(const std::string &propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_LOCAL_PROPERTY, propName));
}

void Graph::notifyBeforeDelLocalProperty(const std::string &propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY, propName,
                         Event::TLP_INFORMATION));
}

void Graph::notifyAfterDelLocalProperty(const std::string &propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY, propName));
}

void Graph::notifyBeforeSetAttribute(const std::string &attName) {
  if (hasOnlookers())
    sendEvent(
        GraphEvent(*this, GraphEvent::TLP_BEFORE_SET_ATTRIBUTE, attName, Event::TLP_INFORMATION));
}

void Graph::notifyAfterSetAttribute(const std::string &attName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_SET_ATTRIBUTE, attName));
}

void Graph::notifyRemoveAttribute(const std::string &attName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_REMOVE_ATTRIBUTE, attName, Event::TLP_INFORMATION));
}

void Graph::notifyDestroy() {
  if (hasOnlookers()) {
    // the undo/redo mechanism has to simulate graph destruction
    Event evt(*this, Event::TLP_MODIFICATION);
    evt._type = Event::TLP_DELETE;
    sendEvent(evt);
  }
}

PropertyInterface *Graph::getLocalProperty(const std::string &propertyName,
                                           const std::string &propertyType) {
  if (propertyType.compare(DoubleProperty::propertyTypename) == 0) {
    return getLocalDoubleProperty(propertyName);
  } else if (propertyType.compare(LayoutProperty::propertyTypename) == 0) {
    return getLocalLayoutProperty(propertyName);
  } else if (propertyType.compare(StringProperty::propertyTypename) == 0) {
    return getLocalStringProperty(propertyName);
  } else if (propertyType.compare(IntegerProperty::propertyTypename) == 0) {
    return getLocalIntegerProperty(propertyName);
  } else if (propertyType.compare(ColorProperty::propertyTypename) == 0) {
    return getLocalColorProperty(propertyName);
  } else if (propertyType.compare(SizeProperty::propertyTypename) == 0) {
    return getLocalSizeProperty(propertyName);
  } else if (propertyType.compare(BooleanProperty::propertyTypename) == 0) {
    return getLocalBooleanProperty(propertyName);
  } else if (propertyType.compare(DoubleVectorProperty::propertyTypename) == 0) {
    return getLocalDoubleVectorProperty(propertyName);
  } else if (propertyType.compare(StringVectorProperty::propertyTypename) == 0) {
    return getLocalStringVectorProperty(propertyName);
  } else if (propertyType.compare(IntegerVectorProperty::propertyTypename) == 0) {
    return getLocalIntegerVectorProperty(propertyName);
  } else if (propertyType.compare(CoordVectorProperty::propertyTypename) == 0) {
    return getLocalCoordVectorProperty(propertyName);
  } else if (propertyType.compare(ColorVectorProperty::propertyTypename) == 0) {
    return getLocalColorVectorProperty(propertyName);
  } else if (propertyType.compare(BooleanVectorProperty::propertyTypename) == 0) {
    return getLocalBooleanVectorProperty(propertyName);
  } else if (propertyType.compare(SizeVectorProperty::propertyTypename) == 0) {
    return getLocalSizeVectorProperty(propertyName);
  } else if (propertyType.compare(GraphProperty::propertyTypename) == 0) {
    return getLocalGraphProperty(propertyName);
  } else {
    return nullptr;
  }
}

PropertyInterface *Graph::getProperty(const std::string &propertyName,
                                      const std::string &propertyType) {
  if (propertyType.compare(DoubleProperty::propertyTypename) == 0) {
    return getDoubleProperty(propertyName);
  } else if (propertyType.compare(LayoutProperty::propertyTypename) == 0) {
    return getLayoutProperty(propertyName);
  } else if (propertyType.compare(StringProperty::propertyTypename) == 0) {
    return getStringProperty(propertyName);
  } else if (propertyType.compare(IntegerProperty::propertyTypename) == 0) {
    return getIntegerProperty(propertyName);
  } else if (propertyType.compare(ColorProperty::propertyTypename) == 0) {
    return getColorProperty(propertyName);
  } else if (propertyType.compare(SizeProperty::propertyTypename) == 0) {
    return getSizeProperty(propertyName);
  } else if (propertyType.compare(BooleanProperty::propertyTypename) == 0) {
    return getBooleanProperty(propertyName);
  } else if (propertyType.compare(DoubleVectorProperty::propertyTypename) == 0) {
    return getDoubleVectorProperty(propertyName);
  } else if (propertyType.compare(StringVectorProperty::propertyTypename) == 0) {
    return getStringVectorProperty(propertyName);
  } else if (propertyType.compare(IntegerVectorProperty::propertyTypename) == 0) {
    return getIntegerVectorProperty(propertyName);
  } else if (propertyType.compare(CoordVectorProperty::propertyTypename) == 0) {
    return getCoordVectorProperty(propertyName);
  } else if (propertyType.compare(ColorVectorProperty::propertyTypename) == 0) {
    return getColorVectorProperty(propertyName);
  } else if (propertyType.compare(BooleanVectorProperty::propertyTypename) == 0) {
    return getBooleanVectorProperty(propertyName);
  } else if (propertyType.compare(SizeVectorProperty::propertyTypename) == 0) {
    return getSizeVectorProperty(propertyName);
  } else if (propertyType.compare(GraphProperty::propertyTypename) == 0) {
    return getGraphProperty(propertyName);
  } else {
    return nullptr;
  }
}

static const string layoutProperty = "viewLayout";
static const string sizeProperty = "viewSize";
static const string rotationProperty = "viewRotation";
static const string colorProperty = "viewColor";

static void buildMapping(Iterator<node> *it, MutableContainer<node> &mapping,
                         GraphProperty *metaInfo, const node from = node()) {
  for (auto n : it) {
    if (!from.isValid())
      mapping.set(n.id, n);
    else
      mapping.set(n.id, from);

    Graph *meta = metaInfo->getNodeValue(n);

    if (meta != nullptr)
      buildMapping(meta->getNodes(), mapping, metaInfo, mapping.get(n.id));
  }
}
//====================================================================================
void updatePropertiesUngroup(Graph *graph, node metanode, GraphProperty *clusterInfo) {
  if (clusterInfo->getNodeValue(metanode) == nullptr)
    return; // The metanode is not a metanode.

  LayoutProperty *graphLayout = graph->getLayoutProperty(layoutProperty);
  SizeProperty *graphSize = graph->getSizeProperty(sizeProperty);
  DoubleProperty *graphRot = graph->getDoubleProperty(rotationProperty);
  const Size &size = graphSize->getNodeValue(metanode);
  const Coord &pos = graphLayout->getNodeValue(metanode);
  const double &rot = graphRot->getNodeValue(metanode);
  Graph *cluster = clusterInfo->getNodeValue(metanode);
  LayoutProperty *clusterLayout = cluster->getLayoutProperty(layoutProperty);
  SizeProperty *clusterSize = cluster->getSizeProperty(sizeProperty);
  DoubleProperty *clusterRot = cluster->getDoubleProperty(rotationProperty);
  BoundingBox box = tlp::computeBoundingBox(cluster, clusterLayout, clusterSize, clusterRot);
  double width = box.width();
  double height = box.height();
  double depth = box.depth();

  if (width < 0.0001)
    width = 1.0;

  if (height < 0.0001)
    height = 1.0;

  if (depth < 0.0001)
    depth = 1.0;

  // keep aspect ratio of content layout when opening a meta-node
  double divW = size[0] / width;
  double divH = size[1] / height;
  double scale = 1.0;

  if (divH * width > size[0]) {
    scale = divW;
  } else {
    scale = divH;
  }

  clusterLayout->translate(-box.center(), cluster);
  clusterLayout->rotateZ(graphRot->getNodeValue(metanode), cluster);
  clusterLayout->scale(Coord(scale, scale, size[2] / depth), cluster);
  clusterLayout->translate(pos, cluster);
  clusterSize->scale(Size(scale, scale, size[2] / depth), cluster);

  for (auto n : cluster->nodes()) {
    graphLayout->setNodeValue(n, clusterLayout->getNodeValue(n));
    graphSize->setNodeValue(n, clusterSize->getNodeValue(n));
    graphRot->setNodeValue(n, clusterRot->getNodeValue(n) + rot);
  }

  for (auto e : cluster->edges()) {
    graphLayout->setEdgeValue(e, clusterLayout->getEdgeValue(e));
    graphSize->setEdgeValue(e, clusterSize->getEdgeValue(e));
  }

  // propagate all cluster local properties
  for (PropertyInterface *property : cluster->getLocalObjectProperties()) {
    if (property == graphLayout || property == graphSize || property == graphRot)
      continue;

    PropertyInterface *graphProp = nullptr;

    if (graph->existProperty(property->getName()) &&
        graph->getProperty(property->getName())->getTypename() == property->getTypename())
      graphProp = graph->getProperty(property->getName());
    else
      graphProp = property->clonePrototype(graph, property->getName());

    for (auto n : cluster->nodes()) {
      graphProp->setNodeStringValue(n, property->getNodeStringValue(n));
    }

    for (auto e : cluster->edges()) {
      graphProp->setEdgeStringValue(e, property->getEdgeStringValue(e));
    }
  }
}
//=========================================================
Graph *Graph::addSubGraph(const std::string &name) {
  Graph *g = addSubGraph(nullptr, name);
  return g;
}
//=========================================================
Graph *Graph::addCloneSubGraph(const std::string &name, bool addSibling,
                               bool addSiblingProperties) {
  BooleanProperty selection(this);
  selection.setAllNodeValue(true);
  selection.setAllEdgeValue(true);
  Graph *parentSubGraph = this;

  if (addSibling) {
    parentSubGraph = getSuperGraph();

    if (this == parentSubGraph)
      // cannot add sibling of root graph
      return nullptr;
  }

  Graph *clone = parentSubGraph->addSubGraph(&selection, name);

  if (addSibling && addSiblingProperties) {
    for (PropertyInterface *prop : getLocalObjectProperties()) {
      PropertyInterface *cloneProp = prop->clonePrototype(clone, prop->getName());
      tlp::debug() << "clone property " << prop->getName().c_str() << std::endl;
      cloneProp->copy(prop);
    }
  }

  return clone;
}
//=========================================================
Graph *Graph::inducedSubGraph(const std::vector<node> &nodes, Graph *parentSubGraph,
                              const string &name) {
  if (parentSubGraph == nullptr)
    parentSubGraph = this;

  // create subgraph and add nodes
  Graph *result = parentSubGraph->addSubGraph(name);
  result->addNodes(nodes);

  for (auto n : nodes) {
    for (auto e : getOutEdges(n)) {
      if (result->isElement(target(e)))
        result->addEdge(e);
    }
  }

  return result;
}
//=========================================================
Graph *Graph::inducedSubGraph(BooleanProperty *selection, Graph *parentSubGraph,
                              const string &name) {
  vector<node> nodes;
  for (auto n : selection->getNodesEqualTo(true, parentSubGraph)) {
    nodes.push_back(n);
  }
  for (auto e : selection->getEdgesEqualTo(true, parentSubGraph)) {
    const pair<node, node> ext = ends(e);
    nodes.push_back(ext.first);
    nodes.push_back(ext.second);
  }
  return inducedSubGraph(nodes, parentSubGraph, name);
}
//====================================================================================
node Graph::createMetaNode(const std::vector<node> &nodes, bool multiEdges, bool delAllEdge) {
  if (getRoot() == this) {
    tlp::warning() << __PRETTY_FUNCTION__ << std::endl;
    tlp::warning() << "\t Error: Could not group a set of nodes in the root graph" << std::endl;
    return node();
  }

  if (nodes.empty()) {
    tlp::warning() << __PRETTY_FUNCTION__ << std::endl;
    tlp::warning() << '\t' << "Warning: Creation of an empty metagraph" << std::endl;
  }

  // create an induced brother sub graph
  Graph *subGraph = inducedSubGraph(nodes, getSuperGraph());
  // all local properties
  // must be cloned in subgraph
  for (PropertyInterface *prop : getLocalObjectProperties()) {
    PropertyInterface *sgProp = prop->clonePrototype(subGraph, prop->getName());
    for (auto n : nodes) {
      DataMem *val = prop->getNodeDataMemValue(n);
      sgProp->setNodeDataMemValue(n, val);
      delete val;
    }
  }

  stringstream st;
  st << "grp_" << setfill('0') << setw(5) << subGraph->getId();
  subGraph->setAttribute("name", st.str());
  return createMetaNode(subGraph, multiEdges, delAllEdge);
}
//====================================================================================
#define NEED_TODEL 2
#define NO_NEED_TODEL 0
#define CHECK_TODEL 1
node Graph::createMetaNode(Graph *subGraph, bool multiEdges, bool edgeDelAll) {
  if (getRoot() == this) {
    tlp::warning() << __PRETTY_FUNCTION__ << std::endl;
    tlp::warning() << "\t Error: Could not create a meta node in the root graph" << std::endl;
    return node();
  }

  GraphProperty *metaInfo = static_cast<GraphAbstract *>(getRoot())->getMetaGraphProperty();
  node metaNode = addNode();
  metaInfo->setNodeValue(metaNode, subGraph);
  Observable::holdObservers();

  // updateGroupLayout(this, subGraph, metaNode);
  // compute meta node values
  for (PropertyInterface *property : getObjectProperties()) {
    property->computeMetaValue(metaNode, subGraph, this);
  }

  // keep track of graph existing edges
  MutableContainer<bool> graphEdges;
  graphEdges.setAll(false);

  for (auto e : edges())
    graphEdges.set(e.id, true);

  // we can now remove nodes from graph
  delNodes(subGraph->nodes());

  // create new meta edges from nodes to metanode
  Graph *super = getSuperGraph();
  std::unordered_map<node, std::unordered_set<node>> edges;
  std::unordered_map<node, edge> metaEdges;
  std::unordered_map<edge, set<edge>> subEdges;

  for (auto n : subGraph->nodes()) {
    for (auto e : getSuperGraph()->getInOutEdges(n)) {
      auto eEnds = ends(e);
      node src = eEnds.first;
      node tgt = eEnds.second;
      unsigned int toDelete = isElement(src);

      if (toDelete && subGraph->isElement(tgt)) {
        if (multiEdges || edges[src].empty()) {
          // add new meta edge
          edge metaEdge = addEdge(src, metaNode);

          if (!graphEdges.get(e.id))
            delEdge(metaEdge);

          // e is a sub-edge of metaEdge
          subEdges[metaEdge].insert(e);

          if (!multiEdges)
            // record metaEdge
            metaEdges[src] = metaEdge;

          if (!super->isElement(metaEdge))
            super->addEdge(metaEdge);
        } else if (!multiEdges)
          // e is a sub-edge of an already created meta edge
          subEdges[metaEdges[src]].insert(e);

        edges[src].insert(tgt);

        if (((metaInfo->getNodeValue(src) != nullptr) ||
             (metaInfo->getNodeValue(tgt) != nullptr)) &&
            existEdge(src, tgt).isValid()) {
          toDelete = NEED_TODEL;
          delEdge(e, edgeDelAll);
        }
      }

      if (isElement(tgt) && subGraph->isElement(src)) {
        if (multiEdges || edges[tgt].empty()) {
          // add new meta edge
          edge metaEdge = addEdge(metaNode, tgt);

          if (!graphEdges.get(e.id))
            delEdge(metaEdge);

          // e is a sub-edge of metaEdge
          subEdges[metaEdge].insert(e);

          if (!multiEdges)
            // record metaEdge
            metaEdges[tgt] = metaEdge;

          if (!super->isElement(metaEdge))
            super->addEdge(metaEdge);
        } else if (!multiEdges)
          // e is a sub-edge of an already created meta edge
          subEdges[metaEdges[tgt]].insert(e);

        edges[tgt].insert(src);

        if (toDelete == CHECK_TODEL)
          toDelete = ((metaInfo->getNodeValue(src) != nullptr) ||
                      (metaInfo->getNodeValue(tgt) != nullptr)) &&
                     existEdge(src, tgt).isValid();

        if (toDelete) {
          delEdge(e, edgeDelAll);
        }
      }
    }
  }

  // update metaInfo of new meta edges
  for (const auto &it : subEdges) {
    edge mE = it.first;
    metaInfo->setEdgeValue(mE, it.second);
    // compute meta edge values
    for (PropertyInterface *property : getObjectProperties()) {
      property->computeMetaValue(mE, getEdgeMetaInfo(mE), this);
    }
  }

  Observable::unholdObservers();
  return metaNode;
}

// map all nodes or embedded nodes (nodes of meta node subgraph)
// of a subgraph to metaNode
static void mapSubGraphNodes(Graph *sg, node metaNode, MutableContainer<node> &mappingM,
                             GraphProperty *metaInfo) {
  for (auto n : sg->nodes()) {
    mappingM.set(n.id, metaNode);
    Graph *ssg = metaInfo->getNodeValue(n);

    if (ssg)
      mapSubGraphNodes(ssg, metaNode, mappingM, metaInfo);
  }
}
//====================================================================================
void Graph::openMetaNode(node metaNode, bool updateProperties) {
  if (getRoot() == this) {
    tlp::warning() << __PRETTY_FUNCTION__ << std::endl;
    tlp::warning() << "\t Error: Could not ungroup a meta node in the root graph" << std::endl;
    return;
  }

  GraphProperty *metaInfo = static_cast<GraphAbstract *>(getRoot())->getMetaGraphProperty();
  Graph *metaGraph = metaInfo->getNodeValue(metaNode);

  if (metaGraph == nullptr)
    return;

  Observable::holdObservers();
  MutableContainer<node> mappingM;
  mappingM.setAll(node());
  // add node from meta to graph
  {
    auto nodes = metaGraph->nodes();
    unsigned int nbNodes = nodes.size();
    // stable in case of fractal graph
    for (unsigned int i = 0; i < nbNodes; ++i) {
      node n = nodes[i];
      addNode(n);
      mappingM.set(n.id, n);
      Graph *sg = metaInfo->getNodeValue(n);

      if (sg)
        // map all nodes or embedded nodes
        // of this subgraph to n
        mapSubGraphNodes(sg, n, mappingM, metaInfo);
    }
    addEdges(metaGraph->edges());
  }

  if (updateProperties)
    updatePropertiesUngroup(this, metaNode, metaInfo);

  // check for edges from or to the meta node
  Graph *super = getSuperGraph();
  Iterator<edge> *metaEdges = super->getInOutEdges(metaNode);

  if (!metaEdges->hasNext()) {
    delete metaEdges;
    // no edge so just remove the meta node
    getRoot()->delNode(metaNode, true);
    Observable::unholdObservers();
    return;
  }

  bool hasSubEdges = super->isMetaEdge(metaEdges->next());
  delete metaEdges;

  ColorProperty *graphColors = getColorProperty(colorProperty);

  if (hasSubEdges) {
    // compute mapping for neighbour nodes
    // and their sub nodes
    for (auto mn : super->getInOutNodes(metaNode)) {
      mappingM.set(mn.id, mn);
      Graph *mnGraph = metaInfo->getNodeValue(mn);

      if (mnGraph != nullptr) {
        for (auto mnn : mnGraph->nodes()) {
          mappingM.set(mnn.id, mn);
        }
      }
    }

    for (auto metaEdge : super->allEdges(metaNode)) {
      if (!super->isElement(metaEdge))
        continue;
      Color metaColor = graphColors->getEdgeValue(metaEdge);
      std::unordered_map<node, std::unordered_map<node, set<edge>>> newMetaEdges;

      for (auto e : getEdgeMetaInfo(metaEdge)) {
        auto eEnds = super->ends(e);

        if (isElement(eEnds.first)) {
          if (isElement(eEnds.second) && isElement(metaEdge)) {
            addEdge(e);
            graphColors->setEdgeValue(e, metaColor);
          } else if (eEnds.first != metaNode) {
            node tgt = mappingM.get(eEnds.second.id);

            // tgt may not be valid because at this time
            // when deleting a node from a subgraph pointed
            // by a metanode we are not able to correctly
            // update the meta edges embedding the inout edges
            // of this node
            if (tgt.isValid())
              newMetaEdges[eEnds.first][tgt].insert(e);
          }
        } else if (eEnds.second != metaNode) {
          node src = mappingM.get(eEnds.first.id);

          // src may not be valid because at this time
          // when deleting a node from a subgraph pointed
          // by a metanode we are not able to correctly
          // update the meta edges embedding the inout edges
          // of this node
          if (src.isValid())
            newMetaEdges[src][eEnds.second].insert(e);
        }
      }

      // iterate on newMetaEdges
      for (const auto &itme : newMetaEdges) {
        node src = itme.first;

        for (const auto &itnme : itme.second) {
          Graph *graph = this;
          node tgt(itnme.first);

          // add an edge in the relevant graph
          if (!isElement(src) || !isElement(tgt))
            graph = super;

          edge mE = graph->addEdge(src, tgt);
          metaInfo->setEdgeValue(mE, itnme.second);
          // compute meta edge values
          for (PropertyInterface *property : graph->getObjectProperties()) {
            property->computeMetaValue(mE, getEdgeMetaInfo(mE), graph);
          }
        }
      }
    }

    getRoot()->delNode(metaNode, true);
  } else {
    MutableContainer<node> mappingC;
    MutableContainer<node> mappingN;
    mappingC.setAll(node());
    mappingN.setAll(node());
    Graph *root = getRoot();
    buildMapping(root->getInOutNodes(metaNode), mappingC, metaInfo, node());
    buildMapping(metaGraph->getNodes(), mappingN, metaInfo, node());

    std::unordered_map<node, Color> metaEdgeToColor;

    for (auto metaEdge : super->getInOutEdges(metaNode)) {
      metaEdgeToColor[opposite(metaEdge, metaNode)] = graphColors->getEdgeValue(metaEdge);
    }

    // Remove the metagraph from the hierarchy and remove the metanode
    root->delNode(metaNode, true);
    std::unordered_map<node, std::unordered_set<node>> edges;
    //=================================
    for (auto e : root->edges()) {

      if (isElement(e))
        continue;

      auto eEnds = root->ends(e);
      unsigned int srcId = eEnds.first.id;
      unsigned int tgtId = eEnds.second.id;
      node sourceC = mappingC.get(srcId);
      node targetN = mappingN.get(tgtId);
      node sourceN = mappingN.get(srcId);
      node targetC = mappingC.get(tgtId);
      node src, tgt;
      Color edgeColor;

      if (sourceC.isValid() && targetN.isValid()) {
        src = sourceC;
        tgt = targetN;
        edgeColor = metaEdgeToColor[src];
      } else {
        if (sourceN.isValid() && targetC.isValid()) {
          src = sourceN;
          tgt = targetC;
          edgeColor = metaEdgeToColor[tgt];
        } else
          continue;
      }

      if (metaInfo->getNodeValue(src) == nullptr && metaInfo->getNodeValue(tgt) == nullptr) {
        addEdge(e);
        continue;
      }

      if ((edges.find(src) == edges.end()) || (edges[src].find(tgt) == edges[src].end())) {
        edges[src].insert(tgt);

        if (!existEdge(src, tgt).isValid()) {
          edge addedEdge = addEdge(src, tgt);
          graphColors->setEdgeValue(addedEdge, edgeColor);
        } else
          tlp::error() << __PRETTY_FUNCTION__ << ": bug exist edge 1" << std::endl;
      }
    }
  }

  Observable::unholdObservers();
}
//====================================================================================
struct MetaEdge {
  unsigned int source, target;
  edge mE;
};

namespace std {
template <>
struct less<MetaEdge> {
  bool operator()(const MetaEdge &c, const MetaEdge &d) const {
    /*if (c.source<d.source) return true;
    if (c.source>d.source) return false;
    if (c.target<d.target) return true;
    if (c.target>d.target) return false;
    return false;*/
    return (c.source < d.source) || ((c.source == d.source) && (c.target < d.target));
  }
};
} // namespace std

void Graph::createMetaNodes(Iterator<Graph *> *itS, Graph *quotientGraph, vector<node> &metaNodes) {
  GraphProperty *metaInfo = static_cast<GraphAbstract *>(getRoot())->getMetaGraphProperty();
  unordered_map<edge, set<edge>> eMapping;
  Observable::holdObservers();
  {
    unordered_map<node, set<node>> nMapping;

    while (itS->hasNext()) {
      Graph *its = itS->next();

      if (its != quotientGraph) {
        // Create one metanode for each subgraph(cluster)
        node metaN = quotientGraph->addNode();
        metaNodes.push_back(metaN);
        metaInfo->setNodeValue(metaN, its);
        // compute meta node values
        for (PropertyInterface *property : quotientGraph->getObjectProperties()) {
          property->computeMetaValue(metaN, its, quotientGraph);
        }
        for (auto n : its->nodes()) {
          // map each subgraph's node to a set of meta nodes
          // in order to deal consistently with overlapping clusters
          nMapping[n].insert(metaN);
        }
      }
    }

    {
      set<MetaEdge> myQuotientGraph;

      // for each existing edge in the current graph
      // add a meta edge for the corresponding couple
      // (meta source, meta target) if it does not already exists
      // and register the edge as associated to this meta edge
      const vector<edge> &edges = this->edges();
      unsigned int nbEdges = edges.size();
      for (unsigned int i = 0; i < nbEdges; ++i) {
        edge e = edges[i];
        auto eEnds = ends(e);
        set<node> &metaSources = nMapping[eEnds.first];
        set<node> &metaTargets = nMapping[eEnds.second];

        for (auto mSource : metaSources) {
          for (auto mTarget : metaTargets) {
            if (mSource != mTarget) {
              MetaEdge tmp;
              tmp.source = mSource.id, tmp.target = mTarget.id;
              auto itm = myQuotientGraph.find(tmp);

              if (itm == myQuotientGraph.end()) {
                edge mE = quotientGraph->addEdge(mSource, mTarget);
                tmp.mE = mE;
                myQuotientGraph.insert(tmp);
                eMapping[mE].insert(e);
              } else {
                // add edge
                eMapping[itm->mE].insert(e);
              }
            }
          }
        }
      }
    }
  }
  // set viewMetaGraph for added meta edges
  for (const auto &itm : eMapping) {
    edge mE = itm.first;
    metaInfo->setEdgeValue(mE, itm.second);
    // compute meta edge values
    for (auto prop : quotientGraph->getObjectProperties()) {
      prop->computeMetaValue(mE, getRoot()->getEdgeMetaInfo(mE), quotientGraph);
    }
  }

  Observable::unholdObservers();
}

Graph *Graph::getNthSubGraph(unsigned int n) const {
  unsigned int i = 0;
  for (tlp::Graph *sg : subGraphs()) {
    if (i++ == n) {
      return sg;
    }
  }
  return nullptr;
}

const std::string &GraphEvent::getPropertyName() const {
  assert((evtType > TLP_AFTER_DEL_SUBGRAPH && evtType < TLP_BEFORE_SET_ATTRIBUTE) ||
         evtType > TLP_REMOVE_ATTRIBUTE);

  if (evtType == TLP_BEFORE_RENAME_LOCAL_PROPERTY || evtType == TLP_AFTER_RENAME_LOCAL_PROPERTY)
    return info.renamedProp->first->getName();

  return *(info.name);
}

void Graph::addNodes(const std::vector<node> &nodes) {
  addNodes(stlIterator(nodes));
}

void Graph::delNodes(const std::vector<node> &nodes, bool deleteInAllGraphs) {
  delNodes(stlIterator(nodes), deleteInAllGraphs);
}

void Graph::addEdges(const std::vector<edge> &edges) {
  addEdges(stlIterator(edges));
}

void Graph::delEdges(const std::vector<edge> &edges, bool deleteInAllGraphs) {
  delEdges(stlIterator(edges), deleteInAllGraphs);
}

BooleanProperty *Graph::getLocalBooleanProperty(const string &propertyName) {
  return getLocalProperty<BooleanProperty>(propertyName);
}

BooleanProperty *Graph::getBooleanProperty(const string &propertyName) {
  return getProperty<BooleanProperty>(propertyName);
}

ColorProperty *Graph::getLocalColorProperty(const string &propertyName) {
  return getLocalProperty<ColorProperty>(propertyName);
}

ColorProperty *Graph::getColorProperty(const string &propertyName) {
  return getProperty<ColorProperty>(propertyName);
}

DoubleProperty *Graph::getLocalDoubleProperty(const string &propertyName) {
  return getLocalProperty<DoubleProperty>(propertyName);
}

DoubleProperty *Graph::getDoubleProperty(const string &propertyName) {
  return getProperty<DoubleProperty>(propertyName);
}

GraphProperty *Graph::getLocalGraphProperty(const string &propertyName) {
  return getLocalProperty<GraphProperty>(propertyName);
}

GraphProperty *Graph::getGraphProperty(const string &propertyName) {
  return getProperty<GraphProperty>(propertyName);
}

IntegerProperty *Graph::getLocalIntegerProperty(const string &propertyName) {
  return getLocalProperty<IntegerProperty>(propertyName);
}

IntegerProperty *Graph::getIntegerProperty(const string &propertyName) {
  return getProperty<IntegerProperty>(propertyName);
}

LayoutProperty *Graph::getLocalLayoutProperty(const string &propertyName) {
  return getLocalProperty<LayoutProperty>(propertyName);
}

LayoutProperty *Graph::getLayoutProperty(const string &propertyName) {
  return getProperty<LayoutProperty>(propertyName);
}

SizeProperty *Graph::getLocalSizeProperty(const string &propertyName) {
  return getLocalProperty<SizeProperty>(propertyName);
}

SizeProperty *Graph::getSizeProperty(const string &propertyName) {
  return getProperty<SizeProperty>(propertyName);
}

StringProperty *Graph::getLocalStringProperty(const string &propertyName) {
  return getLocalProperty<StringProperty>(propertyName);
}

StringProperty *Graph::getStringProperty(const string &propertyName) {
  return getProperty<StringProperty>(propertyName);
}

BooleanVectorProperty *Graph::getLocalBooleanVectorProperty(const string &propertyName) {
  return getLocalProperty<BooleanVectorProperty>(propertyName);
}

BooleanVectorProperty *Graph::getBooleanVectorProperty(const string &propertyName) {
  return getProperty<BooleanVectorProperty>(propertyName);
}

ColorVectorProperty *Graph::getLocalColorVectorProperty(const string &propertyName) {
  return getLocalProperty<ColorVectorProperty>(propertyName);
}

ColorVectorProperty *Graph::getColorVectorProperty(const string &propertyName) {
  return getProperty<ColorVectorProperty>(propertyName);
}

DoubleVectorProperty *Graph::getLocalDoubleVectorProperty(const string &propertyName) {
  return getLocalProperty<DoubleVectorProperty>(propertyName);
}

DoubleVectorProperty *Graph::getDoubleVectorProperty(const string &propertyName) {
  return getProperty<DoubleVectorProperty>(propertyName);
}

IntegerVectorProperty *Graph::getLocalIntegerVectorProperty(const string &propertyName) {
  return getLocalProperty<IntegerVectorProperty>(propertyName);
}

IntegerVectorProperty *Graph::getIntegerVectorProperty(const string &propertyName) {
  return getProperty<IntegerVectorProperty>(propertyName);
}

CoordVectorProperty *Graph::getLocalCoordVectorProperty(const string &propertyName) {
  return getLocalProperty<CoordVectorProperty>(propertyName);
}

CoordVectorProperty *Graph::getCoordVectorProperty(const string &propertyName) {
  return getProperty<CoordVectorProperty>(propertyName);
}

SizeVectorProperty *Graph::getLocalSizeVectorProperty(const string &propertyName) {
  return getLocalProperty<SizeVectorProperty>(propertyName);
}

SizeVectorProperty *Graph::getSizeVectorProperty(const string &propertyName) {
  return getProperty<SizeVectorProperty>(propertyName);
}

StringVectorProperty *Graph::getLocalStringVectorProperty(const string &propertyName) {
  return getLocalProperty<StringVectorProperty>(propertyName);
}

StringVectorProperty *Graph::getStringVectorProperty(const string &propertyName) {
  return getProperty<StringVectorProperty>(propertyName);
}

struct DescendantGraphsIterator : public Iterator<Graph *> {
  // use a stack to store non empty iterators
  stack<Iterator<Graph *> *> iterators;
  // the current one
  Iterator<Graph *> *current;

  DescendantGraphsIterator(const Graph *g) {
    current = g->getSubGraphs();

    if (!current->hasNext()) {
      delete current;
      current = nullptr;
    }
  }

  ~DescendantGraphsIterator() override {
    if (current)
      delete current;

    while (!iterators.empty()) {
      delete iterators.top();
      iterators.pop();
    }
  }

  bool hasNext() override {
    return current != nullptr;
  }

  Graph *next() override {
    if (current) {
      Graph *g = current->next();
      Iterator<Graph *> *itg = g->getSubGraphs();

      if (itg->hasNext()) {
        if (current->hasNext())
          // pushed iterators are always non empty
          iterators.push(current);
        else
          delete current;

        current = itg;
      } else {
        delete itg;

        if (!current->hasNext()) {
          delete current;

          if (!iterators.empty()) {
            current = iterators.top();
            iterators.pop();
          } else
            current = nullptr;
        }
      }

      return g;
    }

    return nullptr;
  }
};

Iterator<Graph *> *Graph::getDescendantGraphs() const {
  return new DescendantGraphsIterator(this);
}

// destructor
GraphEvent::~GraphEvent() {
  if (evtType > TLP_AFTER_DEL_SUBGRAPH) {
    // need to cleanup name if any
    if (evtType == TLP_BEFORE_RENAME_LOCAL_PROPERTY || evtType == TLP_AFTER_RENAME_LOCAL_PROPERTY)
      delete info.renamedProp;
    else
      delete info.name;
  } else {
    //  need to cleanup vectInfos if not null
    if (evtType == TLP_ADD_NODES && vectInfos.addedNodes)
      delete vectInfos.addedNodes;
    else if (evtType == TLP_ADD_EDGES && vectInfos.addedEdges)
      delete vectInfos.addedEdges;
  }
}

const std::vector<node> &GraphEvent::getNodes() const {
  assert(evtType == TLP_ADD_NODES);

  if (vectInfos.addedNodes == nullptr) {
    unsigned int nbElts = info.nbElts;
    std::vector<node> *addedNodes = new std::vector<node>();
    addedNodes->reserve(nbElts);
    const std::vector<node> &nodes = getGraph()->nodes();
    // copy new graph nodes in addedNodes reserved memory
    memcpy(addedNodes->data(), &nodes[nodes.size() - nbElts], nbElts * sizeof(node));
    // set addedNodes size using underlying vector pointers
    // to avoid unnecessary multiple constructeur calls
    reinterpret_cast<node **>(addedNodes)[1] = reinterpret_cast<node **>(addedNodes)[0] + nbElts;
    // record allocated vector in vectInfos
    const_cast<GraphEvent *>(this)->vectInfos.addedNodes = addedNodes;
  }

  return *vectInfos.addedNodes;
}

const std::vector<edge> &GraphEvent::getEdges() const {
  assert(evtType == TLP_ADD_EDGES);

  if (vectInfos.addedEdges == nullptr) {
    unsigned int nbElts = info.nbElts;
    std::vector<edge> *addedEdges = new std::vector<edge>();
    addedEdges->reserve(nbElts);
    const std::vector<edge> &edges = getGraph()->edges();
    // copy new graph edges in addedEdges reserved memory
    memcpy(addedEdges->data(), &edges[edges.size() - nbElts], nbElts * sizeof(edge));
    // set addedEdges size using underlying vector pointers
    // to avoid unnecessary multiple constructeur calls
    reinterpret_cast<edge **>(addedEdges)[1] = reinterpret_cast<edge **>(addedEdges)[0] + nbElts;
    // record allocated vector in vectInfos
    const_cast<GraphEvent *>(this)->vectInfos.addedEdges = addedEdges;
  }

  return *vectInfos.addedEdges;
}
