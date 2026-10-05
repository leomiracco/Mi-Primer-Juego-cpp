#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Core/Scene.h"
#include "Components/Camera.h"
#include "Transform.h"
#include <SDL.h>

class SquareRenderer : public Component {
  public:
    Uint8 r = 50, g = 205, b = 50, a = 255;
    Transform* transform = nullptr;

    SquareRenderer() = default;
    SquareRenderer(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
      : r(r), g(g), b(b), a(a) {}

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
    }

    void Render(SDL_Renderer* renderer) override {      
      if (transform == nullptr) return;

      int drawX = static_cast<int>(transform->position.x);
      int drawY = static_cast<int>(transform->position.y);
      int drawW = static_cast<int>(transform->width * transform->scale.x);
      int drawH = static_cast<int>(transform->height * transform->scale.y);

      // 👉 Si la escena tiene una Cámara activa, proyectamos las coordenadas de Mundo a Pantalla
      if (gameObject->scene != nullptr && gameObject->scene->mainCamera != nullptr) {
        Camera* cam = gameObject->scene->mainCamera;
        Vector3 screenPos = cam->WorldToScreenPoint(transform->position);

        drawX = static_cast<int>(screenPos.x);
        drawY = static_cast<int>(screenPos.y);
        drawW = static_cast<int>(drawW * cam->zoom);
        drawH = static_cast<int>(drawH * cam->zoom);
      }

      SDL_Rect rect = { drawX, drawY, drawW, drawH };

      SDL_SetRenderDrawColor(renderer, r, g, b, a);
      SDL_RenderFillRect(renderer, &rect);
    }
};