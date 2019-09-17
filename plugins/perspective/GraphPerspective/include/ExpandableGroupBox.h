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
#ifndef EXPANDABLEGROUPBOX_H
#define EXPANDABLEGROUPBOX_H

#include <QGroupBox>
#include <QMap>

class ExpandableGroupBox : public QGroupBox {
  Q_OBJECT

  Q_PROPERTY(bool expanded READ expanded WRITE setExpanded)
  bool _expanded;

  Q_PROPERTY(QWidget *widget READ widget WRITE setWidget)
  QWidget *_widget;

public:
  explicit ExpandableGroupBox(QWidget *parent = nullptr, const QString &title = QString());
  ~ExpandableGroupBox() override;
  bool expanded() const {
    return _expanded;
  }

  QWidget *widget() const {
    return _widget;
  }
  QWidget *takeWidget();

public slots:
  void setExpanded(bool e);
  void setWidget(QWidget *);
};

#endif // EXPANDABLEGROUPBOX_H
