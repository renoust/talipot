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

#ifndef TALIPOT_GL_TEXTURE_MANAGER_H
#define TALIPOT_GL_TEXTURE_MANAGER_H

#include <talipot/config.h>

#include <talipot/OpenGlIncludes.h>

#include <set>
#include <unordered_map>
#include <string>

#include <cstdint>

namespace tlp {

struct GlTexture {
  GLuint *id;
  int height;
  int width;
  unsigned int spriteNumber;
};

/**
 *  \brief Class to load textures
 */
class TLP_GL_SCOPE GlTextureLoader {
public:
  /**
   * Load a texture from a file
   * in the default implementation only bmp, jpeg and png files
   * can be loaded.
   * Return false if an error occurs
   */
  virtual bool loadTexture(const std::string &filename, GlTexture &texture);

  virtual ~GlTextureLoader() {}
};

/**
 * class to load/store textures need by OpenGL rendering
 */
class TLP_GL_SCOPE GlTextureManager {

  typedef std::unordered_map<std::string, GlTexture> TextureUnit;
  typedef std::unordered_map<uintptr_t, TextureUnit> ContextAndTextureMap;

public:
  /**
   * Change the current OpenGl context (each OpenGl window have a different OpenGl context)
   */
  static void changeContext(uintptr_t context);
  /**
   * Remove all textures of an OpenGl context and remove this context
   */
  static void removeContext(uintptr_t context);

  /**
   * Return texture info (id, width and height) for the given name
   */
  static GlTexture getTextureInfo(const std::string &);

  /**
   * Check if a texture fo the given name exists in the current context
   */
  static bool existsTexture(const std::string &filename);
  /**
   * Load texture with given name
   */
  static bool loadTexture(const std::string &);
  /**
   * Remove texture with given name
   */
  static void deleteTexture(const std::string &);
  /**
   * Begin a new texture with given name
   */
  static void beginNewTexture(const std::string &);
  /**
   * Activate a texture with given name
   */
  static bool activateTexture(const std::string &, unsigned int);
  /**
   * Activate a texture with given name
   */
  static bool activateTexture(const std::string &);
  /**
   * Disable texture with given name
   */
  static void desactivateTexture();
  /**
   * Set animationStep for next textures (for next activateTexture)
   */
  static inline void setAnimationFrame(unsigned int id) {
    animationFrame = id;
  }
  /**
   * Get animationStep of next textures
   */
  static inline unsigned int getAnimationFrame() {
    return animationFrame;
  }
  /**
   * Clear vector of textures with error
   */
  static inline void clearErrorVector() {
    texturesWithError.clear();
  }
  /**
   * Remove an entry of vector of textures with error
   */
  static inline void removeEntryOfErrorVector(const std::string &name) {
    texturesWithError.erase(name);
  }

  /**
   * Register an external texture is GlTextureManager
   */
  static void registerExternalTexture(const std::string &textureName, const GLuint textureId);

  /**
   * Get Texture loader
   */
  static inline GlTextureLoader *getTextureLoader() {
    return loader ? loader : (loader = new GlTextureLoader());
  }

  /**
   * Set Texture loader
   */
  static void setTextureLoader(GlTextureLoader *texLoader) {
    if (loader)
      delete loader;

    loader = texLoader;
  }

private:
  static GlTextureLoader *loader;

  static uintptr_t currentContext;

  static ContextAndTextureMap texturesMap;
  static std::set<std::string> texturesWithError;

  static unsigned int animationFrame;
};
} // namespace tlp

#endif // TALIPOT_GL_TEXTURE_MANAGER_H
///@endcond
