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

#ifndef TULIPFONTDIALOG_H
#define TULIPFONTDIALOG_H

#include <QDialog>

#include <tulip/tulipconf.h>
#include <tulip/TulipFont.h>

namespace Ui {
class TulipFontDialog;
}

namespace tlp {

class TLP_QT_SCOPE TulipFontDialog : public QDialog {
  Q_OBJECT

  Ui::TulipFontDialog *_ui;
  TulipFont previousFont;
  int ok;

public:
  explicit TulipFontDialog(QWidget *parent = nullptr);
  ~TulipFontDialog() override;
  TulipFont font() const;
  int fontSize() const;
  void done(int res) override {
    ok = res;
    QDialog::done(res);
  }
  TulipFont getSelectedFont() const {
    return ok == QDialog::Accepted ? font() : previousFont;
  }
  void showEvent(QShowEvent *ev) override;

  static TulipFont getFont(QWidget *parent = nullptr, const TulipFont &selectedFont = TulipFont());

public slots:
  void selectFont(const TulipFont &);

protected slots:
  void fontChanged();
};
} // namespace tlp

#endif // TULIPFONTDIALOG_H
///@endcond
