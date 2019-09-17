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

#ifndef STRINGEDITOR_H
#define STRINGEDITOR_H

#include <QString>
#include <QTextEdit>
#include <QDialog>

#include <tulip/tulipconf.h>

namespace tlp {
/**
 * @brief Simple String edition widget.
 **/
class TLP_QT_SCOPE StringEditor : public QDialog {
  QString currentString;
  QTextEdit *edit;

public:
  explicit StringEditor(QWidget *parent = nullptr);

  QString getString() const;
  void setString(const QString &qstr);

  // redefinition to ensure it is shown in the center of its parent
  void showEvent(QShowEvent *ev) override;

public slots:
  // redefinition to ensure to catch the end of input
  void done(int r) override;
};
} // namespace tlp

#endif // STRINGEDITOR_H
///@endcond
