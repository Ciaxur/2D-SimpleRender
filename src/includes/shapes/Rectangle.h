#pragma once

#include "Entity.h"

class Rectangle: protected Entity {
  public:
    Rectangle(float, float, float, float, std::shared_ptr<Shader>, const char*);
};