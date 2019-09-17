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

#include <QString>
#include <QFile>
#include <QDir>
#include <QXmlStreamWriter>
#include <QLibrary>
#include <QDebug>
#include <QApplication>
#include <QDateTime>

#include <tulip/SystemDefinition.h>
#include <tulip/PluginLister.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/QuaZIPFacade.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TlpTools.h>
#include <tulip/Interactor.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>

#include <fcntl.h>

using namespace std;
using namespace tlp;

struct PluginInformationCollector : public PluginLoader {

  void loaded(const tlp::Plugin *info, const std::list<Dependency> &) override {
    _directoryPlugins[_currentDirectory].push_back(tlp::tlpStringToQString(info->name()));
  }

  void aborted(const std::string &plugin, const std::string &message) override {
    std::cout << "failed to load plugin " << plugin << ": " << message << std::endl;
  }
  void loading(const std::string &) override {}
  void finished(bool, const std::string &) override {}
  void start(const std::string &) override {}

  QString _currentDirectory;
  QMap<QString, QStringList> _directoryPlugins;
};

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  if (argc < 2) {
    cerr << argv[0] << " pluginPath [destinationDir]" << endl;
    return EXIT_FAILURE;
  }

  QString destinationDir = QDir::currentPath();

  if (argc >= 3)
    destinationDir = argv[2];

  QFileInfo destInfo(destinationDir);
  QDir::home().mkpath(destInfo.absoluteFilePath());

  // First we initialize Tulip with basic plugins to ensure dependencies consistency
  tlp::initTulipLib(tlp::QStringToTlpString(QApplication::applicationDirPath()).c_str());
  tlp::PluginLibraryLoader::loadPlugins();
  tlp::PluginLister::checkLoadedPluginsDependencies(nullptr);
  tlp::InteractorLister::initInteractorsDependencies();
  tlp::GlyphManager::loadGlyphPlugins();
  tlp::EdgeExtremityGlyphManager::loadGlyphPlugins();

  QDir outputDir(destinationDir);

  // Next: we load additional plugins from external project and ZIP data into output directory
  PluginInformationCollector collector;
  QDir pluginServerDir(argv[1]);
  PluginLister::currentLoader = &collector;

  for (const QFileInfo &component :
       pluginServerDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
    collector._currentDirectory = component.fileName();
    QDir pluginDir(component.absoluteFilePath());
    QDir::home().mkpath(outputDir.absoluteFilePath(component.fileName()));

    if (!QuaZIPFacade::zipDir(pluginDir.absolutePath(),
                              outputDir.absoluteFilePath(component.fileName() + QDir::separator() +
                                                         "data-" + OS_PLATFORM + OS_ARCHITECTURE +
                                                         ".zip"))) {
      qFatal("Failed to zip archive");
    }

    pluginDir.cd("lib");
    pluginDir.cd("tulip");

    for (const QFileInfo &pluginFile : pluginDir.entryInfoList(QDir::Files | QDir::NoSymLinks)) {
      if (QLibrary::isLibrary(pluginFile.absoluteFilePath())) {
        PluginLibraryLoader::loadPluginLibrary(
            tlp::QStringToTlpString(pluginFile.absoluteFilePath()), &collector);
      }
    }
  }

  QFile outputXML(outputDir.absoluteFilePath("server.xml"));
  outputXML.open(QIODevice::Truncate | QIODevice::WriteOnly);
  QXmlStreamWriter stream(&outputXML);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();
  stream.writeStartElement("server");
  stream.writeAttribute("serverName", destinationDir);
  stream.writeAttribute("lastUpdate", QDateTime::currentDateTime().toString(Qt::ISODate));
  stream.writeAttribute("release", TULIP_VERSION);
  stream.writeStartElement("plugins");

  for (const QString &component : collector._directoryPlugins.keys()) {
    for (const QString &plugin : collector._directoryPlugins[component]) {
      // Server description
      stream.writeStartElement("plugin");
      stream.writeAttribute("name", plugin);
      stream.writeAttribute("path", component);
      const Plugin &info = PluginLister::pluginInformation(tlp::QStringToTlpString(plugin));
      stream.writeAttribute("category", info.category().c_str());
      stream.writeAttribute("author", tlp::tlpStringToQString(info.author()));
      stream.writeAttribute("date", info.date().c_str());
      stream.writeAttribute("desc", tlp::tlpStringToQString(info.info()));
      stream.writeAttribute("release", info.release().c_str());
      stream.writeAttribute("tulip", (info.tulipMajor() + '.' + info.tulipMinor()).c_str());
      stream.writeStartElement("dependencies");
      const std::list<Dependency> &deps = PluginLister::getPluginDependencies(info.name());

      for (std::list<Dependency>::const_iterator it = deps.begin(); it != deps.end(); ++it) {
        stream.writeStartElement("dependency");
        stream.writeAttribute("name", tlp::tlpStringToQString(it->pluginName));
        stream.writeEndElement(); // dependency
      }

      stream.writeEndElement(); // dependencies
      stream.writeEndElement(); // plugin
    }
  }

  stream.writeEndElement(); // plugins
  stream.writeEndElement(); // server
  stream.writeEndDocument();
  outputXML.close();

  for (const QFileInfo &phpFile : QDir(":/tulip/pluginpackager/php/").entryInfoList(QDir::Files)) {
    QFile::copy(phpFile.absoluteFilePath(), QDir(destinationDir).filePath(phpFile.fileName()));
  }

  qDebug() << "Output stored in" << outputDir.absolutePath();

  return EXIT_SUCCESS;
}
