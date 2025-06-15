#include "Polygon.h"
#include <memory>
#include "Texture.h"
#include "utils/common.h"

Polygon::Polygon( const double width, const double height, const glm::vec3 &origin, std::shared_ptr<BufferData> buffer )
  : Shape( buffer ), width( width ), height( height ) {
  this->origin = origin;
}

Polygon::~Polygon() {}

/**
 * Initializes a 2d polygon instance.
 *
 * @param vertices Array of (x,y) points.
 * @param shader Pointer to the shader used
 * @param texturePath Optional path to the shape texture
 */
std::shared_ptr<Polygon> Polygon::create(const std::vector<glm::vec2> &vertices, std::shared_ptr<Shader> shader, const char* texturePath) {
  // Find the min/max x and y points.
  double _min_x = vertices[0].x, min_y = vertices[0].y;
  double _max_x = vertices[0].x, max_y = vertices[0].y;
  for (const glm::vec2 &point: vertices) {
    if (point.x > _max_x) _max_x = point.x;
    else if (point.x < _min_x) _min_x = point.x;

    if (point.y > max_y) max_y = point.y;
    else if (point.y < min_y) min_y = point.y;
  }
  const double min_x = _min_x;
  const double max_x = _max_x;

  // Calculate buffer sizes.
  const size_t num_points = vertices.size();
  const size_t VERTEX_BUFFER_SIZE = (
    // vec3 vertex points
    num_points * 3 +

    // vec4 RGBA values
    num_points * 4 +

    // vec2 texture coordinates
    num_points * 2
  );
  const size_t INDEX_BUFFER_SIZE = num_points * 3;

  // Allocate vertex & index buffer for the GPU.
  std::shared_ptr<Buffer<GLdouble>> vertex_buffer( Buffer<GLdouble>::create( VERTEX_BUFFER_SIZE ) );
  GLuint *indicies = new GLuint[INDEX_BUFFER_SIZE];

  // Generate them verticies.
  size_t vertex_buffer_index = 0;
  for (const glm::vec2 &point: vertices) {
    // Populate the vertex buffer.
    // Coordinates.
    vertex_buffer->get_buffer()[vertex_buffer_index]      = point.x;
    vertex_buffer->get_buffer()[vertex_buffer_index + 1]  = point.y;
    vertex_buffer->get_buffer()[vertex_buffer_index + 2]  = 0.0;

    // RGBA
    vertex_buffer->get_buffer()[vertex_buffer_index + 3]  = 0.0;
    vertex_buffer->get_buffer()[vertex_buffer_index + 4]  = 0.0;
    vertex_buffer->get_buffer()[vertex_buffer_index + 5]  = 0.0;
    vertex_buffer->get_buffer()[vertex_buffer_index + 6]  = 0.0;

    // Map the texture to each vertex point.
    //   - https://learnopengl.com/Getting-started/Textures
    vertex_buffer->get_buffer()[vertex_buffer_index + 7]  = normalizeFloat( point.x, min_x, max_x, 0.f, 1.f );
    vertex_buffer->get_buffer()[vertex_buffer_index + 8]  = normalizeFloat( point.y, min_y, max_y, 0.f, 1.f );
    vertex_buffer_index += 9;
  }

  // Not the greatest, but it can generate an index array given an arbitrary set of points.
  size_t j = 1;
  for (size_t i = 0; i < INDEX_BUFFER_SIZE; i+=3) {
    indicies[i]     = 0;
    indicies[i + 1] = j;

    size_t i3 = (j + 1) % num_points;
    if (i3 == 0) i3++;

        indicies[i + 2] = i3;

    j = (j + 1) % num_points;
    if (j == 0) j++;
  }

  // Generate buffer.
  std::shared_ptr<IndexBuffer> index_data( IndexBuffer::create() );
  index_data->buffers.emplace_back( Buffer<GLuint>::create( indicies, INDEX_BUFFER_SIZE ) );
  delete[] indicies;

  std::shared_ptr<BufferData> buffer = BufferData::create_dynamic_float(
    vertex_buffer,
    index_data,
    shader
  );
  std::shared_ptr<Polygon> obj(new Polygon(
    max_x - min_x,
    max_y - min_y,
    glm::vec3(findMidpoint(vertices), 0.0),
    buffer
  ));
  if ( texturePath ) { obj->buffer->texture = new Texture( texturePath ); }

  return obj;
}

glm::vec3 Polygon::get_center_vec() {
  return glm::vec3(
    this->width,
    this->height,
    0.0
  );
};
