#include "Circle.h"
#include "utils/common.h"
#include <glm/glm.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#define MIN_QUALITY_LIMIT 200

Circle::Circle( const double r, std::shared_ptr<BufferData> buffer ) : Shape( buffer ), radius( r ) {}

/**
 * Initializes a circle instance.
 *
 * @param x Position on x-axis
 * @param y Position on y-axis
 * @param r Circle's radius
 * @param shader Pointer to the shader used
 * @param texturePath Optional path to the shape texture
 * @param quality Number of points to generate for the circle
*/
std::shared_ptr<Circle> Circle::create(const double x, const double y, const double r, const std::shared_ptr<Shader> shader, const char* texturePath, const size_t quality) {
  // Ensure we hit the minimum quality requirement.
  assert( quality >= MIN_QUALITY_LIMIT );
  constexpr double HALF_PI               = glm::half_pi<double>();
  constexpr double PI                    = glm::pi<double>();
  constexpr double THREE_OVER_TWO_PI     = glm::three_over_two_pi<double>();
  constexpr double TWO_PI                = glm::two_pi<double>();

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
  const size_t INDICIES_ARRAY_SIZE = _quality * 3;

  // Calculate the min and max positional values. This is used to map textures onto the
  // verticies.
  const double max_x = x + ( r * glm::cos(0) );
  const double min_x = x + ( r * glm::cos(PI) );
  const double max_y = y + ( r * glm::sin(HALF_PI) );
  const double min_y = y + ( r * glm::sin(THREE_OVER_TWO_PI) );

  // Generate circle verticies.
  // PI Chart -> https://tinyurl.com/5mm8nm9c
  size_t v_index = 0;

  // Allocate vertex & index buffer. Index buffer has 3 data points cause
  // triangles.
  std::shared_ptr<Buffer<GLdouble>> verticies( Buffer<GLdouble>::create( VERTEX_ARRAY_SIZE ) );
  GLuint *indicies = new GLuint[INDICIES_ARRAY_SIZE];

  // Add initial data point in the center of the circle.
  verticies->get_buffer()[v_index]      = x;
  verticies->get_buffer()[v_index + 1]  = y;
  verticies->get_buffer()[v_index + 2]  = 0.f;
  verticies->get_buffer()[v_index + 3]  = 0.f;
  verticies->get_buffer()[v_index + 4]  = 0.f;
  verticies->get_buffer()[v_index + 5]  = 0.f;
  verticies->get_buffer()[v_index + 6]  = 0.f;

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
  verticies->get_buffer()[v_index + 7]  = normalizeFloat( x, min_x, max_x, 0.f, 1.f );
  verticies->get_buffer()[v_index + 8]  = normalizeFloat( y, min_y, max_y, 0.f, 1.f );
  v_index += 9;

  // Now generate circle data points.
  double v = 0.f;
  double circle_percision = TWO_PI / quality;

  // Offset by the vertex stride.
  for (; v_index < VERTEX_ARRAY_SIZE; v_index += 9) {
    v += circle_percision;

    const double _x = ( glm::cos(v) * r ) + x;
    const double _y = ( glm::sin(v) * r ) + y;

    // Coordinates.
    verticies->get_buffer()[v_index]      = _x;
    verticies->get_buffer()[v_index + 1]  = _y;
    verticies->get_buffer()[v_index + 2]  = 0.f;

    // RGBA
    verticies->get_buffer()[v_index + 3]  = 0.f;
    verticies->get_buffer()[v_index + 4]  = 0.f;
    verticies->get_buffer()[v_index + 5]  = 0.f;
    verticies->get_buffer()[v_index + 6]  = 0.f;

    // Map the texture to each vertex point around the circle.
    verticies->get_buffer()[v_index + 7]  = normalizeFloat( _x, min_x, max_x, 0.f, 1.f );
    verticies->get_buffer()[v_index + 8]  = normalizeFloat( _y, min_y, max_y, 0.f, 1.f );
  }

  // Generate the indicies to map the center of the circle to 2 points across the
  // circle's arc.
  size_t value = 1;
  for (size_t i = 0; i < INDICIES_ARRAY_SIZE; i++) indicies[i] = 0;
  for (size_t i = 0; i <= INDICIES_ARRAY_SIZE - 3; i+=3) {
    indicies[i    ] = 0;
    indicies[i + 1] = value++;

    // Ensure we loop around
    if (value >= _quality) value = 1;
    indicies[i + 2] = value;
  }

  // Generate buffer.
  std::shared_ptr<IndexBuffer> index_data( IndexBuffer::create() );
  index_data->buffers.emplace_back( Buffer<GLuint>::create( indicies, INDICIES_ARRAY_SIZE ) );
  delete[] indicies;

  std::shared_ptr<BufferData> buffer = BufferData::create_dynamic_float( verticies, index_data, shader );

  if (texturePath) {
    buffer->texture = new Texture(texturePath);
  }

  // // Origin is the center of the circle.
  std::shared_ptr<Circle> obj( new Circle( r, buffer ) );
  obj->set_origin( glm::vec3{ x, y, 0.f } );
  return obj;
}

Circle::~Circle() {}

glm::vec3 Circle::get_center_vec() {
  return this->origin;
}
