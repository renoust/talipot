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

#ifndef TALIPOT_COORD_EDITOR_H
#define TALIPOT_COORD_EDITOR_H

#include <QDialog>

#include <talipot/Coord.h>
#include <talipot/config.h>

namespace Ui {
class CoordEditor;
}

namespace tlp {
/**
 * @brief Simple Coord edition widget.
 **/
class TLP_QT_SCOPE CoordEditor : public QDialog {
  Q_OBJECT
  Q_PROPERTY(Coord coord READ coord WRITE setCoord NOTIFY coordChanged)

  Ui::CoordEditor *ui;
  Coord currentCoord;

public:
  explicit CoordEditor(QWidget *parent = nullptr, bool editSize = false);
  ~CoordEditor() override;

  tlp::Coord coord() const;

  // redefinition to ensure it is shown in the center of its parent
  void showEvent(QShowEvent *ev) override;

public slots:
  void setCoord(const tlp::Coord &s);
  // redefinition to ensure to catch the end of input
  void done(int r) override;

signals:
  void coordChanged(tlp::Coord coord);

private slots:
  void coordUpdated();
};
}

#endif // TALIPOT_COORD_EDITOR_H
///@endcond
