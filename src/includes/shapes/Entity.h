#pragma once

#include <glm/vec2.hpp>
#include <vector>

// Project Libraries
#include "Texture.h"
#include "BufferData.h"

class Entity {
  public:
    std::vector<BufferData> buffers;

  public:
    Entity();

    /**
     * Translates the entity by the given vector. This is done by mutating the
     * internal buffer and updating the buffer storage.
     *
     * @param v 2D Vector to traslate entity by.
    */
    virtual void translate(const glm::vec2&);

    /** Updates entity state */
    void update();
};
