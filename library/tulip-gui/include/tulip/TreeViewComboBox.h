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

#ifndef TREEVIEWCOMBOBOX_H
#define TREEVIEWCOMBOBOX_H

#include <QComboBox>
#include <QTreeView>

#include <tulip/tulipconf.h>

class TLP_QT_SCOPE TreeViewComboBox : public QComboBox {

  Q_OBJECT
  QTreeView *_treeView;
  bool _skipNextHide;
  bool _popupVisible;
  QModelIndex _lastIndex;

public:
  explicit TreeViewComboBox(QWidget *parent = nullptr);

  void setModel(QAbstractItemModel *model);

  void showPopup() override;
  void hidePopup() override;

  QModelIndex selectedIndex() const;

  bool eventFilter(QObject *, QEvent *) override;

public slots:

  void selectIndex(const QModelIndex &);
  void rowsRemoved(const QModelIndex &, int, int);
  void currentIndexChanged();

signals:

  void currentItemChanged();
};

#endif // TREEVIEWCOMBOBOX_H
///@endcond
