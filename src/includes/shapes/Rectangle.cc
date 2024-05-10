#include "Rectangle.h"

Rectangle::Rectangle(float x, float y, float width, float height, std::shared_ptr<Shader> shader, const char* texturePath) {
  this->width = width;
  this->height = height;
  this->set_origin(glm::vec3{ x, y, 0.f });

  GLfloat verticies[] = {
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
  const float x0 = this->buffer.data_ptr[0];
  const float y0 = this->buffer.data_ptr[1];
  const float z0 = this->buffer.data_ptr[2];

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