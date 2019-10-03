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

#include <talipot/PluginManager.h>

#include <QDir>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <talipot/Settings.h>
#include <talipot/PluginsManager.h>
#include <talipot/SystemDefinition.h>
#include <talipot/YajlFacade.h>
#include <talipot/QuaZIPFacade.h>
#include <talipot/TlpQtTools.h>

using namespace tlp;

const QString PluginManager::STABLE_LOCATION =
    QString("http://tulip.labri.fr/pluginserver/stable/") + TALIPOT_MM_VERSION;
const QString PluginManager::TESTING_LOCATION =
    QString("http://tulip.labri.fr/pluginserver/testing/") + TALIPOT_MM_VERSION;

QDebug operator<<(QDebug dbg, const PluginVersionInformation &c) {
  dbg.nospace() << "(author " << c.author << ") "
                << "(version " << c.version << ") "
                << "(icon " << c.icon << ") "
                << "(description " << c.description << ") "
                << "(date " << c.date << ") "
                << "(librarylocation " << c.libraryLocation << ") "
                << "(dependencies " << c.dependencies << ") ";

  return dbg.space();
}

QDebug operator<<(QDebug dbg, const PluginInformation &c) {
  dbg.nospace() << "(name " << c.name << ") "
                << "(category " << c.category << ") "
                << "(installed " << c.installedVersion << ") "
                << "(available " << c.availableVersion << ") ";

  return dbg.space();
}

class PluginServerClient : public YajlParseFacade {
  QString _location;

  PluginManager::PluginInformationList _result;
  QString _currentKey;
  QMap<QString, QString> _currentMap;

public:
  PluginServerClient(const QString &location) : _location(location) {}

  void fetch(const QString &name, QObject *recv, const char *progressSlot) {
    QNetworkAccessManager mgr;

    QNetworkReply *reply = nullptr;
    QUrl url(_location + "/fetch.php?os=" + OS_PLATFORM + "&arch=" + OS_ARCHITECTURE +
             "&tulip=" + TALIPOT_MM_VERSION + "&name=" + name);

    do {
      QNetworkRequest request(url);
      reply = mgr.get(request);
      QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), recv, progressSlot);

      while (!reply->isFinished()) {
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
      }

      url = QUrl(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl());
    } while (reply->attribute(QNetworkRequest::RedirectionTargetAttribute).isValid());

    QString tmpOutPath = QDir::temp().absoluteFilePath("talipot_plugin_" + name + ".zip");
    QFile tmpOut(tmpOutPath);
    tmpOut.open(QIODevice::WriteOnly);
    tmpOut.write(reply->readAll());
    tmpOut.close();
    reply->close();
    QuaZIPFacade::unzip(tlp::localPluginsPath(), tmpOutPath);
    tmpOut.remove();
  }

  PluginManager::PluginInformationList list(const QString &nameFilter,
                                            const QString &categoryFilter) {
    _result.clear();
    QNetworkAccessManager mgr;
    QNetworkRequest request(QUrl(_location + "/list.php?os=" + OS_PLATFORM +
                                 "&arch=" + OS_ARCHITECTURE + "&tulip=" + TALIPOT_MM_VERSION +
                                 "&name=" + nameFilter + "&category=" + categoryFilter));
    QNetworkReply *reply = mgr.get(request);

    while (!reply->isFinished()) {
      QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    QByteArray contents = reply->readAll();
    reply->close();
    parse(reinterpret_cast<const unsigned char *>(contents.constData()), contents.size());
    return _result;
  }

  void parseString(const std::string &value) override {
    _currentMap[_currentKey] = tlpStringToQString(value);
  }

  void parseMapKey(const std::string &value) override {
    _currentKey = value.c_str();
  }

  void parseStartMap() override {
    _currentMap.clear();
  }

  void parseEndMap() override {
    PluginInformation info;
    info.name = _currentMap["name"];
    info.category = _currentMap["category"];

    PluginVersionInformation versionInfo;
    versionInfo.description = _currentMap["desc"];
    versionInfo.libraryLocation = _location;
    versionInfo.version = _currentMap["release"];
    versionInfo.author = _currentMap["author"];
    versionInfo.date = _currentMap["date"];
    versionInfo.isValid = true;
    // TODO fill icon
    info.availableVersion = versionInfo;
    _result.push_back(info);
  }
};

void PluginManager::addRemoteLocation(const QString &location) {
  Settings::instance().addRemoteLocation(location);
}

void PluginManager::removeRemoteLocation(const QString &location) {
  Settings::instance().removeRemoteLocation(location);
}

QStringList PluginManager::remoteLocations() {
  return Settings::instance().remoteLocations();
}

QStringList PluginManager::_markedForInstallation = QStringList();

PluginManager::PluginInformationList PluginManager::listPlugins(PluginLocations locations,
                                                                const QString &nameFilter,
                                                                const QString &categoryFilter) {
  QMap<QString, PluginInformation> nameToInfo;

  if (locations.testFlag(Local)) {
    std::list<std::string> localResults = PluginsManager::availablePlugins();

    for (std::list<std::string>::iterator it = localResults.begin(); it != localResults.end();
         ++it) {
      const Plugin &info = PluginsManager::pluginInformation(*it);

      if (QString(info.category().c_str()).contains(categoryFilter) &&
          QString(info.name().c_str()).contains(nameFilter, Qt::CaseInsensitive)) {
        nameToInfo[info.name().c_str()].fillLocalInfo(info);
      }
    }
  }

  if (locations.testFlag(Remote)) {
    for (const QString &loc : remoteLocations()) {
      PluginServerClient client(loc);

      for (const PluginInformation &info : client.list(nameFilter, categoryFilter)) {
        PluginInformation storedInfo = nameToInfo[info.name];
        storedInfo.name = info.name;
        storedInfo.category = info.category;
        storedInfo.availableVersion = info.availableVersion;
        nameToInfo[info.name] = storedInfo;
      }
    }
  }

  PluginInformationList result;

  for (const PluginInformation &i : nameToInfo.values())
    result.push_back(i);

  return result;
}

void PluginManager::markForRemoval(const QString &plugin) {
  Settings::instance().markPluginForRemoval(plugin);
}

void PluginManager::markForInstallation(const QString &plugin, QObject *recv,
                                        const char *progressSlot) {
  PluginInformationList lst = listPlugins(Remote, plugin);

  if (lst.size() == 0 || !lst.first().availableVersion.isValid)
    return;

  PluginVersionInformation version = lst.first().availableVersion;
  PluginServerClient clt(version.libraryLocation);
  clt.fetch(plugin, recv, progressSlot);
  _markedForInstallation.push_back(plugin);
}

QStringList PluginManager::markedForInstallation() {
  return _markedForInstallation;
}

QStringList PluginManager::markedForRemoval() {
  return Settings::instance().pluginsToRemove();
}

void PluginManager::unmarkForRemoval(const QString &file) {
  Settings::instance().unmarkPluginForRemoval(file);
}

PluginInformation::PluginInformation() {}

PluginInformation::PluginInformation(const PluginInformation &copy) {
  name = copy.name;
  category = copy.category;
  installedVersion = copy.installedVersion;
  availableVersion = copy.availableVersion;
}

void PluginInformation::fillLocalInfo(const Plugin &info) {
  name = tlp::tlpStringToQString(info.name());
  category = tlp::tlpStringToQString(info.category());
  installedVersion.description = tlp::tlpStringToQString(info.info());
  installedVersion.icon = tlp::tlpStringToQString(info.icon());
  installedVersion.version = info.release().c_str();
  installedVersion.date = info.date().c_str();
  installedVersion.author = tlp::tlpStringToQString(info.author());
  installedVersion.libraryLocation =
      tlp::tlpStringToQString(PluginsManager::getPluginLibrary(info.name()));
  std::list<tlp::Dependency> dependencies = PluginsManager::getPluginDependencies(info.name());

  for (std::list<tlp::Dependency>::iterator it = dependencies.begin(); it != dependencies.end();
       ++it) {
    installedVersion.dependencies.push_back(it->pluginName.c_str());
  }

  installedVersion.isValid = true;
}

PluginVersionInformation::PluginVersionInformation() : isValid(false) {}

PluginVersionInformation::PluginVersionInformation(const PluginVersionInformation &copy) {
  libraryLocation = copy.libraryLocation;
  author = copy.author;
  version = copy.version;
  icon = copy.icon;
  description = copy.description;
  date = copy.date;
  dependencies = copy.dependencies;
  isValid = copy.isValid;
}
