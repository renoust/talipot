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

#ifndef TEXTUREFILEDIALOG_H
#define TEXTUREFILEDIALOG_H

#include <QDialog>

#include <tulip/tulipconf.h>
#include <tulip/TulipMetaTypes.h>
#include "ui_TextureFileDialog.h"

namespace Ui {
class TextureFileDialogData;
}

namespace tlp {

/**
 * @brief Provide a dialog that allow the user to choose
 * a file whose name may be empty
 *
 *
 **/
class TLP_QT_SCOPE TextureFileDialog : public QDialog {
  Q_OBJECT
public:
  Ui::TextureFileDialogData *ui;
  TextureFile _data;
  int ok;
  TextureFileDialog(QWidget *parent = nullptr);

  ~TextureFileDialog() override;

  void done(int res) override;

  void setData(const TextureFile &tf);

  const TextureFile &data() {
    return _data;
  }

  void showEvent(QShowEvent *ev) override;

public slots:
  void browse();
};
} // namespace tlp
#endif // TEXTUREFILEDIALOG_H
///@endcond
