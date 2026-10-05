#pragma once
#include <vector>
#include <memory>
#include <string>
#include <SDL.h>
#include "ECS/GameObject.h"
#include "Components/BoxCollider.h"

// Declaración adelantada de Camera para evitar inclusiones circulares
class Camera;

class Scene {
  public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void Init() {}

    virtual void Update(float deltaTime) {
      // 1. Actualizar todas las entidades vivas
      for (size_t i = 0; i < gameObjects.size(); ++i) {
        if (gameObjects[i]->isAlive) {
          gameObjects[i]->Update(deltaTime);
        }
      }

      // 👉 2. PASO DE FÍSICA: Chequear colisiones entre colliders activos
      CheckCollisions();

      // 3. Limpieza diferida universal: remueve los objetos que murieron en este frame
      for (auto it = gameObjects.begin(); it != gameObjects.end(); ) {
        if (!(*it)->isAlive) {
          it = gameObjects.erase(it);
        } else {
          ++it;
        }
      }
    }

    virtual void Render() {
      for (auto& obj : gameObjects) {
        if (obj->isAlive) {
          obj->Render();
        }
      }
    }

    GameObject* CreateGameObject(const std::string& name = "GameObject") {
      auto newObj = std::make_unique<GameObject>(name);
      newObj->scene = this;
      GameObject* rawPtr = newObj.get();
      gameObjects.push_back(std::move(newObj));
      return rawPtr;
    }

    // 👉 Acceso a la cámara principal de la escena (como Camera.main en Unity)
    Camera* mainCamera = nullptr;

  protected:
    std::vector<std::unique_ptr<GameObject>> gameObjects;

  private:
    void CheckCollisions() {
      // Comparamos pares únicos para no duplicar trabajo: (i, j) donde j = i + 1
      for (size_t i = 0; i < gameObjects.size(); ++i) {
        if (!gameObjects[i]->isAlive) continue;
        auto* colA = gameObjects[i]->GetComponent<BoxCollider>();
        if (!colA) continue;

        for (size_t j = i + 1; j < gameObjects.size(); ++j) {
          if (!gameObjects[j]->isAlive) continue;
          auto* colB = gameObjects[j]->GetComponent<BoxCollider>();
          if (!colB) continue;

          // Si colisionan, disparamos el evento en AMBOS objetos
          if (colA->CheckCollision(*colB)) {
            gameObjects[i]->OnCollisionEnter(gameObjects[j].get());
            gameObjects[j]->OnCollisionEnter(gameObjects[i].get());
          }
        }
      }
    }
};