#include "Circle.h"
#include "utils/common.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#define MIN_QUALITY_LIMIT 200

Circle::Circle(float x, float y, float r, std::shared_ptr<Shader> shader, const char* texturePath, size_t quality = 200) {
  // Ensure we hit the minimum quality requirement.
  assert( quality >= MIN_QUALITY_LIMIT );
  constexpr float HALF_PI               = glm::half_pi<float>();
  constexpr float PI                    = glm::pi<float>();
  constexpr float THREE_OVER_TWO_PI     = glm::three_over_two_pi<float>();
  constexpr float TWO_PI                = glm::two_pi<float>();

  // Include an additional point in the middle, which is used to link indicies.
  const size_t _quality = quality + 1;
  const size_t VERTEX_ARRAY_SIZE = (
    // vec3 vertex points
    _quality * 3 +

    // vec4 RGBA values
    _quality * 4 +

    // vec2 texture coordinates
    _quality * 2
  );
  const size_t INDICIDES_ARRAY_SIZE = _quality * 3;

  // Calculate the min and max positional values. This is used to map textures onto the
  // verticies.
  const float max_x = x + ( r * glm::cos(0) );
  const float min_x = x + ( r * glm::cos(PI) );
  const float max_y = y + ( r * glm::sin(HALF_PI) );
  const float min_y = y + ( r * glm::sin(THREE_OVER_TWO_PI) );

  // Origin is the center of the circle.
  this->radius = r;
  this->set_origin(glm::vec3{ x, y, 0.f });

  // Generate circle verticies.
  // PI Chart -> https://tinyurl.com/5mm8nm9c
  size_t v_index = 0;

  // Allocate vertex & index buffer. Index buffer has 3 data points cause
  // triangles.
  GLfloat *verticies  = new GLfloat[VERTEX_ARRAY_SIZE];
  GLuint  *indicies   = new GLuint[INDICIDES_ARRAY_SIZE];

  // Add initial data point in the center of the circle.
  verticies[v_index]      = x;
  verticies[v_index + 1]  = y;
  verticies[v_index + 2]  = 0.f;
  verticies[v_index + 3]  = 0.f;
  verticies[v_index + 4]  = 0.f;
  verticies[v_index + 5]  = 0.f;
  verticies[v_index + 6]  = 0.f;

  /*
    Map the texture to each vertex.
      - https://learnopengl.com/Getting-started/Textures

    Draw starts at bottom left, where is texture land, is:
      -----------------------------------
      | (0,1)                     (1,1) |
      |                                 |
      |                                 |
      |                                 |
      |                                 |
      |                                 |
      |                                 |
      |                                 |
      | (0,0)                     (1,0) |
      -----------------------------------
  */
  verticies[v_index + 7]  = normalizeFloat( x, min_x, max_x, 0.f, 1.f );
  verticies[v_index + 8]  = normalizeFloat( y, min_y, max_y, 0.f, 1.f );
  v_index += 9;

  // Now generate circle data points.
  float v = 0.f;
  float circle_percision = TWO_PI / quality;

  // Offset by the vertex stride.
  for (; v_index < VERTEX_ARRAY_SIZE; v_index += 9) {
    v += circle_percision;

    const float _x = ( glm::cos(v) * r ) + x;
    const float _y = ( glm::sin(v) * r ) + y;

    // Coordinates.
    verticies[v_index]      = _x;
    verticies[v_index + 1]  = _y;
    verticies[v_index + 2]  = 0.f;

    // RGBA
    verticies[v_index + 3]  = 0.f;
    verticies[v_index + 4]  = 0.f;
    verticies[v_index + 5]  = 0.f;
    verticies[v_index + 6]  = 0.f;

    // Map the texture to each vertex point around the circle.
    verticies[v_index + 7]  = normalizeFloat( _x, min_x, max_x, 0.f, 1.f );
    verticies[v_index + 8]  = normalizeFloat( _y, min_y, max_y, 0.f, 1.f );
  }

  // Generate the indicies to map the center of the circle to 2 points across the
  // circle's arc.
  size_t value = 1;
  for (size_t i = 0; i < INDICIDES_ARRAY_SIZE; i++) indicies[i] = 0;
  for (size_t i = 0; i <= INDICIDES_ARRAY_SIZE - 3; i+=3) {
    indicies[i    ] = 0;
    indicies[i + 1] = value++;

    // Ensure we loop around
    if (value >= _quality) value = 1;
    indicies[i + 2] = value;
  }

  this->buffer = CreateBuffer::dynamic_float(
    verticies,
    VERTEX_ARRAY_SIZE * sizeof(GLfloat),
    indicies,
    INDICIDES_ARRAY_SIZE * sizeof(GLuint),
    shader
  );
  if (texturePath)
    this->buffer.texture = new Texture(texturePath);

  // Free up heap.
  delete[] verticies;
  delete[] indicies;
}

Circle::~Circle() {}

glm::vec3 Circle::get_center_vec() {
  return this->origin;
}