# pragma once
#include <GL/glew.h>

#include <memory>
#include <vector>

#include "Buffer.hpp"

struct IndexBuffer {
  private:
    IndexBuffer() noexcept;
    bool _is_buffer_bound   = false;

  public:
    ~IndexBuffer() noexcept;

    static std::shared_ptr<IndexBuffer> create() noexcept;

    // Multiple index buffers. 2D array.
    std::vector<std::shared_ptr<Buffer<GLuint>>> buffers;
};
