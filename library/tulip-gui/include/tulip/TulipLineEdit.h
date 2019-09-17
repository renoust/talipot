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

#ifndef TULIPLINEEDIT_H
#define TULIPLINEEDIT_H

#include <QLineEdit>
#include <tulip/tulipconf.h>

namespace tlp {

class TulipLineEdit : public QLineEdit {
public:
  _DEPRECATED TulipLineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {}
};
} // namespace tlp

#endif

///@endcond
