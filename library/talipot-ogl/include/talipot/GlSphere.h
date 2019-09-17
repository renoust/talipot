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

#ifndef TALIPOT_GL_SPHERE_H
#define TALIPOT_GL_SPHERE_H

#include <talipot/config.h>

#include <talipot/Coord.h>
#include <talipot/Color.h>

#include <talipot/GlSimpleEntity.h>

namespace tlp {

/**
 * @ingroup OpenGL
 * @brief Class to create a sphere with GlEntity system
 */
class TLP_GL_SCOPE GlSphere : public GlSimpleEntity {

public:
  /**
   * @brief Constructor
   *
   * @warning Don't use this constructor
   */
  GlSphere() {}

  ~GlSphere() override;

  /**
   * @brief Create a sphere with a position, a radius a fill color and multiple rotation (if you
   * want)
   */
  GlSphere(const Coord &position, float radius, const Color &color = Color(0, 0, 0, 255),
           float rotX = 0, float rotY = 0, float rotZ = 0);

  /**
   * @brief Create a sphere with a position, a radius, a texture, an alphe and multiple rotation (if
   * you want)
   */
  GlSphere(const Coord &position, float radius, const std::string &textureFile, int alpha = 255,
           float rotX = 0, float rotY = 0, float rotZ = 0);

  /**
   * @brief Draw the sphere
   */
  void draw(float lod, Camera *camera) override;

  /**
   * @brief Translate entity
   */
  void translate(const Coord &mouvement) override;

  /**
   * @brief Get absolute position
   */
  const Coord &getPosition() const {
    return position;
  }

  /**
   * @brief Set absolute position
   */
  void setPosition(const Coord &pos) {
    position = pos;
  }

  /**
   * @brief Set the texture name
   */
  virtual void setTexture(const std::string &texture) {
    textureFile = texture;
  }

  /**
   * @brief Get the color
   */
  const Color &getColor() const {
    return color;
  }

  /**
   * @brief Set the color
   */
  void setColor(const Color &newColor) {
    color = newColor;
  }

  /**
   * @brief Function to export data in outString (in XML format)
   */
  void getXML(std::string &outString) override;

  /**
   * @brief Function to set data with inString (in XML format)
   */
  void setWithXML(const std::string &inString, unsigned int &currentPosition) override;

private:
  void generateBuffers(int space);

  Coord position;
  float radius;
  Color color;
  std::string textureFile;
  Coord rot;

  std::vector<unsigned int> buffers;
  std::vector<float> vertices;
  std::vector<float> texturesCoord;
  std::vector<unsigned short> indices;
  unsigned int verticesCount;
};
} // namespace tlp

#endif // TALIPOT_GL_SPHERE_H
