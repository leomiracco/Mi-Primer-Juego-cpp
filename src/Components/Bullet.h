#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Transform.h"

class Bullet : public Component {
  public:
    float speed = 700.0f; // Velocidad rápida del proyectil
    Transform* transform = nullptr;

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
    }

    void Update(float deltaTime) override {
      if (transform != nullptr) {
        // 1. La bala vuela hacia la derecha a toda velocidad
        transform->position.x += speed * deltaTime;

        // 2. Si sale del ancho de la pantalla (800 px), se destruye para no consumir RAM
        if (transform->position.x > 850.0f) {
          gameObject->Destroy();
        }
      }
    }
};