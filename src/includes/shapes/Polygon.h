#pragma once

#include "Shape.h"

class Polygon: protected Shape {
  private:
    double width, height;

  public:
    /**
     * Initializes a 2d polygon instance.
     *
     * @param vertices Array of (x,y) points.
     * @param shader Pointer to the shader used
     * @param texturePath Optional path to the shape texture
    */
    Polygon(const std::vector<glm::vec2> &vertices, std::shared_ptr<Shader> shader, const char* texturePath);
    ~Polygon();

    glm::vec3 get_center_vec();
};