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

#include <QMouseEvent>
#include <QMessageBox>
#include <QDrag>
#include <QtCore/QTime>

#include "TalipotMainWindow.h"
#include "AlgorithmRunnerItem.h"
#include "ui_AlgorithmRunnerItem.h"

#include <talipot/GraphTest.h>
#include <talipot/Mimes.h>
#include <talipot/ItemDelegate.h>
#include <talipot/ParameterListModel.h>
#include <talipot/Settings.h>
#include <talipot/TlpQtTools.h>
#include <talipot/BooleanProperty.h>
#include <talipot/StringProperty.h>
#include <talipot/LayoutProperty.h>
#include <talipot/GraphProperty.h>
#include <talipot/SizeProperty.h>
#include <talipot/IntegerProperty.h>
#include <talipot/ColorProperty.h>
#include <talipot/MetaTypes.h>
#include <talipot/ColorScalesManager.h>
#include <talipot/StableIterator.h>

using namespace tlp;

AlgorithmRunnerItem::AlgorithmRunnerItem(QString pluginName, QWidget *parent)
    : QWidget(parent), _ui(new Ui::AlgorithmRunnerItem), _pluginName(pluginName), _graph(nullptr),
      _storeResultAsLocal(true) {
  _ui->setupUi(this);
  connect(_ui->favoriteCheck, SIGNAL(toggled(bool)), this, SIGNAL(favorized(bool)));
  const Plugin &plugin = PluginsManager::pluginInformation(QStringToTlpString(pluginName));
  // split pluginName after the second word if needed
  QStringList words = pluginName.split(' ');

  if (words.size() > 3) {
    QString name = pluginName;
    name.replace(words[1] + ' ', words[1] + '\n');
    _ui->playButton->setText(name);
  } else
    _ui->playButton->setText(pluginName);

  _ui->playButton->setStyleSheet("text-align: left");
  QString tooltip(QString("Apply '") + pluginName + "'");
  // initialize parameters only if needed
  _ui->parameters->setVisible(false);

  if (plugin.inputRequired()) {
    tooltip += " with current settings";
  }

  if (!plugin.getParameters().empty()) {
    _ui->parameters->setItemDelegate(new ItemDelegate(_ui->parameters));
  } else {
    _ui->settingsButton->setVisible(false);
  }

  std::string info = plugin.info();

  // show info in tooltip only if it contains more than one word
  if (info.find(' ') != std::string::npos)
    _ui->playButton->setToolTip(
        QString("<table><tr><td>%1:</td></tr><tr><td><i>%2</i></td></tr></table>")
            .arg(tooltip)
            .arg(tlp::tlpStringToQString(info)));
  else
    _ui->playButton->setToolTip(tooltip);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

  static QPixmap cppPix(":/talipot/app/icons/16/cpp.png");
  static QPixmap pythonPix(":/talipot/app/icons/16/python.png");

  if (plugin.programmingLanguage() == "Python") {
    _ui->languageLabel->setPixmap(pythonPix);
    _ui->languageLabel->setToolTip("Plugin written in Python");
  } else {
    _ui->languageLabel->setPixmap(cppPix);
    _ui->languageLabel->setToolTip("Plugin written in C++");
  }

  connect(_ui->favoriteCheck, SIGNAL(stateChanged(int)), this, SLOT(favoriteChanged(int)));
}

AlgorithmRunnerItem::~AlgorithmRunnerItem() {
  delete _ui->parameters->itemDelegate();
  delete _ui;
}

void AlgorithmRunnerItem::setGraph(Graph *g) {
  _graph = g;

  if (_ui->parameters->model() != nullptr) {
    ParameterListModel *model = static_cast<ParameterListModel *>(_ui->parameters->model());
    DataSet dataSet = model->parametersValues();
    for (const std::pair<std::string, tlp::DataType *> &it : stableIterator(dataSet.getValues())) {
      if (it.second->isTalipotProperty()) {
        dataSet.remove(it.first);
      }
    }
    _initData = dataSet;

    _ui->parameters->setModel(nullptr);
  }

  if (_ui->parameters->isVisible())
    initModel();
}

void AlgorithmRunnerItem::setData(const DataSet &data) {
  initModel();
  ParameterListModel *model = static_cast<ParameterListModel *>(_ui->parameters->model());
  model->setParametersValues(data);
}

QString AlgorithmRunnerItem::name() const {
  return _pluginName;
}

tlp::Graph *AlgorithmRunnerItem::graph() const {
  return _graph;
}

template <typename PROP>
void asLocal(QVariant var, DataSet &data, Graph *g) {
  if (var.userType() == qMetaTypeId<PROP *>()) {
    PROP *prop = var.value<PROP *>();
    if (!prop) {
      data.remove("result");
      return;
    }
    const std::string &propName = prop->getName();
    bool hasProp = g->existLocalProperty(propName);
    PROP *local = g->getLocalProperty<PROP>(propName);

    if (!hasProp) {
      // copy default property values to ensure
      // the inheritance of user defined property settings
      local->setAllNodeValue(prop->getNodeDefaultValue());
      local->setAllEdgeValue(prop->getEdgeDefaultValue());
    }

    data.set("result", local);
  }
}

static void copyToLocal(DataSet &data, Graph *g) {
  if (!data.exists("result"))
    return;

  DataType *d = data.getData("result");
  QVariant var = MetaTypes::dataTypeToQvariant(d, "");
  asLocal<DoubleProperty>(var, data, g);
  asLocal<IntegerProperty>(var, data, g);
  asLocal<LayoutProperty>(var, data, g);
  asLocal<SizeProperty>(var, data, g);
  asLocal<ColorProperty>(var, data, g);
  asLocal<BooleanProperty>(var, data, g);
  asLocal<StringProperty>(var, data, g);
  delete d;
}

// simple structure to hold an output property parameter
// while running an algorithm
struct OutPropertyParam {
  std::string name;        // the name of the parameter
  PropertyInterface *dest; // the destination property
  PropertyInterface *tmp;  // the temporary property

  OutPropertyParam(const std::string &pName) : name(pName), dest(nullptr), tmp(nullptr) {}
};

class AlgorithmPreviewHandler : public ProgressPreviewHandler {
  Graph *graph;
  const std::vector<OutPropertyParam> &outPropParams;
  bool inited;
  std::map<std::string, tlp::PropertyInterface *> outPropsMap;

public:
  AlgorithmPreviewHandler(Graph *g, std::vector<OutPropertyParam> &opp)
      : graph(g), outPropParams(opp), inited(false) {}

  ~AlgorithmPreviewHandler() override {
    if (!outPropsMap.empty()) {
      // build outPropsMap with initial properties
      for (const auto &outPropParam : outPropParams) {
        const std::string &outPropName = outPropParam.dest->getName();

        if (outPropParam.tmp && !outPropName.empty())
          outPropsMap[outPropName] = outPropParam.dest;
      }

      // restore initial properties
      TalipotMainWindow::getInstance()->setGlMainViewPropertiesForGraph(graph, outPropsMap);
    }
  }

  void progressStateChanged(int, int) override {
    if (!inited) {
      // build outPropsMap with temporary properties
      for (const auto &outPropParam : outPropParams) {
        const std::string &outPropName = outPropParam.dest->getName();

        if (outPropParam.tmp && !outPropName.empty())
          outPropsMap[outPropName] = outPropParam.tmp;
      }

      inited = true;

      if (!outPropsMap.empty() &&
          // set temporary properties as drawing properties
          (TalipotMainWindow::getInstance()->setGlMainViewPropertiesForGraph(graph, outPropsMap) ==
           false))
        // clear map if there is nothing to do
        outPropsMap.clear();
    }

    // draw with temporary computed properties
    if (!outPropsMap.empty()) {
      TalipotMainWindow::getInstance()->centerPanelsForGraph(graph, true, true);
    }
  }
};

#define TN(T) typeid(T).name()

void AlgorithmRunnerItem::run(Graph *g) {
  initModel();

  if (g == nullptr)
    g = _graph;

  if (g == nullptr) {
    qCritical() << QStringToTlpString(name()) << ": No graph selected";
    return;
  }

  Observable::holdObservers();
  DataSet originalDataSet =
      static_cast<ParameterListModel *>(_ui->parameters->model())->parametersValues();

  DataSet dataSet(originalDataSet);

  // ensure each input property
  // is a local one when it exits
  std::string algorithm = QStringToTlpString(_pluginName);
  ParameterDescriptionList paramList = PluginsManager::getPluginParameters(algorithm);
  for (const ParameterDescription &desc : paramList.getParameters()) {
    if (desc.getDirection() == IN_PARAM) {

      std::string typeName(desc.getTypeName());

      if (DataType::isTalipotProperty(typeName)) {
        PropertyInterface *prop = nullptr;
        dataSet.get(desc.getName(), prop);

        if (prop != nullptr) {
          PropertyInterface *localProp = g->getProperty(prop->getName());

          if (prop != localProp)
            dataSet.set(desc.getName(), localProp);
        }
      }
    }
  }

  g->push();

  if (_storeResultAsLocal)
    copyToLocal(dataSet, g);

  std::vector<std::string> outNonPropertyParams;
  // use temporary output properties
  // to ease the undo in case of failure
  std::vector<OutPropertyParam> outPropertyParams;
  for (const ParameterDescription &desc : paramList.getParameters()) {
    std::string typeName(desc.getTypeName());

    // forget non property out param
    if (!DataType::isTalipotProperty(typeName)) {
      if (desc.getDirection() != IN_PARAM)
        outNonPropertyParams.push_back(desc.getName());

      continue;
    }

    auto displayMandatoryMessage = [&]() {
      QString message("The mandatory property parameter\n'");
      message += tlp::tlpStringToQString(desc.getName());
      message += "' cannot be empty.";
      qCritical() << message;
      QMessageBox::critical(parentWidget(), name(), message);
    };

    if (desc.getDirection() == IN_PARAM) {
      if (desc.isMandatory()) {
        // if it is a mandatory input property
        // check it is not null
        PropertyInterface *prop = nullptr;
        dataSet.get(desc.getName(), prop);

        if (prop == nullptr) {
          g->pop();
          Observable::unholdObservers();
          displayMandatoryMessage();
          return;
        }
      }

      continue;
    }

    OutPropertyParam outPropParam(desc.getName());
    // get destination property
    dataSet.get(desc.getName(), outPropParam.dest);
    // if it is a mandatory property
    // it cannot be null
    if (desc.isMandatory() && !outPropParam.dest) {
      g->pop();
      Observable::unholdObservers();
      displayMandatoryMessage();
      return;
    }

    // clone it in a not registered (because unnamed)
    // temporary property
    outPropParam.tmp = outPropParam.dest
                           ? outPropParam.dest->clonePrototype(outPropParam.dest->getGraph(), "")
                           : nullptr;
    // set the temporary as the destination property
    dataSet.set(desc.getName(), outPropParam.tmp);

    if (outPropParam.tmp) {
      outPropertyParams.push_back(outPropParam);

      if (desc.getDirection() == OUT_PARAM) {
        DataMem *nodeData = outPropParam.dest->getNodeDefaultDataMemValue();
        DataMem *edgeData = outPropParam.dest->getEdgeDefaultDataMemValue();
        outPropParam.tmp->setAllNodeDataMemValue(nodeData);
        outPropParam.tmp->setAllEdgeDataMemValue(edgeData);
        delete nodeData;
        delete edgeData;
      } else
        // inout property
        outPropParam.tmp->copy(outPropParam.dest);
    }
  }

  std::string errorMessage;
  PluginProgress *progress = TalipotMainWindow::getInstance()->progress();
  progress->setTitle(algorithm);

  // set preview handler if needed
  if (!outPropertyParams.empty())
    progress->setPreviewHandler(new AlgorithmPreviewHandler(g, outPropertyParams));
  else
    progress->showPreview(false);

  // take time before run
  QTime start = QTime::currentTime();
  bool result = g->applyAlgorithm(algorithm, errorMessage, &dataSet, progress);

  // get spent time
  int spentTime = start.msecsTo(QTime::currentTime());

  if (!outPropertyParams.empty())
    progress->setPreviewHandler(nullptr);

  if (!result) {
    if (progress->state() == TLP_CANCEL && errorMessage.empty()) {
      errorMessage = "Cancelled by user";
      tlp::warning() << QStringToTlpString(name()) << ": " << errorMessage;
      QMessageBox::warning(parentWidget(), name(), errorMessage.c_str());
    } else {
      tlp::error() << QStringToTlpString(name()) << ": " << errorMessage;
      QMessageBox::critical(parentWidget(), name(), errorMessage.c_str());
    }
    progress->setComment("Cancelling graph changes...");
    g->pop();
  } else {
    if (progress->state() == TLP_STOP) {
      errorMessage = "Stopped by user";
      tlp::warning() << QStringToTlpString(name()) << ": " << errorMessage;
      QMessageBox::warning(parentWidget(), name(), errorMessage.c_str());
    }
  }

  if (result) {
    progress->setComment("Applying graph changes...");
    // copy or cleanup out properties
    for (const OutPropertyParam &opp : outPropertyParams) {
      // copy computed property in the original output property
      opp.dest->copy(opp.tmp);
      // restore it in the dataset
      dataSet.set(opp.name, opp.dest);

      if (opp.name == "result" && Settings::instance().isResultPropertyStored()) {
        // store the result property values in an automatically named property
        std::string storedResultName =
            algorithm + " - " + originalDataSet.toString() + "(" + opp.dest->getName() + ")";
        PropertyInterface *storedResultProp =
            opp.dest->clonePrototype(opp.dest->getGraph(), storedResultName);
        storedResultProp->copy(opp.tmp);
      }
    }

    // display spentTime if needed
    if (Settings::instance().logPluginCall() != Settings::NoLog) {
      std::stringstream log;
      log << algorithm.c_str() << " - " << dataSet.toString().c_str();

      if (Settings::instance().logPluginCall() == Settings::LogCallWithExecutionTime)
        log << ": " << spentTime << "ms";

      qDebug() << log.str().c_str();
    }
  }

  for (const OutPropertyParam &opp : outPropertyParams) {
    delete opp.tmp;
  }

  afterRun(g, dataSet);

  if (result && !outNonPropertyParams.empty()) {
    // only show computed value of non property output parameters.
    // output property params are not taken into account
    // because they may have been created on the fly
    // (local properties see copyToLocal function above)
    // and thus they may be deleted further in case of undo
    for (unsigned int i = 0; i < outNonPropertyParams.size(); ++i) {
      tlp::DataType *dataType = dataSet.getData(outNonPropertyParams[i]);
      originalDataSet.setData(outNonPropertyParams[i], dataType);
    }

    ParameterListModel *model = static_cast<ParameterListModel *>(_ui->parameters->model());
    model->setParametersValues(originalDataSet);
  }

  while (Observable::observersHoldCounter() > 0)
    Observable::unholdObservers();

  g->popIfNoUpdates();

  // keep progress alive until the end
  // to give feedback to user
  delete progress;
}

void AlgorithmRunnerItem::setStoreResultAsLocal(bool m) {
  _storeResultAsLocal = m;
}

void AlgorithmRunnerItem::mousePressEvent(QMouseEvent *ev) {
  if (ev->button() == Qt::LeftButton)
    _dragStartPosition = ev->pos();
}

void AlgorithmRunnerItem::mouseMoveEvent(QMouseEvent *ev) {
  if (!(ev->buttons() & Qt::LeftButton) ||
      (ev->pos() - _dragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
    QWidget::mouseMoveEvent(ev);
    return;
  }

  QDrag *drag = new QDrag(this);
  const Plugin &p = PluginsManager::pluginInformation(QStringToTlpString(_pluginName).c_str());
  QPixmap icon(QPixmap(p.icon().c_str()).scaled(64, 64));
  QFont f;
  f.setBold(true);
  QFontMetrics metrics(f);
  int textHeight =
      metrics
          .boundingRect(0, 0, icon.width(), INT_MAX,
                        Qt::AlignTop | Qt::AlignHCenter | Qt::TextWordWrap, _pluginName)
          .height();
  QPixmap pix(icon.width() + textHeight, icon.height() + textHeight);
  pix.fill(Qt::white);
  QPainter painter(&pix);
  painter.drawPixmap(pix.width() / 2 - icon.width() / 2, 0, icon.width(), icon.height(), icon);
  painter.setFont(f);
  painter.drawText(0, icon.height(), pix.width(), pix.height() - icon.height(),
                   Qt::AlignCenter | Qt::AlignHCenter | Qt::TextWordWrap, _pluginName);
  painter.setBrush(Qt::transparent);
  painter.setPen(QColor(169, 169, 169));
  painter.drawRect(0, 0, pix.width() - 1, pix.height() - 1);
  drag->setPixmap(pix);

  initModel();
  AlgorithmMimeType *mimeData = new AlgorithmMimeType(
      name(), static_cast<ParameterListModel *>(_ui->parameters->model())->parametersValues());
  connect(mimeData, SIGNAL(mimeRun(tlp::Graph *)), this, SLOT(run(tlp::Graph *)));
  drag->setMimeData(mimeData);
  drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void AlgorithmRunnerItem::afterRun(Graph *g, const tlp::DataSet &dataSet) {
  std::string stdName = QStringToTlpString(name());

  if (PluginsManager::pluginExists<LayoutAlgorithm>(stdName)) {
    if (Settings::instance().isAutomaticRatio()) {
      LayoutProperty *prop = nullptr;
      dataSet.get<LayoutProperty *>("result", prop);

      if (prop)
        prop->perfectAspectRatio(g);
    }

    if (Settings::instance().isAutomaticCentering())
      TalipotMainWindow::getInstance()->centerPanelsForGraph(g);
  } else if (Settings::instance().isAutomaticCentering() &&
             PluginsManager::pluginExists<Algorithm>(stdName) &&
             !PluginsManager::pluginExists<PropertyAlgorithm>(stdName) &&
             !PluginsManager::pluginExists<GraphTest>(stdName)) {
    TalipotMainWindow::getInstance()->centerPanelsForGraph(g);
  } else if (PluginsManager::pluginExists<DoubleAlgorithm>(stdName) &&
             Settings::instance().isAutomaticMapMetric()) {
    DoubleProperty *prop = nullptr;
    dataSet.get<DoubleProperty *>("result", prop);

    if ((prop != nullptr) && (prop->getName().compare("viewMetric") == 0)) {
      bool applyMapping = prop->numberOfNonDefaultValuatedNodes() != 0;

      std::string errMsg;
      ColorProperty *color;

      if (g->existLocalProperty("viewColor")) {
        color = g->getLocalColorProperty("viewColor");
        if (!applyMapping && (color->numberOfNonDefaultValuatedNodes() != 0)) {
          color->setAllNodeDataMemValue(color->getNodeDefaultDataMemValue());
          color->setAllEdgeDataMemValue(color->getEdgeDefaultDataMemValue());
        }
      } else {
        color = g->getLocalColorProperty("viewColor");
        ColorProperty *ancestorColor = g->getSuperGraph()->getColorProperty("viewColor");
        if (!applyMapping && (ancestorColor->numberOfNonDefaultValuatedNodes(g) != 0)) {
          // same default values as ancestor property default values
          color->setAllNodeDataMemValue(ancestorColor->getNodeDefaultDataMemValue());
          color->setAllEdgeDataMemValue(ancestorColor->getEdgeDefaultDataMemValue());
        }
      }

      if (applyMapping) {
        // set value of "color scale" parameter of "Color Mapping" plugin
        // to the user defined value
        tlp::DataSet data;
        ColorScale cs;

        if (colorMappingModel)
          colorMappingModel->parametersValues().get<ColorScale>("color scale", cs);
        else
          cs = ColorScalesManager::getLatestColorScale();

        data.set<ColorScale>("color scale", cs);
        g->applyPropertyAlgorithm("Color Mapping", color, errMsg, &data);
      }
    }
  } else if (PluginsManager::pluginExists<GraphTest>(stdName)) {
    bool result = true;
    dataSet.get<bool>("result", result);
    std::string str = "\"" + stdName + "\" test " + (result ? "succeeded" : "failed") + " on:\n" +
                      g->getName() + ".";

    if (result) {
      tlp::debug() << str << std::endl;
      QMessageBox::information(parentWidget(), "Talipot test result", tlp::tlpStringToQString(str));
    } else {
      tlp::warning() << str << std::endl;
      QMessageBox::warning(parentWidget(), "Talipot test result", tlp::tlpStringToQString(str));
    }
  }
}

void AlgorithmRunnerItem::setFavorite(bool f) {
  _ui->favoriteCheck->setChecked(f);
}

void AlgorithmRunnerItem::favoriteChanged(int state) {
  _ui->favoriteCheck->setToolTip((state == Qt::Unchecked) ? QString("Add to Favorites")
                                                          : QString("Remove from Favorites"));
}

tlp::DataSet AlgorithmRunnerItem::data() const {
  if (_ui->parameters->model() == nullptr)
    return DataSet();

  return static_cast<ParameterListModel *>(_ui->parameters->model())->parametersValues();
}

ParameterListModel *AlgorithmRunnerItem::colorMappingModel = nullptr;

void AlgorithmRunnerItem::initModel() {
  if (_ui->parameters->model() != nullptr)
    return;

  ParameterListModel *model =
      new ParameterListModel(PluginsManager::getPluginParameters(QStringToTlpString(_pluginName)),
                             _graph, _ui->parameters);

  if (_pluginName == "Color Mapping") {
    colorMappingModel = model;
    tlp::DataSet data = model->parametersValues();
    data.set<ColorScale>("color scale", ColorScalesManager::getLatestColorScale());
    model->setParametersValues(data);
  }

  _ui->parameters->setModel(model);
  int h = 10;

  for (int i = 0; i < model->rowCount(); ++i)
    h += _ui->parameters->rowHeight(i);

  _ui->parameters->setMinimumSize(_ui->parameters->minimumSize().width(), h);
  _ui->parameters->setMaximumSize(_ui->parameters->maximumSize().width(), h);

  if (!_initData.empty()) {
    DataSet dataSet = model->parametersValues();
    for (const std::pair<std::string, tlp::DataType *> &it : _initData.getValues()) {
      dataSet.setData(it.first, it.second);
    }
    model->setParametersValues(dataSet);
  }
}
