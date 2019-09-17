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

#ifndef MATRIXVIEWQUICKACCESSBAR_H
#define MATRIXVIEWQUICKACCESSBAR_H

#include <tulip/QuickAccessBar.h>

namespace tlp {

class MatrixViewConfigurationWidget;

class MatrixViewQuickAccessBar : public tlp::QuickAccessBarImpl {
  Q_OBJECT

  MatrixViewConfigurationWidget *_optionsWidget;

public:
  MatrixViewQuickAccessBar(MatrixViewConfigurationWidget *opt, QWidget *parent = nullptr);

public slots:
  void reset() override;
  void setEdgesVisible(bool) override;
  void setBackgroundColor(const QColor &) override;
  void setColorInterpolation(bool) override;
};
} // namespace tlp
#endif // SCATTERPLOTQUICKACCESSBAR_H
