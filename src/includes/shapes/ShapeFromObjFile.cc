#include "ShapeFromObjFile.h"
#include "BufferData.h"
#include "common.h"

#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>


ShapeFromObjFile::ShapeFromObjFile(std::string filepath) {
  const size_t vertex_stride = 9;

  std::ifstream fd{filepath};
  if (!fd.is_open()) {
    spdlog::error("Failed to open object file {}", filepath);
    return;
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

    // Load verticies.
    if (!buffer.empty() && buffer[0] == 'v') {
      ss >> _c >> x >> y >> z;
      vertex_vec.push_back({ x, y, z });
    }

    // Load faces(faces).
    else if (!buffer.empty() && buffer[0] == 'f') {
      std::stringstream ss{buffer};
      ss >> _c >> x >> y >> z;
      faces.push_back({ x, y, z });
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

  this->width = max_x - min_x;
  this->height = max_y - min_y;
  this->set_origin(glm::vec3{ this->width / 2.f, this->height / 2.f, 0.f });

  spdlog::info("min -> x={} y={}", min_x, min_y);
  spdlog::info("max -> x={} y={}", max_x, max_y);
  spdlog::info("dimensions -> width={} height={}", this->width, this->height);

  const size_t VERTEX_ARRAY_SIZE = (
    // vec3 vertex points
    vertex_vec.size() * 3 +

    // vec4 RGBA values
    vertex_vec.size() * 4 +

    // vec2 texture coordinates
    vertex_vec.size() * 2
  );
  GLdouble verticies[VERTEX_ARRAY_SIZE];

  const size_t INDICIES_ARRAY_SIZE = faces.size() * 3;
  GLuint indicies[INDICIES_ARRAY_SIZE];

  // Offset by the vertex stride.
  size_t vec_index = 0;
  for (size_t v_index = 0; v_index < VERTEX_ARRAY_SIZE; v_index += vertex_stride) {
    // Coordinates.
    verticies[v_index]      = vertex_vec[vec_index].x;
    verticies[v_index + 1]  = vertex_vec[vec_index].y;
    verticies[v_index + 2]  = 0.f;

    // RGBA
    verticies[v_index + 3]  = 0.f;
    verticies[v_index + 4]  = 0.f;
    verticies[v_index + 5]  = 0.f;
    verticies[v_index + 6]  = 0.f;

    // Map the texture to each vertex point around the circle.
    verticies[v_index + 7]  = normalizeFloat( vertex_vec[vec_index].x, min_x, max_x, 0.f, 1.f );
    verticies[v_index + 8]  = normalizeFloat( vertex_vec[vec_index].y, min_y, max_y, 0.f, 1.f );
    vec_index++;
  }

  // TODO: split up verticies based on indicies. or somehow split up indicies to multiple faces.
  // Populate faces.
  vec_index = 0;
  for (size_t i = 0; i <= INDICIES_ARRAY_SIZE - 3; i+=3) {
    indicies[i]     = faces[vec_index].x;
    indicies[i + 1] = faces[vec_index].y;
    indicies[i + 2] = faces[vec_index].z;
    vec_index++;
  }

  // TEST:
  std::shared_ptr<Shader> shader = std::make_shared<Shader>();
  shader->compile("./shaders/shader.vert", "./shaders/shader.frag");

  spdlog::info("Size of verticies in bytes -> {}", sizeof(verticies));
  spdlog::info("Size of indicies in bytes -> {}", sizeof(indicies));
  this->buffer = CreateBuffer::dynamic_float(
    verticies,
    sizeof(verticies),
    indicies,
    sizeof(indicies),
    shader
  );
  this->buffer.texture = new Texture("./textures/615-checkerboard.png");
}

ShapeFromObjFile::~ShapeFromObjFile() {}

glm::vec3 ShapeFromObjFile::get_center_vec() {
  return this->origin;
}
