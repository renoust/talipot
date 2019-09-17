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

#include <talipot/config.h>
#include <talipot/Release.h>

#include <iostream>

using namespace std;

static std::ostream *debugStream = nullptr;
std::ostream &tlp::debug() {
  return debugStream ? *debugStream : std::cout;
}
void tlp::setDebugOutput(std::ostream &os) {
  debugStream = &os;
}

static std::ostream *warningStream = nullptr;
std::ostream &tlp::warning() {
  return warningStream ? *warningStream : std::cerr;
}
void tlp::setWarningOutput(std::ostream &os) {
  warningStream = &os;
}

static std::ostream *errorStream = nullptr;
std::ostream &tlp::error() {
  return errorStream ? *errorStream : std::cerr;
}
void tlp::setErrorOutput(std::ostream &os) {
  errorStream = &os;
}

std::string tlp::getTalipotVersion() {
  return TALIPOT_VERSION;
}
