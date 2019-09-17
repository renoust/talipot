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
#include <cassert>

#include "tulip/TulipIconicFont.h"
#include "tulip/TulipFontAwesome.h"
#include "tulip/TulipMaterialDesignIcons.h"

using namespace tlp;

std::string TulipIconicFont::getTTFLocation(const std::string &iconName) {
  return (iconName.find("fa-") == 0) ? TulipFontAwesome::getTTFLocation(iconName)
                                     : TulipMaterialDesignIcons::getTTFLocation();
}

std::string TulipIconicFont::getWOFFLocation(const std::string &iconName) {
  assert(isIconSupported(iconName));
  return (iconName.find("fa-") == 0) ? TulipFontAwesome::getWOFFLocation(iconName)
                                     : TulipMaterialDesignIcons::getWOFFLocation();
}

std::string TulipIconicFont::getWOFF2Location(const std::string &iconName) {
  assert(isIconSupported(iconName));
  return (iconName.find("fa-") == 0) ? TulipFontAwesome::getWOFF2Location(iconName)
                                     : TulipMaterialDesignIcons::getWOFF2Location();
}

bool TulipIconicFont::isIconSupported(const std::string &iconName) {
  return (iconName.find("fa-") == 0) ? TulipFontAwesome::isIconSupported(iconName)
                                     : TulipMaterialDesignIcons::isIconSupported(iconName);
}

unsigned int TulipIconicFont::getIconCodePoint(const std::string &iconName) {
  assert(isIconSupported(iconName));
  return (iconName.find("fa-") == 0) ? TulipFontAwesome::getIconCodePoint(iconName)
                                     : TulipMaterialDesignIcons::getIconCodePoint(iconName);
}

std::string TulipIconicFont::getIconFamily(const std::string &iconName) {
  assert(isIconSupported(iconName));
  return (iconName.find("fa-") == 0) ? TulipFontAwesome::getIconFamily(iconName)
                                     : TulipMaterialDesignIcons::getIconFamily(iconName);
}

std::string TulipIconicFont::getIconUtf8String(const std::string &iconName) {
  assert(isIconSupported(iconName));
  return (iconName.find("fa-") == 0) ? TulipFontAwesome::getIconUtf8String(iconName)
                                     : TulipMaterialDesignIcons::getIconUtf8String(iconName);
}
