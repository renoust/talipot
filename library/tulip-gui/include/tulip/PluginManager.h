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
///@cond DOXYGEN_HIDDEN

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QStringList>

#include <tulip/tulipconf.h>

class QNetworkReply;

namespace tlp {

class Plugin;

struct TLP_QT_SCOPE PluginVersionInformation {
  bool isValid;

  QString libraryLocation;
  QString author;
  QString version;
  QString icon;
  QString description;
  QString date;

  QStringList dependencies;

  PluginVersionInformation();
  PluginVersionInformation(const PluginVersionInformation &copy);
  PluginVersionInformation &operator=(const PluginVersionInformation &) = default;
};

struct TLP_QT_SCOPE PluginInformation {
  QString name;
  QString category;
  PluginVersionInformation installedVersion;
  PluginVersionInformation availableVersion;

  PluginInformation();
  PluginInformation(const PluginInformation &copy);
  PluginInformation &operator=(const PluginInformation &) = default;

  void fillLocalInfo(const tlp::Plugin &info);
};

class TLP_QT_SCOPE PluginManager {
  static QStringList _markedForInstallation;

public:
  enum PluginLocation { Remote = 0x01, Local = 0x02 };
  Q_DECLARE_FLAGS(PluginLocations, PluginLocation)

  static const QString STABLE_LOCATION;
  static const QString TESTING_LOCATION;

  typedef QList<PluginVersionInformation> PluginVersionInformationList;

  typedef QList<PluginInformation> PluginInformationList;

  static void addRemoteLocation(const QString &location);
  static void removeRemoteLocation(const QString &location);
  static QStringList remoteLocations();

  static PluginInformationList listPlugins(PluginLocations locations,
                                           const QString &nameFilter = QString(),
                                           const QString &categoryFilter = QString());

  static void markForRemoval(const QString &plugin);
  static void markForInstallation(const QString &plugin, QObject *recv, const char *progressSlot);

  static QStringList markedForInstallation();
  static QStringList markedForRemoval();
  static void unmarkForRemoval(const QString &file);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PluginManager::PluginLocations)
} // namespace tlp

#endif // PLUGINMANAGER_H
///@endcond
