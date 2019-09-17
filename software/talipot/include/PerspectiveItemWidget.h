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

#ifndef PERSPECTIVE_ITEM_WIDGET_H
#define PERSPECTIVE_ITEM_WIDGET_H

#include <QWidget>

namespace tlp {
struct PluginInformation;
}

namespace Ui {
class PerspectiveItemData;
}

class PerspectiveItemWidget : public QWidget {
  Q_OBJECT

  QString _perspectiveName;
  Ui::PerspectiveItemData *_ui;

public:
  explicit PerspectiveItemWidget(const QString &perspectiveName, QWidget *parent = nullptr);
  ~PerspectiveItemWidget() override;

public slots:
  void run();

signals:
  void selected();

protected:
  void mouseDoubleClickEvent(QMouseEvent *) override;
  void focusInEvent(QFocusEvent *) override;
  void focusOutEvent(QFocusEvent *) override;
};

#endif // PERSPECTIVE_ITEM_WIDGET_H
