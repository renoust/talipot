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

#ifndef HEADER_FRAME_H
#define HEADER_FRAME_H

#include <QWidget>

namespace Ui {
class HeaderFrameData;
}

class HeaderFrame : public QWidget {
  Q_OBJECT

  QPair<int, int> _oldHeightInfo;

  Ui::HeaderFrameData *_ui;
  Q_PROPERTY(QString title READ title WRITE setTitle)
  QString _title;

  Q_PROPERTY(bool expandable READ isExpandable WRITE setExpandable)

  Q_PROPERTY(bool expanded READ isExpanded WRITE expand)
  bool _expanded;

  Q_PROPERTY(QStringList menus READ menus WRITE setMenus)

public:
  explicit HeaderFrame(QWidget *parent = nullptr);
  ~HeaderFrame() override;

  QString title() const;
  QStringList menus() const;
  QString currentMenu() const;
  int currentMenuIndex() const;

  bool isExpandable() const;
  bool isExpanded() const;

  QWidget *expandControl() const;
  QWidget *mainFrame() const;

public slots:
  void setTitle(const QString &title);
  void setMenus(const QStringList &menus);
  void setExpandable(bool f);

  void expand(bool e);
  void insertWidget(QWidget *);

signals:
  void menuChanged(const QString &);
  void expanded(bool);

protected slots:
  void setExpanded(bool e);
};

#endif // HEADER_FRAME_H
