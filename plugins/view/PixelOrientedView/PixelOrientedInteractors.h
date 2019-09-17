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

#ifndef PIXELORIENTEDVIEWINTERACTORS_H_
#define PIXELORIENTEDVIEWINTERACTORS_H_

#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include "../../utils/PluginNames.h"

namespace tlp {

class PixelOrientedInteractor : public NodeLinkDiagramComponentInteractor {

public:
  PixelOrientedInteractor(const QString &iconPath, const QString &text,
                          const unsigned int priority = 0);

  bool isCompatible(const std::string &viewName) const override;
};

class PixelOrientedInteractorNavigation : public PixelOrientedInteractor {

public:
  PLUGININFORMATION(InteractorName::PixelOrientedInteractorNavigation, "Tulip Team", "02/04/2009",
                    "Pixel Oriented Navigation Interactor", "1.0", "Navigation")

  PixelOrientedInteractorNavigation(const tlp::PluginContext *);

  void construct() override;
};
} // namespace tlp

#endif /* PIXELORIENTEDVIEWINTERACTORS_H_ */
