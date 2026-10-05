#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Transform.h"

// Estructura de límites en coma flotante (precisión sub-pixel)
struct BoundsF {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
};

class BoxCollider : public Component {
  public:
    Transform* transform = nullptr;
    
    // Offset opcional por si queremos ajustar el hitbox más chico que el sprite
    float offsetX = 0.0f;
    float offsetY = 0.0f;

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
    }

    BoundsF GetBounds() const {
      if (transform != nullptr) {
        return {
          transform->position.x + offsetX,
          transform->position.y + offsetY,
          transform->width * transform->scale.x,
          transform->height * transform->scale.y
        };
      }
      return { 0.0f, 0.0f, 0.0f, 0.0f };
    }

    // Algoritmo matemático AABB puro en float
    bool CheckCollision(const BoxCollider& other) const {
      BoundsF a = GetBounds();
      BoundsF b = other.GetBounds();

      return (a.x < b.x + b.width  &&
              a.x + a.width > b.x  &&
              a.y < b.y + b.height &&
              a.y + a.height > b.y);
    }
};