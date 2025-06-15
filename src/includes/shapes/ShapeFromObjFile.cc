#include "ShapeFromObjFile.h"
#include "BufferData.h"
#include "common.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

ShapeFromObjFile::~ShapeFromObjFile() {}

ShapeFromObjFile::ShapeFromObjFile(
  const double width, const double height, const glm::vec3 &origin, std::shared_ptr<BufferData> buffer
)
  : Shape( buffer ), width( width ), height( height ) {
  this->set_origin( origin );
};

/**
 * Initializes an instance from a file.
 *
 * See: https://en.wikipedia.org/wiki/Wavefront_.obj_file
 *
 * @param filepath Path to a .obj file.
 */
std::shared_ptr<ShapeFromObjFile> ShapeFromObjFile::create( const std::string &filepath ) {
  const size_t vertex_stride = 9;

  std::ifstream fd{filepath};
  if (!fd.is_open()) {
    spdlog::error("Failed to open object file {}", filepath.c_str());
    return nullptr;
  }

  std::vector<glm::vec3> vertex_vec;
  std::vector<glm::vec3> faces;

  // Array of slices for each face.
  std::vector<std::pair<size_t, size_t>> face_slices;

  std::string buffer;
  while (std::getline(fd, buffer)) {
    std::stringstream ss{buffer};
    double x, y, z;
    char _c;

    // TODO: read .mtl file for color.

    // Load verticie->s
    if (!buffer.empty() && buffer[0] == 'v') {
      ss >> _c >> x >> y >> z;
      vertex_vec.push_back({ x, y, z });
    }

    // Load faces(faces).
    // NOTE: these faces are 1-based index!
    // See: https://en.wikipedia.org/wiki/Wavefront_.obj_file#Face_elements
    else if (!buffer.empty() && buffer[0] == 'f') {
      std::stringstream ss{buffer};
      ss >> _c >> x >> y >> z;
      faces.push_back({ x - 1, y - 1, z - 1 });
    }

    // Comment block.
    else if (!buffer.empty() && buffer[0] == '#') {
      std::stringstream ss{buffer};
      uint64_t num_of;
      std::string data_type;
      ss >> _c >> num_of >> data_type;

      // Create a slice of faces.
      if (data_type == "faces" && !faces.empty()) {
        size_t last_idx = face_slices.empty() ? 0 : (face_slices.end() - 1)->second;
        face_slices.push_back({ last_idx, faces.size() - 1 });
      }
    }
  }

  fd.close();

  spdlog::info("Loaded {} verticies and {} faces ({} slices)", vertex_vec.size(), faces.size(), face_slices.size());
  for (const auto &slice : face_slices) {
    spdlog::info("face slice {} -> {}", slice.first, slice.second);
  }

  // Find min and max for texture mapping normilization.
  double max_x = vertex_vec[0].x;
  double min_x = vertex_vec[0].x;
  double max_y = vertex_vec[0].y;
  double min_y = vertex_vec[0].y;
  for (const glm::vec3 &v : vertex_vec) {
    if (v.x > max_x) max_x = v.x;
    if (v.x < min_x) min_x = v.x;
    if (v.y > max_y) max_y = v.y;
    if (v.y < min_x) min_y = v.y;
  }
  const double width = max_x - min_x;
  const double height = max_y - min_y;

  const size_t VERTEX_ARRAY_SIZE = (
    // vec3 vertex points
    vertex_vec.size() * 3 +

    // vec4 RGBA values
    vertex_vec.size() * 4 +

    // vec2 texture coordinates
    vertex_vec.size() * 2
  );
  std::shared_ptr<Buffer<GLdouble>> verticies( Buffer<GLdouble>::create( VERTEX_ARRAY_SIZE ) );

  // Offset by the vertex stride.
  size_t vec_index = 0;
  for (size_t v_index = 0; v_index < VERTEX_ARRAY_SIZE; v_index += vertex_stride) {
    // Coordinates.
    verticies->get_buffer()[v_index]      = vertex_vec[vec_index].x;
    verticies->get_buffer()[v_index + 1]  = vertex_vec[vec_index].y;
    verticies->get_buffer()[v_index + 2]  = 0.f;

    // RGBA
    verticies->get_buffer()[v_index + 3]  = 0.f;
    verticies->get_buffer()[v_index + 4]  = 0.f;
    verticies->get_buffer()[v_index + 5]  = 0.f;
    verticies->get_buffer()[v_index + 6]  = 0.f;

    // Map the texture to each vertex point around the circle.
    verticies->get_buffer()[v_index + 7]  = normalizeFloat( vertex_vec[vec_index].x, min_x, max_x, 0.f, 1.f );
    verticies->get_buffer()[v_index + 8]  = normalizeFloat( vertex_vec[vec_index].y, min_y, max_y, 0.f, 1.f );
    vec_index++;
  }

  // Generate buffer.
  std::shared_ptr<IndexBuffer> index_data( IndexBuffer::create() );

  // TODO: split up verticies based on indicies. or somehow split up indicies to multiple faces.
  // Populate faces.
  for ( const auto& face_slice : face_slices ) {
    const size_t face_slice_size = face_slice.second - face_slice.first;
    const size_t indicies_size   = face_slice_size * 3;
    GLuint *indicies             = new GLuint[indicies_size];

    for ( size_t face_idx = face_slice.first; face_idx < face_slice.second; face_idx++ ) {
      // map face index into indicies array.
      size_t i = (face_idx - face_slice.first) * 3;
      indicies[i]   = faces[face_idx].x;
      indicies[++i] = faces[face_idx].y;
      indicies[++i] = faces[face_idx].z;
    }

    index_data->buffers.emplace_back( Buffer<GLuint>::create( indicies, indicies_size ) );
    delete[] indicies;
  }


  // TODO: should we delete this test? i don't remember what i was thinking here.
  // but obv make sure shader is taken care of through caller.
  // TEST:
  std::shared_ptr<Shader> shader = std::make_shared<Shader>();
  shader->compile("./shaders/shader.vert", "./shaders/shader.frag");

  std::shared_ptr<BufferData> buffer_data = BufferData::create_dynamic_float( verticies, index_data, shader );
  buffer_data->texture = new Texture("./textures/615-checkerboard.png");

  std::shared_ptr<ShapeFromObjFile> obj(
    new ShapeFromObjFile( width, height, { width / 2.f, height / 2.f, 0.f }, buffer_data )
  );
  return obj;
}

glm::vec3 ShapeFromObjFile::get_center_vec() {
  return this->origin;
}
