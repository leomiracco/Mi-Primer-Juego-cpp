#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Transform.h"
#include "Core/Input.h"

class PlayerController : public Component {
  public:
  float speed = 250.0f;
  Transform* transform = nullptr; // Puntero cacheado

  // 1. Al nacer, busca su Transform una sola vez
  void Init() override {
    transform = gameObject->GetComponent<Transform>();
  }

  // 2. En cada fotograma, el Hero mueve su propia posición
  void Update(float deltaTime) override {
    if (transform != nullptr) {
      // 3. Leemos la intención del jugador (WASD o Flechas)
      float moveX = Input::GetAxisHorizontal();
      float moveY = Input::GetAxisVertical();

      // 4. Movemos al personaje de forma suave con deltaTime
      transform->x += moveX * speed * deltaTime;
      transform->y += moveY * speed * deltaTime;
    }
  }
};