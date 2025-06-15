#pragma once

// Library
#include "Buffer.hpp"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/ext/vector_float4.hpp"

// Core libraries
#include <GL/glew.h>

#include <cstring>
#include <memory>

class BufferData {
  private:
    BufferData(
      std::shared_ptr<Buffer<GLdouble>> vertex_buffer, const size_t vertex_stride,
      std::shared_ptr<IndexBuffer> index_buffer, GLuint &vao_index, std::shared_ptr<Shader> shader
    );

  public:
    std::shared_ptr<Buffer<GLdouble>> vertex_buffer_ptr;
    GLsizei vertex_stride;

    std::shared_ptr<IndexBuffer> index_buffer_ptr;

    const GLuint vao_index;  // Vertex Array Object
    Texture *texture;        // Texture Object

    glm::vec4 solid_color;   // rgba solid color as fallback from texture

    // Shared pointer to a shader since there could be multiple references.
    // Bound shader program on this buffer.
    std::shared_ptr<Shader> shader;

  public:
    BufferData() = delete;
    ~BufferData();

    /* Updates the buffer data store with the current instance's data */
    void update();

    // Static methods.

    /* Creates a float Buffer with a given buffer usage (https://docs.gl/gl4/glBufferData) */
    static std::shared_ptr<BufferData> create_float_buffer(
      std::shared_ptr<Buffer<GLdouble>> &vertex_data, std::shared_ptr<IndexBuffer> &index_data,
      std::shared_ptr<Shader> shader, GLenum buffer_usage
    );

    /* Creates a Static Draw float Buffer */
    static std::shared_ptr<BufferData> create_static_float(
      std::shared_ptr<Buffer<GLdouble>> &vertex_data, std::shared_ptr<IndexBuffer> &index_data,
      std::shared_ptr<Shader> shader
    );

    /* Creates a Stream Draw float Buffer */
    static std::shared_ptr<BufferData> create_stream_float(
      std::shared_ptr<Buffer<GLdouble>> &vertex_data, std::shared_ptr<IndexBuffer> &index_data,
      std::shared_ptr<Shader> shader
    );

    /* Creates a Dynamnic Draw float Buffer */
    static std::shared_ptr<BufferData> create_dynamic_float(
      std::shared_ptr<Buffer<GLdouble>> &vertex_data, std::shared_ptr<IndexBuffer> &index_data,
      std::shared_ptr<Shader> shader
    );
};
