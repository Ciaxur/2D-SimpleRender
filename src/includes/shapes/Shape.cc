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

void Shape::translate(const glm::vec2 &t) {
  double buff_array_len = this->buffer.data_size_bytes / sizeof(GL_FLOAT);
  for (size_t i = 0; i < buff_array_len; i += buffer.stride) {
    buffer.data_ptr[i]      += t.x;
    buffer.data_ptr[i + 1]  += t.y;
  }

  // Update origin to reflect translation.
  this->set_origin( this->get_origin() + glm::vec3(t, 0.f) );
}

void Shape::rotate(const float radians) {
  // Rotate on z-axis since this is 2D.
  constexpr glm::vec3 z_axis { 0.f, 0.f, 1.f };

  // Create rotation matrix.
  const glm::mat4 rotMat = glm::rotate(
    glm::mat4(1.f),
    radians,
    z_axis
  );

  // Create a translation to push/pop on shape's origin.
  glm::mat4 push_translate_to_origin  = glm::translate(glm::mat4(1.f), -this->origin);
  glm::mat4 pop_translate_to_origin   = glm::translate(glm::mat4(1.f),  this->origin);

  // Apply the rotation on all verticies.
  double buff_array_len = this->buffer.data_size_bytes / sizeof(GL_FLOAT);
  for (GLsizei i = 0; i < (GLsizei)buff_array_len; i += buffer.stride) {
    const glm::vec3 vertex {
      buffer.data_ptr[i],
      buffer.data_ptr[i + 1],
      buffer.data_ptr[i + 2]
    };

    // Sprinkle some math magic.
    // 1. Translate to the shape's origin
    glm::vec3 result = glm::vec3( push_translate_to_origin * glm::vec4(vertex, 1.f) );

    // 2. Apply rotation.
    result = glm::vec3( rotMat * glm::vec4(result, 1.f) );

    // 3. Pop origin translation.
    result = glm::vec3( pop_translate_to_origin * glm::vec4(result, 1.f) );

    // Update inner data buffer.
    buffer.data_ptr[i]      = result.x;
    buffer.data_ptr[i + 1]  = result.y;
    buffer.data_ptr[i + 2]  = result.z;
  }
}

void Shape::update() {
  buffer.update();
}