#pragma once

#include <memory>
#include "Shape.h"


class Circle: public Shape {
  private:
    Circle( const double r, std::shared_ptr<BufferData> buffer );
    double radius;

  public:
    static std::shared_ptr<Circle> create(
      const double x, const double y, const double r, const std::shared_ptr<Shader> shader, const char* texturePath, const size_t quality = 200
    );
    ~Circle();

    glm::vec3 get_center_vec();
};
