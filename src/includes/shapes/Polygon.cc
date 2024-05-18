#include "Polygon.h"
#include "utils/common.h"

Polygon::Polygon(const std::vector<glm::vec2> &vertices, std::shared_ptr<Shader> shader, const char* texturePath) {
  // Find the min/max x and y points.
  double min_x = vertices[0].x, min_y = vertices[0].y;
  double max_x = vertices[0].x, max_y = vertices[0].y;
  for (const glm::vec2 &point: vertices) {
    if (point.x > max_x) max_x = point.x;
    else if (point.x < min_x) min_x = point.x;

    if (point.y > max_y) max_y = point.y;
    else if (point.y < min_y) min_y = point.y;
  }

  this->width   = max_x - min_x;
  this->height  = max_y - min_y;
  this->origin  = glm::vec3(findMidpoint(vertices), 0.0);

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
  GLdouble  *vertex_buffer   = new GLdouble[VERTEX_BUFFER_SIZE];
  GLuint    *index_buffer    = new GLuint[INDEX_BUFFER_SIZE];


  // Generate them verticies.
  size_t vertex_buffer_index = 0;
  for (const glm::vec2 &point: vertices) {
    // Populate the vertex buffer.
    // Coordinates.
    vertex_buffer[vertex_buffer_index]      = point.x;
    vertex_buffer[vertex_buffer_index + 1]  = point.y;
    vertex_buffer[vertex_buffer_index + 2]  = 0.0;

    // RGBA
    vertex_buffer[vertex_buffer_index + 3]  = 0.0;
    vertex_buffer[vertex_buffer_index + 4]  = 0.0;
    vertex_buffer[vertex_buffer_index + 5]  = 0.0;
    vertex_buffer[vertex_buffer_index + 6]  = 0.0;

    // Map the texture to each vertex point.
    //   - https://learnopengl.com/Getting-started/Textures
    vertex_buffer[vertex_buffer_index + 7]  = normalizeFloat( point.x, min_x, max_x, 0.f, 1.f );
    vertex_buffer[vertex_buffer_index + 8]  = normalizeFloat( point.y, min_y, max_y, 0.f, 1.f );
    vertex_buffer_index += 9;
  }

  // Not the greatest, but it can generate an index array given an arbitrary set of points.
  size_t j = 1;
  for (size_t i = 0; i < INDEX_BUFFER_SIZE; i+=3) {
    index_buffer[i]     = 0;
    index_buffer[i + 1] = j;

    size_t i3 = (j + 1) % num_points;
    if (i3 == 0) i3++;

    index_buffer[i + 2] = i3;

    j = (j + 1) % num_points;
    if (j == 0) j++;
  }

  this->buffer = CreateBuffer::dynamic_float(
    vertex_buffer,
    VERTEX_BUFFER_SIZE * sizeof(GLdouble),
    index_buffer,
    INDEX_BUFFER_SIZE * sizeof(GLuint),
    shader
  );
  if (texturePath)
    this->buffer.texture = new Texture(texturePath);


  // Free up heap.
  delete[] vertex_buffer;
  delete[] index_buffer;
}

Polygon::~Polygon() {}

glm::vec3 Polygon::get_center_vec() {
  return glm::vec3(
    this->width,
    this->height,
    0.0
  );
}