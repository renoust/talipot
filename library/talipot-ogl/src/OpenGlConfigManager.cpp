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

#include <GL/glew.h>

#include <talipot/OpenGlConfigManager.h>
#include <talipot/ParallelTools.h>

#include <iostream>
#include <sstream>

//====================================================

using namespace std;

namespace tlp {

bool OpenGlConfigManager::_glewIsInit = false;
bool OpenGlConfigManager::_antialiased = true;
std::unordered_map<std::string, bool> OpenGlConfigManager::_checkedExtensions;

void OpenGlConfigManager::initExtensions() {
  if (!_glewIsInit) {
    glewExperimental = true;
    _glewIsInit = (glewInit() == GLEW_OK);
  }
}

string OpenGlConfigManager::getOpenGLVersionString() {
  return reinterpret_cast<const char *>(glGetString(GL_VERSION));
}

double OpenGlConfigManager::getOpenGLVersion() {
  double ret = 0;
  std::istringstream iss(getOpenGLVersionString()); //.substr(0,3));
  iss >> ret;
  return ret;
}

string OpenGlConfigManager::getOpenGLVendor() {
  return string(reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
}

bool OpenGlConfigManager::isExtensionSupported(const string &extensionName) {
  if (!_glewIsInit)
    return false;

  bool supported = false;
  TLP_LOCK_SECTION(OpenGlConfigManagerExtensionSupported) {
    auto it = _checkedExtensions.find(extensionName);
    if (it == _checkedExtensions.end()) {
      supported = _checkedExtensions[extensionName] =
          (glewIsSupported(extensionName.c_str()) == GL_TRUE);
    } else
      supported = it->second;
  }
  TLP_UNLOCK_SECTION(OpenGlConfigManagerExtensionSupported);
  return supported;
}

bool OpenGlConfigManager::hasVertexBufferObject() {
  return isExtensionSupported("GL_ARB_vertex_buffer_object");
}

void OpenGlConfigManager::activateAntiAliasing() {
  if (_antialiased) {
    glEnable(GL_MULTISAMPLE);
  }
}

void OpenGlConfigManager::desactivateAntiAliasing() {
  glDisable(GL_MULTISAMPLE);
}

int OpenGlConfigManager::maxNumberOfSamples() {
  static int maxSamples = -1;

  if (maxSamples < 0)
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

  return maxSamples / 4;
}
} // namespace tlp
