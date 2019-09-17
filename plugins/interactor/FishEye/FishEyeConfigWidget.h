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

#ifndef FISHEYECONFIGWIDGET_H_
#define FISHEYECONFIGWIDGET_H_

#include <QWidget>

namespace Ui {
class FishEyeConfigWidget;
}

namespace tlp {

class FishEyeConfigWidget : public QWidget {

  Ui::FishEyeConfigWidget *_ui;

public:
  FishEyeConfigWidget(QWidget *parent = nullptr);
  ~FishEyeConfigWidget() override;

  int getFishEyeType() const;

  float getFishEyeRadius() const;
  void setFishEyeRadius(const float radius);

  float getFishEyeHeight() const;
  void setFishEyeHeight(const float height);

  float getFishEyeRadiusIncrementStep() const;
  float getFishEyeHeightIncrementStep() const;
};
} // namespace tlp
#endif /* FISHEYECONFIGWIDGET_H_ */
