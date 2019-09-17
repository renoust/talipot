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

#ifndef GLYPHSCALECONFIGDIALOG_H_
#define GLYPHSCALECONFIGDIALOG_H_

#include <QDialog>
#include <QStringList>

namespace Ui {
class GlyphScaleConfigDialogData;
}

class QShowEvent;
class QWidget;

namespace tlp {

class GlyphScaleConfigDialog : public QDialog {

  Q_OBJECT

public:
  GlyphScaleConfigDialog(QWidget *parent = nullptr);
  ~GlyphScaleConfigDialog() override;

  std::vector<int> getSelectedGlyphsId() const;

  void showEvent(QShowEvent *event) override;

private slots:

  void nbGlyphsSpinBoxValueChanged(int value);

private:
  QStringList glyphsNameList;
  Ui::GlyphScaleConfigDialogData *_ui;
};
} // namespace tlp
#endif /* GLYPHSCALECONFIGDIALOG_H_ */
