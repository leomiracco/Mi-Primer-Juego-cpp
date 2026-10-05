#pragma once
#include <vector>
#include <memory>
#include <string>
#include <SDL.h>
#include "ECS/GameObject.h"

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

    // 2. Limpieza diferida universal con iteradores
      for (auto it = gameObjects.begin(); it != gameObjects.end(); ) {
        if (!(*it)->isAlive) {
          it = gameObjects.erase(it);
        } else {
          ++it;
        }
      }
    }

    virtual void Render(SDL_Renderer* renderer) {
      for (auto& obj : gameObjects) {
        if (obj->isAlive) {
          obj->Render(renderer);
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
};