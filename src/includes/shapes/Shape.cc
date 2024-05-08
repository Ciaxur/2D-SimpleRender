#include "Shape.h"
#include <spdlog/spdlog.h>

Shape::Shape(): origin(0.f) {}
Shape::~Shape() {}

void Shape::set_origin(glm::vec3 origin) {
  this->origin = origin;
}

glm::vec3 Shape::get_origin() {
  return this->origin;
}

inline double Shape::get_buffer_length() {
  return this->buffer.data_size_bytes / sizeof(GL_FLOAT);
}

void Shape::translate(const glm::vec2 &t) {
  for (size_t i = 0; i < this->get_buffer_length(); i += buffer.stride) {
    buffer.data_ptr[i]      += t.x;
    buffer.data_ptr[i + 1]  += t.y;
  }

  // Update origin to reflect translation.
  this->set_origin( this->get_origin() + glm::vec3(t, 0.f) );
}

void Shape::iter_buffer(std::function<glm::vec3(glm::vec3&)> fn) {
  // Create a translation to push/pop on shape's origin.
  glm::mat4 push_translate_to_origin  = glm::translate(glm::mat4(1.f), -this->origin);
  glm::mat4 pop_translate_to_origin   = glm::translate(glm::mat4(1.f),  this->origin);

  for (size_t i = 0; i < this->get_buffer_length(); i += buffer.stride) {
    const glm::vec3 vertex {
      buffer.data_ptr[i],
      buffer.data_ptr[i + 1],
      buffer.data_ptr[i + 2]
    };

    // Sprinkle some math magic.
    // 1. Translate to the shape's origin
    glm::vec3 result = glm::vec3( push_translate_to_origin * glm::vec4(vertex, 1.f) );

    // 2. Apply on vertex.
    result = fn(result);

    // 3. Pop origin translation.
    result = glm::vec3( pop_translate_to_origin * glm::vec4(result, 1.f) );

    // Update inner data buffer.
    buffer.data_ptr[i]      = result.x;
    buffer.data_ptr[i + 1]  = result.y;
    buffer.data_ptr[i + 2]  = result.z;
  }
}

void Shape::rotate(const float radians) {
  // Rotate on z-axis since this is 2D.
  constexpr glm::vec3 z_axis { 0.f, 0.f, 1.f };

  // Create rotation matrix.
  const glm::mat4 rot_mat = glm::rotate(
    glm::mat4(1.f),
    radians,
    z_axis
  );

  // Apply rotation matrix on all verticies.
  this->iter_buffer([&](glm::vec3 &slice) -> glm::vec3 {
    return glm::vec3( rot_mat * glm::vec4(slice, 1.f) );
  });
}

void Shape::scale(const glm::vec2& scale) {
  glm::mat4 scale_mat = glm::scale( glm::mat4(1.f), glm::vec3(scale, 1.f) );

  this->iter_buffer([&](glm::vec3 &slice) -> glm::vec3 {
    return glm::vec3( scale_mat * glm::vec4(slice, 1.f) );
  });
}

void Shape::update() {
  buffer.update();
}