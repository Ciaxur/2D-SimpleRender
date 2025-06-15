#pragma once

#include <functional>

// Graphics libraries.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

// Project Libraries
#include "BufferData.h"

class Shape {
  protected:
    glm::vec3 origin;
    bool _is_hidden;

  public:
    std::shared_ptr<BufferData> buffer;

  protected:
    /* Internal helper function which returns the buffer's length. **/
    inline double get_buffer_length();

    /**
     * Internal helper function which applies a change to the internal buffer
     * by calling the given function on the vec3 vertex slice wrapped in a origin
     * translation.
     *
     * @param fn Function called to apply a change on the vertex argument.
    */
    void iter_buffer(std::function<glm::vec3 (glm::vec3&)>);

  public:
    Shape( const std::shared_ptr<BufferData> buffer );
    virtual ~Shape();

    /**
     * Translates the shape by the given vector. This is done by mutating the
     * internal buffer and updating the buffer storage.
     *
     * @param v 2D Vector to translate entity by.
    */
    virtual void translate(const glm::vec2&);

    /**
     * Returns the center of the stored shape.
     */
    virtual glm::vec3 get_center_vec() = 0;

    /**
     * Sets a new position to the current shape instance.
     *
     * @param v 3D vector to set new position to.
     */
    virtual void set_position(const glm::vec3&);

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

    /**
     * Scales the shape by the given vector.
     *
     * @param scale vec2 scale factor.
     */
    void scale(const glm::vec2&);

    /** Updates entity state */
    void update();

    /**
     * Sets the shape's hidden state.
     *
     * @param state new hidden state.
     */
    void set_hidden(bool state);

    /**
     * Returns the hidden state of the shape.
     */
    bool is_hidden() const;
};
