#pragma once
#include <cmath>
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Core/Scene.h"
#include "Core/Vector3.h"
#include "Transform.h"

struct BoundsF {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
};

class BoxCollider : public Component {
  public:
    Transform* transform = nullptr;
    
    Vector3 offset = Vector3::Zero();
    Vector3 size   = Vector3::One();

    uint32_t layer = 1;
    uint32_t collisionMask = 0xFFFFFFFF;

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
    }

    void OnEnable() override {
      if (gameObject && gameObject->scene) {
        gameObject->scene->RegisterCollider(this);
      }
    }

    void OnDisable() override {
      if (gameObject && gameObject->scene) {
        gameObject->scene->UnregisterCollider(this);
      }
    }

    ~BoxCollider() override {
      OnDisable();
    }

    bool CanCollideWith(const BoxCollider& other) const {
      return (collisionMask & other.layer) && (other.collisionMask & layer);
    }

    BoundsF GetBounds() const {
      if (transform != nullptr) {
        Vector3 worldPos = transform->GetWorldPosition();
        Vector3 worldScale = transform->GetWorldScale();

        float w = std::abs(worldScale.x * size.x);
        float h = std::abs(worldScale.y * size.y);

        float x = worldPos.x + offset.x;
        float y = worldPos.y + offset.y;

        if (transform->scale.x < 0.0f) {
          x -= w;
        }
        if (transform->scale.y < 0.0f) {
          y -= h;
        }

        return { x, y, w, h };
      }
      return { 0.0f, 0.0f, 0.0f, 0.0f };
    }

    bool CheckCollision(const BoxCollider& other) const {
      BoundsF a = GetBounds();
      BoundsF b = other.GetBounds();

      return (a.x < b.x + b.width  &&
              a.x + a.width > b.x  &&
              a.y < b.y + b.height &&
              a.y + a.height > b.y);
    }
};