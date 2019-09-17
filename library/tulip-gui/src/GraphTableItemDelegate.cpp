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
#include <tulip/GraphTableItemDelegate.h>

#include <QDebug>

#include <tulip/GraphModel.h>
#include <tulip/DoubleProperty.h>
#include <tulip/TulipMetaTypes.h>

using namespace tlp;
using namespace std;

GraphTableItemDelegate::GraphTableItemDelegate(QObject *parent) : TulipItemDelegate(parent) {}

void GraphTableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const {
  PropertyInterface *pi = index.data(TulipModel::PropertyRole).value<PropertyInterface *>();

  if (index.data().type() == QVariant::Double && dynamic_cast<DoubleProperty *>(pi) != nullptr) {
    DoubleProperty *prop = static_cast<DoubleProperty *>(pi);
    double value = index.data().value<double>();
    double min = 0, max = 0;

    if (index.data(TulipModel::IsNodeRole).value<bool>()) {
      min = prop->getNodeMin(index.data(TulipModel::GraphRole).value<tlp::Graph *>());
      max = prop->getNodeMax(index.data(TulipModel::GraphRole).value<tlp::Graph *>());
    }

    if (max != min) {
      painter->setBrush(QColor(200, 200, 200));
      painter->setPen(QColor(200, 200, 200));
      painter->drawRect(option.rect.x(), option.rect.y() + 1,
                        ((value - min) / (max - min)) * option.rect.width(),
                        option.rect.height() - 2);
    }
  }

  TulipItemDelegate::paint(painter, option, index);
}
