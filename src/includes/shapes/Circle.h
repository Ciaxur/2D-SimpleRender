#pragma once

#include "Shape.h"


class Circle: protected Shape {
  private:
    double radius;

  public:
    /**
     * Initializes a circle instance.
     *
     * @param x Position on x-axis
     * @param y Position on y-axis
     * @param r Circle's radius
     * @param shader Pointer to the shader used
     * @param texturePath Optional path to the shape texture
     * @param quality Number of points to generate for the circle
    */
    Circle(double, double, double, std::shared_ptr<Shader>, const char*, size_t);
    ~Circle();

  glm::vec3 get_center_vec();
};