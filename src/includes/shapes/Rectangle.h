#pragma once

#include "Shape.h"

class Rectangle: protected Shape {
  private:
    float width, height;

  public:
    Rectangle(float, float, float, float, std::shared_ptr<Shader>, const char*);
    ~Rectangle();

    glm::vec3 get_center_vec();
};