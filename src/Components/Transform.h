#pragma once
#include "ECS/Component.h"

class Transform : public Component {
  public:
    Transform() = default;
    Transform(float x, float y, int width = 32, int height = 32)
      : x(x), y(y), width(width), height(height) {}

    float x = 0.0f;
    float y = 0.0f;
    int width = 32;
    int height = 32;

};