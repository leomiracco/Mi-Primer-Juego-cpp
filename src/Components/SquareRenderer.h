#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Transform.h"
#include <SDL.h>

class SquareRenderer : public Component {
  public:
    // Color RGBA (por defecto el verde que usamos antes)
    Uint8 r = 50, g = 205, b = 50, a = 255;
    Transform* transform = nullptr; // Puntero cacheado

    SquareRenderer() = default;
    SquareRenderer(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
      : r(r), g(g), b(b), a(a) {}

    // Se ejecuta una sola vez al nacer: buscamos la posición
    void Init() override {
      transform = gameObject->GetComponent<Transform>();
    }

    // Dibuja el cuadrado en las coordenadas que dicte el Transform
    void Render(SDL_Renderer* renderer) override {
      if (transform != nullptr) {
        SDL_Rect rect = {
          static_cast<int>(transform->x),
          static_cast<int>(transform->y),
          transform->width,
          transform->height
        };
        // Pintamos el escenario en este mismo instante
        // con esta nueva información...

        // 👉 Cambia el pincel: ahora lo moja en pintura verde.
        SDL_SetRenderDrawColor(renderer, r, g, b, a);

        //👉 Pinta el cuadrado verde directamente sobre el lienzo oculto.
        // En la memoria de la tarjeta gráfica, esos píxeles específicos
        // (del 50 al 150) dejan de ser azules y pasan a ser verdes en
        // ese preciso nanosegundo.
        SDL_RenderFillRect(renderer, &rect);
      }
    }
};