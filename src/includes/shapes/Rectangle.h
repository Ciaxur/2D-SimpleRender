#pragma once

#include "Shape.h"

class Rectangle: protected Shape {
  private:
    float width, height;

  public:
    /**
     * Initializes a rectangle instance.
     *
     * @param x Position on x-axis
     * @param y Position on y-axis
     * @param width Width of the rectangle
     * @param height Height of the rectangle
     * @param shader Pointer to the shader used
     * @param texturePath Optional path to the shape texture
    */
    Rectangle(float, float, float, float, std::shared_ptr<Shader>, const char*);
    ~Rectangle();

    glm::vec3 get_center_vec();
};