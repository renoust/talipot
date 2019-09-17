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

#ifndef TULIPITEMDELEGATE_H
#define TULIPITEMDELEGATE_H

#include <QStyledItemDelegate>

#include <tulip/TulipItemEditorCreators.h>
#include <tulip/TulipMetaTypes.h>

namespace tlp {

class TLP_QT_SCOPE TulipItemDelegate : public QStyledItemDelegate {
  Q_OBJECT

  QMap<int, TulipItemEditorCreator *> _creators;

  QObject *_currentMonitoredChild;
  QComboBox *_currentMonitoredCombo;

public:
  static QVariant showEditorDialog(tlp::ElementType, tlp::PropertyInterface *, tlp::Graph *,
                                   TulipItemDelegate *, QWidget *dialogParent = nullptr,
                                   unsigned int id = UINT_MAX);

  explicit TulipItemDelegate(QObject *parent = nullptr);
  ~TulipItemDelegate() override;

  template <typename T>
  void registerCreator(tlp::TulipItemEditorCreator *);

  void unregisterCreator(tlp::TulipItemEditorCreator *);

  template <typename T>
  void unregisterCreator();

  template <typename T>
  tlp::TulipItemEditorCreator *creator() const;

  tlp::TulipItemEditorCreator *creator(int) const;

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;
  QString displayText(const QVariant &value, const QLocale &locale) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

  bool eventFilter(QObject *object, QEvent *event) override;

protected slots:
  void comboDataChanged();
};
} // namespace tlp

#include "cxx/TulipItemDelegate.cxx"

#endif // TULIPITEMDELEGATE_H
///@endcond
