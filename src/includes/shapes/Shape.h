#pragma once

#include <vector>

// Graphics libraries.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Project Libraries
#include "Texture.h"
#include "BufferData.h"

class Shape {
  protected:
    glm::vec3 origin;

  public:
    BufferData buffer;

  public:
    Shape();
    virtual ~Shape();

    /**
     * Translates the shape by the given vector. This is done by mutating the
     * internal buffer and updating the buffer storage.
     *
     * @param v 2D Vector to traslate entity by.
    */
    virtual void translate(const glm::vec2&);

    /**
     * Returns the center of the stored shape.
     */
    virtual glm::vec3 get_center_vec() = 0;

    /**
     * Sets the shape's origin.
     *
     * @param x Origin at x-axis.
     * @param y Origin at y-axis.
     */
    void set_origin(glm::vec3);

    /** Returns the shape's origin. */
    glm::vec3 get_origin();

    /**
     * Rotates the current shape instance, in-place, by the given radians.
     *
     * @param radians Radians to rotate the shape by.
     */
    void rotate(const float);

    /** Updates entity state */
    void update();
};
