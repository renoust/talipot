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
#include "tulip/TulipModel.h"

#include <QFont>

using namespace tlp;

TulipModel::TulipModel(QObject *parent) : QAbstractItemModel(parent) {}

TulipModel::~TulipModel() {}

QVariant TulipModel::headerData(int, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal && role == Qt::FontRole) {
    QFont f;
    f.setBold(true);
    f.setPointSize(f.pointSize() - 1);
    return f;
  }

  return QVariant();
}
