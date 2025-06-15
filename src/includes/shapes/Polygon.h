#pragma once

#include <memory>
#include "Shape.h"

class Polygon: public Shape {
  private:
    double width, height;
    Polygon(const double width, const double height, const glm::vec3 &origin, std::shared_ptr<BufferData> buffer);

  public:
    static std::shared_ptr<Polygon> create(const std::vector<glm::vec2> &vertices, std::shared_ptr<Shader> shader, const char* texturePath);
    ~Polygon();

    glm::vec3 get_center_vec();
};
