#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Transform.h"
#include "Core/Input.h"
#include "Core/Prefabs.h"

class PlayerController : public Component {
  public:
  float speed = 250.0f;
  float fireCooldown = 0.0f;
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
      transform->position.x += moveX * speed * deltaTime;
      transform->position.y += moveY * speed * deltaTime;

      // 2. Temporizador de cadencia
      if (fireCooldown > 0.0f) {
        fireCooldown -= deltaTime;
      }

      // 3. Disparo limpio: sin singletons ni variables globales
      if (Input::GetButtonFire() && fireCooldown <= 0.0f) {
        if (gameObject->scene != nullptr) {
          float spawnX = transform->position.x + transform->width;
          float spawnY = transform->position.y + (transform->height / 2.0f) - 6.0f;

          // 👈 Nace en la escena del jugador sin tocar el motor
          Prefabs::SpawnBullet(gameObject->scene, spawnX, spawnY);
          fireCooldown = 0.25f;
        }
      }
    }
  }
};