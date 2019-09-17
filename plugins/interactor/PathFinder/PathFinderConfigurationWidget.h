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

#ifndef PATHFINDERCONFIGURATIONWIDGET_H_
#define PATHFINDERCONFIGURATIONWIDGET_H_

#include <QWidget>

#include <string>

namespace Ui {
class PathFinderConfigurationData;
}

class QString;

namespace tlp {
class PathFinderConfigurationWidget : public QWidget {
  Q_OBJECT

  Ui::PathFinderConfigurationData *_ui;

public:
  PathFinderConfigurationWidget(QWidget *parent = nullptr);
  ~PathFinderConfigurationWidget() override;

  void addweightComboItem(const QString &s);
  void setCurrentweightComboIndex(const int i);
  int weightComboFindText(const QString &text) const;
  void addedgeOrientationComboItem(const QString &s);
  void setCurrentedgeOrientationComboIndex(const int i);
  int edgeOrientationComboFindText(const QString &text) const;
  void addpathsTypeComboItem(const QString &s);
  void toleranceChecked(const bool checked);
  void setToleranceSpinValue(const int val);
  void highlightersLabelDisabled(const bool disable);
  void addbottomWidget(QWidget *w);
  void toleranceDisabled(const bool disabled);

signals:
  void setWeightMetric(const QString &);
  void setEdgeOrientation(const QString &);
  void setPathsType(const QString &);
  void activateTolerance(bool);
  void setTolerance(int);
};
} // namespace tlp
#endif /* PATHFINDERCONFIGURATIONWIDGET_H_ */
