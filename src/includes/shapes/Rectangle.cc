#include "Rectangle.h"
#include <memory>
#include "Texture.h"

Rectangle::Rectangle(
  const double width, const double height, const glm::vec3& origin, std::shared_ptr<BufferData> buffer
)
  : Shape( buffer ), width( width ), height( height ) {
  this->origin = origin;
}

Rectangle::~Rectangle() {}

/**
 * Initializes a rectangle instance.
 *
 * @param x Position on x-axis
 * @param y Position on y-axis
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param shader Pointer to the shader used
 * @param texturePath Optional path to the shape texture
 */
std::shared_ptr<Rectangle> Rectangle::create(
  double x, double y, double width, double height, std::shared_ptr<Shader> shader, const char* texturePath
) {
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

  // Generate buffer.
  std::shared_ptr<Buffer<GLdouble>> vertex_data( Buffer<GLdouble>::create( verticies, std::size( verticies ) ) );
  std::shared_ptr<IndexBuffer> index_data( IndexBuffer::create() );
  index_data->buffers.emplace_back( Buffer<GLuint>::create( indicies, std::size( indicies ) ) );

  std::shared_ptr<BufferData> buffer = BufferData::create_dynamic_float( vertex_data, index_data, shader );
  std::shared_ptr<Rectangle> obj( new Rectangle( width, height, glm::vec3{ x, y, 0.f }, buffer ) );
  if ( texturePath ) { obj->buffer->texture = new Texture( texturePath ); }

  return obj;
};

glm::vec3 Rectangle::get_center_vec() {
  const double x0 = this->buffer->vertex_buffer_ptr->at(0);
  const double y0 = this->buffer->vertex_buffer_ptr->at(1);
  const double z0 = this->buffer->vertex_buffer_ptr->at(2);

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
