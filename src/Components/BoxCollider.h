#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Transform.h"
#include <SDL.h>

class BoxCollider : public Component {
  public:
    Transform* transform = nullptr;

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
    }

    // Devuelve el rectángulo de impacto físico en base al Transform
    SDL_Rect GetBounds() const {
      if (transform != nullptr) {
        return {
          static_cast<int>(transform->position.x),
          static_cast<int>(transform->position.y),
          transform->width,
          transform->height
        };
      }
      return { 0, 0, 0, 0 };
    }

    // Comprueba si esta caja física choca contra otra caja
    bool CheckCollision(const BoxCollider& other) const {
      SDL_Rect a = GetBounds();
      SDL_Rect b = other.GetBounds();

      // 👈 Función nativa de SDL que calcula intersección matemática (AABB)
      return SDL_HasIntersection(&a, &b) == SDL_TRUE;
    }
};