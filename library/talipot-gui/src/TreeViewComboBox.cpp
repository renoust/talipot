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

#include "talipot/TreeViewComboBox.h"

#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QMouseEvent>
#include <QTreeView>

class TreeViewDelegate : public QStyledItemDelegate {
public:
  TreeViewDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(result.height() + 10);
    return result;
  }
};

TreeViewComboBox::TreeViewComboBox(QWidget *parent)
    : QComboBox(parent), _treeView(nullptr), _skipNextHide(false), _popupVisible(false) {
  _treeView = new QTreeView(this);
  _treeView->setEditTriggers(QTreeView::NoEditTriggers);
  _treeView->setAlternatingRowColors(true);
  _treeView->setSelectionBehavior(QTreeView::SelectRows);
  _treeView->setRootIsDecorated(false);
  _treeView->setAllColumnsShowFocus(true);
  _treeView->header()->setVisible(false);
  _treeView->setItemDelegate(new TreeViewDelegate(_treeView));
  _treeView->setItemsExpandable(true);
  setView(_treeView);
  view()->viewport()->installEventFilter(this);
  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged()));
}

void TreeViewComboBox::setModel(QAbstractItemModel *model) {
  QComboBox::setModel(model);
  connect(model, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this,
          SLOT(rowsRemoved(const QModelIndex &, int, int)));

  for (int i = 1; i < model->columnCount(); ++i)
    _treeView->hideColumn(i);
}

void TreeViewComboBox::showPopup() {
  setRootModelIndex(QModelIndex());
  _treeView->expandAll();
  _treeView->resizeColumnToContents(0);
  QComboBox::showPopup();
  QWidget *popup = findChild<QFrame *>();

  if (_treeView->columnWidth(0) > popup->width()) {
    popup->resize(_treeView->columnWidth(0), popup->height());
  }

  _popupVisible = true;
}

void TreeViewComboBox::hidePopup() {
  if (!_popupVisible)
    return;

  if (_skipNextHide)
    _skipNextHide = false;
  else {
    QComboBox::hidePopup();
    _popupVisible = false;
    selectIndex(view()->currentIndex());
  }
}

void TreeViewComboBox::selectIndex(const QModelIndex &index) {
  if (!_popupVisible && index != _lastIndex) {
    _lastIndex = index;
    setRootModelIndex(index.parent());
    setCurrentIndex(index.row());
    emit currentItemChanged();
  }
}

QModelIndex TreeViewComboBox::selectedIndex() const {
  return model()->index(currentIndex(), 0, rootModelIndex());
}

bool TreeViewComboBox::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::MouseButtonPress && object == view()->viewport()) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    QModelIndex index = view()->indexAt(mouseEvent->pos());

    if (!view()->visualRect(index).contains(mouseEvent->pos()))
      _skipNextHide = true;
  }

  return false;
}

void TreeViewComboBox::rowsRemoved(const QModelIndex &parent, int, int) {
  QModelIndex currentIndex = selectedIndex();

  if (currentIndex.isValid())
    selectIndex(currentIndex);
  else
    selectIndex(parent);
}

void TreeViewComboBox::currentIndexChanged() {
  selectIndex(selectedIndex());
}
