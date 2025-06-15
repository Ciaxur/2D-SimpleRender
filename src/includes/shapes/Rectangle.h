#pragma once

#include <memory>
#include "Shape.h"

class Rectangle: public Shape {
  private:
    double width, height;
    Rectangle( const double width, const double height, const glm::vec3& origin, std::shared_ptr<BufferData> buffer );

  public:
    static std::shared_ptr<Rectangle> create(
      double x, double y, double width, double height, std::shared_ptr<Shader> shader, const char* texturePath
    );
    ~Rectangle();

    glm::vec3 get_center_vec();
};
