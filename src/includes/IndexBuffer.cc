#include "IndexBuffer.h"
#include <memory>

IndexBuffer::IndexBuffer() noexcept : buffers() {}
IndexBuffer::~IndexBuffer() noexcept {}

std::shared_ptr<IndexBuffer> IndexBuffer::create() noexcept {
  return std::shared_ptr<IndexBuffer>( new IndexBuffer() );
}

