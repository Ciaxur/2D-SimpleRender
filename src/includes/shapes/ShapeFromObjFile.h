#pragma once

#include <memory>
#include "Shape.h"

class ShapeFromObjFile: public Shape {
  private:
    double width, height;
    ShapeFromObjFile(
      const double width, const double height, const glm::vec3& origin, const std::shared_ptr<BufferData> buffer
    );

  public:
    static std::shared_ptr<ShapeFromObjFile> create( const std::string& filepath );
    ~ShapeFromObjFile();

    glm::vec3 get_center_vec();
};
