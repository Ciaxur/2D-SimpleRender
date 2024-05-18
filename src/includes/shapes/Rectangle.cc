#include "Rectangle.h"

Rectangle::Rectangle(double x, double y, double width, double height, std::shared_ptr<Shader> shader, const char* texturePath) {
  this->width = width;
  this->height = height;
  this->set_origin(glm::vec3{ x, y, 0.f });

  GLdouble verticies[] = {
    // VERTEX<vec3>		                        RGBA<vec4>					      // Texture Coordinates<vec2>
    x,              y,             0.0f,      1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom-left
    x + width,      y,             0.0f,      0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom-right
    x,              y + height,    0.0f,      0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,   // top-left
    x + width,      y + height,    0.0f,      0.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f    // top-right
  };

  GLuint indicies[] = {
    0, 3, 1,
    0, 2, 3
  };

  this->buffer = CreateBuffer::dynamic_float(verticies, sizeof(verticies), indicies, sizeof(indicies), shader);
  if (texturePath)
    this->buffer.texture = new Texture(texturePath);
};

Rectangle::~Rectangle() {}

glm::vec3 Rectangle::get_center_vec() {
  const double x0 = this->buffer.vertex_buffer_ptr[0];
  const double y0 = this->buffer.vertex_buffer_ptr[1];
  const double z0 = this->buffer.vertex_buffer_ptr[2];

  return glm::vec3(
    // Half of the rectangle's width, offset at the x-axis.
    x0 + (this->width / 2.f),

    // Y-axis is inversed (0 at bottom and HEIGHT at top).
    // Same as x, half of the heigh, offset at the y-axis.
    y0 + (this->height / 2.f),

    // Meh.
    z0
  );
}