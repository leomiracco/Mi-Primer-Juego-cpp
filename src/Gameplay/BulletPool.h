#pragma once
#include <vector>
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Components/Transform.h"
#include "Gameplay/Prefabs.h"

class BulletPool : public Component {
  public:
    // Pre-calienta N balas en memoria al inicio
    void InitPool(size_t poolSize = 30) {
      if (!gameObject || !gameObject->scene) return;

      pool.reserve(poolSize);
      for (size_t i = 0; i < poolSize; ++i) {
        GameObject* bullet = Prefabs::CreateBullet(gameObject->scene);
        pool.push_back(bullet);
      }
    }

    // 👉 DISPARO A COSTO CERO: Despierta una bala reciclable y la ubica
    GameObject* Spawn(float x, float y) {
      for (auto* bullet : pool) {
        if (bullet != nullptr && !bullet->isActive) {
          // Reutilizamos la bala
          auto* t = bullet->GetComponent<Transform>();
          if (t) {
            t->position.x = x;
            t->position.y = y;
          }

          bullet->SetActive(true); // Se despierta y vuela
          return bullet;
        }
      }

      // Si se agotaron las 30 balas simultáneas en pantalla, agrandamos el pool
      GameObject* newBullet = Prefabs::CreateBullet(gameObject->scene);
      auto* t = newBullet->GetComponent<Transform>();
      if (t) {
        t->position.x = x;
        t->position.y = y;
      }
      newBullet->SetActive(true);
      pool.push_back(newBullet);
      return newBullet;
    }

  private:
    std::vector<GameObject*> pool;
};