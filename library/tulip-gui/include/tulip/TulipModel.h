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

#ifndef TULIPMODEL_H
#define TULIPMODEL_H

#include <QAbstractItemModel>

#include <tulip/tulipconf.h>

namespace tlp {

class TLP_QT_SCOPE TulipModel : public QAbstractItemModel {
  Q_OBJECT
  Q_ENUMS(TulipRole)

public:
  enum TulipRole {
    GraphRole = Qt::UserRole + 1,
    PropertyRole = Qt::UserRole + 2,
    IsNodeRole = Qt::UserRole + 3,
    StringRole = Qt::UserRole + 4,
    MandatoryRole = Qt::UserRole + 5,
    ElementIdRole = Qt::UserRole + 6
  };

  explicit TulipModel(QObject *parent = nullptr);
  ~TulipModel() override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

signals:
  void checkStateChanged(QModelIndex, Qt::CheckState);
};
} // namespace tlp

#endif // TULIPMODEL_H
///@endcond
