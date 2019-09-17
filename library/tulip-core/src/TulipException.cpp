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

#include <tulip/TulipException.h>

using namespace tlp;

TulipException::TulipException(const std::string &desc) : desc(desc) {}
TulipException::~TulipException() throw() {}
const char *TulipException::what() const throw() {
  return desc.c_str();
}
