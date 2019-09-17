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

#ifndef PARAMETERLISTMODEL_H
#define PARAMETERLISTMODEL_H

#include <vector>

#include <tulip/TulipModel.h>
#include <tulip/WithParameter.h>
#include <tulip/DataSet.h>

namespace tlp {

class TLP_QT_SCOPE ParameterListModel : public TulipModel {
  std::vector<ParameterDescription> _params;
  tlp::DataSet _data;
  tlp::Graph *_graph;

public:
  explicit ParameterListModel(const tlp::ParameterDescriptionList &params,
                              tlp::Graph *graph = nullptr, QObject *parent = nullptr);
  tlp::DataSet parametersValues() const;
  void setParametersValues(const tlp::DataSet &data);

  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;
};
} // namespace tlp

#endif // PARAMETERLISTMODEL_H
///@endcond
