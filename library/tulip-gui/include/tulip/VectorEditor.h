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

#ifndef VECTOREDITOR_H
#define VECTOREDITOR_H

#include <QDialog>
#include <QAbstractButton>

#include <tulip/tulipconf.h>

namespace Ui {
class VectorEditor;
}

class TLP_QT_SCOPE VectorEditor : public QDialog {
  Q_OBJECT
  Ui::VectorEditor *_ui;

  int _userType;
  QVector<QVariant> currentVector;

public:
  explicit VectorEditor(QWidget *parent = nullptr);
  ~VectorEditor() override;
  void setVector(const QVector<QVariant> &d, int userType);
  const QVector<QVariant> &vector() const {
    return currentVector;
  }

public slots:
  void add();
  void remove();
  void done(int r) override;
};

#endif // VECTOREDITIONWIDGET_H
///@endcond
