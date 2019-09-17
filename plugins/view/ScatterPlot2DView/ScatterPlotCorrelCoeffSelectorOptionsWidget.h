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

#ifndef SCATTERPLOTCORRELCOEFFSELECTOROPTIONSWIDGET_H_
#define SCATTERPLOTCORRELCOEFFSELECTOROPTIONSWIDGET_H_

#include <QWidget>

#include <tulip/Color.h>

class QPushButton;

namespace Ui {
class ScatterPlotCorrelCoeffSelectorOptionsWidgetData;
}

namespace tlp {

class ColorButton;
class ScatterPlotCorrelCoeffSelectorOptionsWidget : public QWidget {

  Q_OBJECT

public:
  ScatterPlotCorrelCoeffSelectorOptionsWidget(QWidget *parent = nullptr);
  ~ScatterPlotCorrelCoeffSelectorOptionsWidget() override;

  Color getMinusOneColor() const;
  Color getZeroColor() const;
  Color getOneColor() const;

protected:
  void showEvent(QShowEvent *event) override;

private slots:
  void updateColorScale();

private:
  void setButtonColor(tlp::ColorButton *button, const Color &color);
  Ui::ScatterPlotCorrelCoeffSelectorOptionsWidgetData *_ui;
};
} // namespace tlp

#endif /* SCATTERPLOTCORRELCOEFFSELECTOROPTIONSWIDGET_H_ */
