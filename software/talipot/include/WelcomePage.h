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

#ifndef WELCOME_PAGE_H
#define WELCOME_PAGE_H

#include <QWidget>

class QVBoxLayout;

namespace Ui {
class WelcomePageData;
}

class QNetworkReply;

/**
  @brief The widget containing the Welcome screen in talipot_app.
  This widget will allow a user to select an installed perspective, read latest news from the
  Talipot website, and display startup tips.
  */
class WelcomePage : public QWidget {
  Q_OBJECT
public:
  explicit WelcomePage(QWidget *parent = nullptr);
  ~WelcomePage() override;

signals:
  void openPerspective(QString);
  void openProject();
  void openFile(QString);

protected slots:
  void rssReply(QNetworkReply *);
  void openLink(const QString &link);
  void recentFileLinkActivated(const QString &);

private:
  Ui::WelcomePageData *_ui;
};

#endif // WELCOME_PAGE_H
