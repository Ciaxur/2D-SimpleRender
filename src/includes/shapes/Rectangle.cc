#include "Rectangle.h"

Rectangle::Rectangle(float x, float y, float width, float height, std::shared_ptr<Shader> shader, const char* texturePath) {
  GLfloat verticies[] = {
    // VERTEX<vec3>		                        RGBA<vec4>					      // Texture Coordinates<vec2>
    x,              y,             0.0f,      1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    x + width,      y,             0.0f,      0.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f,
    x,              y + height,    0.0f,      0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    x + width,      y + height,    0.0f,      0.0f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f
  };

  GLuint indicies[] = {
    0, 3, 1,
    0, 2, 3
  };

  BufferData buffer = CreateBuffer::dynamic_float(verticies, sizeof(verticies), indicies, sizeof(indicies), shader);
  buffer.texture = new Texture(texturePath);
  this->buffers.push_back(buffer);
};
