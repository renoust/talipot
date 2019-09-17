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

#ifndef PARALLELCOORDINATESVIEWQUICKACCESSBAR_H
#define PARALLELCOORDINATESVIEWQUICKACCESSBAR_H

#include <tulip/QuickAccessBar.h>

namespace tlp {

class ParallelCoordsDrawConfigWidget;

class ParallelCoordinatesViewQuickAccessBar : public tlp::QuickAccessBarImpl {
  Q_OBJECT

  ParallelCoordsDrawConfigWidget *_optionsWidget;

public:
  ParallelCoordinatesViewQuickAccessBar(ParallelCoordsDrawConfigWidget *opt,
                                        QWidget *parent = nullptr);

public slots:
  void reset() override;
  void setNodesVisible(bool) override;
  void setBackgroundColor(const QColor &) override;
  void setLabelsVisible(bool) override;
};
} // namespace tlp
#endif
