#pragma once

#include "Shape.h"

class ShapeFromObjFile: protected Shape {
  private:
    double width, height;

  public:
    /**
     * Initializes an instance from a file.
     *
     * @param filepath Path to a .obj file.
    */
    ShapeFromObjFile(std::string filepath);
    ~ShapeFromObjFile();

    glm::vec3 get_center_vec();
};