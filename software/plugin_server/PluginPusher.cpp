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
#include <cstdlib>

#include "WebDavManager.h"
#include <QDir>
#include <QDomDocument>
#include <tulip/PluginManager.h>
#include <tulip/TlpQtTools.h>

using namespace std;

void uploadfolder(const QString &origin, WebDavManager &manager) {
  if (!manager.folderExists(origin)) {
    manager.mkdir(origin);
  }

  QDir originDir(origin);

  for (const QString &element : originDir.entryList(QDir::Files | QDir::NoSymLinks)) {
    QFile file(originDir.canonicalPath() + "/" + element);
    bool opened = file.open(QIODevice::ReadOnly);

    if (opened) {
      manager.putFile(origin + "/" + element, &file);
      file.close();
    } else {
      std::cout << "could not open file! :'(" << std::endl;
    }
  }

  for (const QString &element :
       originDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot)) {
    uploadfolder(origin + "/" + element, manager);
  }
}

int main(int argc, char **argv) {
  if (argc < 4) {
    std::cout << "pushPlugin pluginsPath serverURL base64EncodedCredentials [destinationPath]"
              << std::endl;
    cout << "destinationPath defaults to pluginsPath" << endl;
    exit(0);
  }

  QCoreApplication app(argc, argv);

  QString path = argv[1];
  QString serverURL = argv[2];
  QString credentials = argv[3];
  QString destinationPath = path;

  if (argc > 4) {
    destinationPath = argv[4];
  }

  WebDavManager manager("webdav." + serverURL, destinationPath, credentials);

  QFile description(path + "/serverDescription.xml");

  if (!description.open(QIODevice::ReadOnly)) {
    std::cout << "could not open " << tlp::QStringToTlpString(path)
              << "/serverDescription.xml in Read mode." << std::endl;
    exit(0);
  }

  QString localDescription(description.readAll());

  QString location = "http://www." + serverURL + destinationPath;

  QNetworkAccessManager networkManager;
  QNetworkReply *reply =
      networkManager.get(QNetworkRequest(QUrl(location + "/serverDescription.xml")));

  while (!reply->isFinished()) {
    QCoreApplication::processEvents();
  }

  QString remoteDescription(reply->readAll());

  QStringList pluginList;

  QDomDocument localDocument;
  localDocument.setContent(localDescription);
  QDomNodeList localPlugins(localDocument.elementsByTagName("plugin"));

  for (int i = 0; i < localPlugins.count(); ++i) {
    QDomNode currentNode(localPlugins.at(i));
    QDomElement currentElement(currentNode.toElement());
    QString pluginName(currentElement.attribute("name"));
    pluginList << pluginName;
  }

  if (!remoteDescription.isEmpty()) {
    QDomDocument remoteDocument;
    remoteDocument.setContent(remoteDescription);
    std::cout << tlp::QStringToTlpString(remoteDescription) << std::endl;
    QDomNodeList remotePlugins(remoteDocument.elementsByTagName("plugin"));

    for (int i = 0; i < remotePlugins.count(); ++i) {
      QDomNode currentNode(remotePlugins.at(i));
      QDomElement currentElement(currentNode.toElement());
      QString pluginName(currentElement.attribute("name"));

      if (!pluginList.contains(pluginName)) {
        localDocument.documentElement().appendChild(currentNode);
        std::cout << "keeping remote plugin: " << tlp::QStringToTlpString(pluginName) << std::endl;
      } else {
        std::cout << "discarding remote plugin: " << tlp::QStringToTlpString(pluginName)
                  << std::endl;
      }
    }
  }

  // Debug output
  //   QFile outputXML("serverDescription.xml");
  //   outputXML.open(QIODevice::ReadWrite);
  //   outputXML.write(localDocument.toByteArray());
  //   outputXML.close();
  description.close();
  description.open(QIODevice::ReadWrite | QIODevice::Truncate);
  description.write(localDocument.toByteArray());
  description.close();

  uploadfolder(path, manager);
}
