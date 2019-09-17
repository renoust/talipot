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

#ifndef TULIPWELCOMEPAGE_H
#define TULIPWELCOMEPAGE_H

#include <QWidget>

class QVBoxLayout;

namespace Ui {
class TulipWelcomePageData;
}

class QNetworkReply;

/**
  @brief The widget containing the Welcome screen in tulip_app.
  This widget will allow a user to select an installed perspective, read latest news from the Tulip
  website, and display startup tips.
  */
class TulipWelcomePage : public QWidget {
  Q_OBJECT
public:
  explicit TulipWelcomePage(QWidget *parent = nullptr);
  ~TulipWelcomePage() override;

signals:
  void openPerspective(QString);
  void openProject();
  void openFile(QString);

protected slots:
  void rssReply(QNetworkReply *);
  void openLink(const QString &link);
  void recentFileLinkActivated(const QString &);

private:
  Ui::TulipWelcomePageData *_ui;
};

#endif // TULIPWELCOMEPAGE_H
