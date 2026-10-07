#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Core/Screen.h"
#include "Components/Transform.h"
#include "Gameplay/Health.h"

class Bullet : public Component {
  public:
    float speed = 700.0f;
    int damage = 1;
    Transform* transform = nullptr;

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
    }

    void Update(float deltaTime) override {
      if (transform != nullptr) {
        transform->position.x += speed * deltaTime;

        // 👉 AL SALIR DE PANTALLA: Se apaga para volver al Pool (Cero delete)
        if (transform->position.x > Screen::GetWidthF() + 50.0f) {
          gameObject->SetActive(false);
        }
      }
    }

    void OnCollisionEnter(GameObject* other) override {
      if (other != nullptr) {
        auto* health = other->GetComponent<Health>();
        if (health != nullptr) {
          health->TakeDamage(damage);
        }
      }

      // 👉 AL IMPACTAR: Se apaga para volver al Pool (Cero delete)
      gameObject->SetActive(false);
    }
};