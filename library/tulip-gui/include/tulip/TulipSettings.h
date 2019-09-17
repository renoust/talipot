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

#ifndef TULIPSETTINGS_H
#define TULIPSETTINGS_H

#include <QSettings>
#include <QNetworkProxy>
#include <QString>

#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Size.h>
#include <tulip/Graph.h>

namespace tlp {

/**
 * @brief This class provides convenience functions to access the Tulip settings file (using
 * QSettings)
 * TulipSettings is a wrapper for QSettings providing quick access to common keys provided in the
 * tulip configuration file.
 * This object does not mask any method from the QSettings class, which mean that the user can
 * still access custom keys by invoking the QSettings::value method.
 */
class TLP_QT_SCOPE TulipSettings : public QSettings, GlDefaultSelectionColorManager, Observable {
  Q_OBJECT
  Q_ENUMS(DisplayProperty)

public:
  static TulipSettings &instance();

  static QString elementKey(const QString &configEntry, tlp::ElementType elem);

  void synchronizeViewSettings();

  /**
    @brief Retrieves the list of documents recently opened with tulip.
    */
  QStringList recentDocuments() const;

  /**
    @brief Registers a file in the list of recently opened documents.
    If the list already contains 5 elements, the last one (in chronological order) will be removed.
    */
  void addToRecentDocuments(const QString &);

  /**
    @brief Check recently opened documents list and remove non-existing ones
    */
  void checkRecentDocuments();

  /**
   * @brief Adds a remote location from which to list plugins.
   *
   * @param remoteLocation The URL of the plugin server.
   * @return void
   **/
  void addRemoteLocation(const QString &remoteLocation);

  /**
   * @brief Removes a location from which to list plugins.
   *
   * @param remoteLocation The URL of the plugin server.
   * @return void
   **/
  void removeRemoteLocation(const QString &remoteLocation);

  /**
   * @brief Retrieves the list of locations from which to list plugins.
   *
   * @return const QStringList
   **/
  const QStringList remoteLocations() const;
  const QStringList pluginsToRemove() const;

  void markPluginForRemoval(const QString &pluginLibrary);
  void unmarkPluginForRemoval(const QString &pluginLibrary);

  tlp::Color defaultColor(tlp::ElementType elem, bool tulipDefault = false);
  void setDefaultColor(tlp::ElementType elem, const tlp::Color &color);

  tlp::Color defaultLabelColor(bool tulipDefault = false);
  void setDefaultLabelColor(const tlp::Color &color);

  tlp::Size defaultSize(tlp::ElementType elem, bool tulipDefault = false);
  void setDefaultSize(tlp::ElementType elem, const tlp::Size &size);

  int defaultShape(tlp::ElementType elem, bool tulipDefault = false);
  void setDefaultShape(tlp::ElementType elem, int shape);

  // in GlDefaultSelectionColorManager interface
  tlp::Color defaultSelectionColor(bool tulipDefault = false) override;

  void setDefaultSelectionColor(const tlp::Color &color);

  QSet<QString> favoriteAlgorithms() const;
  void addFavoriteAlgorithm(const QString &name);
  void removeFavoriteAlgorithm(const QString &name);

  // Proxy settings
  bool isProxyEnabled() const;
  void setProxyEnabled(bool);

  QNetworkProxy::ProxyType proxyType() const;
  void setProxyType(QNetworkProxy::ProxyType);

  QString proxyHost() const;
  void setProxyHost(const QString &);

  unsigned int proxyPort() const;
  void setProxyPort(unsigned int);

  bool isUseProxyAuthentification() const;
  void setUseProxyAuthentification(bool);

  QString proxyUsername() const;
  void setProxyUsername(const QString &);

  QString proxyPassword() const;
  void setProxyPassword(const QString &);

  void applyProxySettings();

  bool isFirstRun() const;
  void setFirstRun(bool);

  bool isFirstTulipMMRun() const;
  void setFirstTulipMMRun(bool);

  bool displayDefaultViews() const;
  void setDisplayDefaultViews(bool);

  bool isAutomaticMapMetric() const;
  void setAutomaticMapMetric(bool);

  bool isAutomaticCentering() const;
  void setAutomaticCentering(bool);

  bool isAutomaticRatio() const;
  void setAutomaticRatio(bool);

  bool isViewOrtho() const;
  void setViewOrtho(bool);

  bool isResultPropertyStored() const;
  void setResultPropertyStored(bool);

  enum LogPluginCall { NoLog = 0, LogCall = 1, LogCallWithExecutionTime = 2 };
  unsigned int logPluginCall() const;
  void setLogPluginCall(unsigned int);

  bool isUseTlpbFileFormat() const;
  void setUseTlpbFileFormat(bool);

  unsigned int seedOfRandomSequence() const;
  void setSeedOfRandomSequence(unsigned int);
  void initSeedOfRandomSequence();

  bool warnUserAboutGraphicsCard() const;
  void setWarnUserAboutGraphicsCard(bool);

  bool showStatusBar() const;
  void setShowStatusBar(bool);

  bool loggerAnchored() const;
  void setLoggerAnchored(bool);

  void treatEvent(const Event &message) override;

private:
  TulipSettings();
  static TulipSettings *_instance;

  void setFavoriteAlgorithms(const QSet<QString> &lst);
};
} // namespace tlp
#endif // TULIPSETTINGS_H
///@endcond
