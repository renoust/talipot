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

#ifndef ABOUTTULIPPAGE_H
#define ABOUTTULIPPAGE_H

#include <tulip/tulipconf.h>

#include <QWidget>

namespace Ui {
class AboutTulipPageData;
}

namespace tlp {

/**
  @brief The "About us/Getting help" screen displayed in the Tulip app startup window.
  This widget is mainly meant to provide links to the user and developer documentations, direct user
  to Tulip forums
  and provide basic offline support.
  */
class TLP_QT_SCOPE AboutTulipPage : public QWidget {
  Q_OBJECT
public:
  explicit AboutTulipPage(QWidget *parent = nullptr);
  ~AboutTulipPage() override;

private slots:

  void openUrlInBrowser(const QString &url);

private:
  Ui::AboutTulipPageData *_ui;
};
} // namespace tlp

#endif // ABOUTTULIPPAGE_H

///@endcond
