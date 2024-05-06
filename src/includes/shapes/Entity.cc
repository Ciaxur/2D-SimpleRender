#include "Entity.h"

Entity::Entity(): buffers() {}

void Entity::translate(const glm::vec2 &t) {

  for (BufferData &buffer: this->buffers) {
    double buff_array_len = buffer.data_size_bytes / sizeof(GL_FLOAT);
    for (GLsizei i = 0; i < (GLsizei)buff_array_len; i += buffer.stride) {
      buffer.data_ptr[i]      += t.x;
      buffer.data_ptr[i + 1]  += t.y;
    }
  }

}

void Entity::update() {
  for (BufferData &buffer: this->buffers)
    buffer.update();
}