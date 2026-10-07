#pragma once
#include <vector>
#include <memory>
#include <string>
#include "ECS/GameObject.h"
#include "Core/CollisionSystem.h" // 👈 Delega las físicas a CollisionSystem

class Camera;
class BoxCollider;
class MeshRenderer;

class Scene {
  public:
    Scene() = default;
    virtual ~Scene();

    virtual void Init() {}
    virtual void Update(float deltaTime);
    virtual void Render();

    GameObject* CreateGameObject(const std::string& name = "GameObject");

    // Delegación limpia de físicas al CollisionSystem
    void RegisterCollider(BoxCollider* collider) { collisionSystem.Register(collider); }
    void UnregisterCollider(BoxCollider* collider) { collisionSystem.Unregister(collider); }

    // Registro de renderers
    void RegisterRenderer(MeshRenderer* renderer);
    void UnregisterRenderer(MeshRenderer* renderer);

    // Consultas de escena
    GameObject* FindGameObject(const std::string& name) const;
    GameObject* FindGameObjectWithTag(const std::string& tag) const;
    std::vector<GameObject*> FindGameObjectsWithTag(const std::string& tag) const;

    template <typename T>
    T* FindObjectOfType() const {
      for (const auto& obj : gameObjects) {
        if (obj->isAlive) {
          T* comp = obj->GetComponent<T>();
          if (comp != nullptr) return comp;
        }
      }
      return nullptr;
    }

    Camera* mainCamera = nullptr;

  protected:
    CollisionSystem collisionSystem;            // 👈 El motor de físicas vive aquí encapsulado
    std::vector<MeshRenderer*> activeRenderers;
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    std::vector<std::unique_ptr<GameObject>> pendingObjects;
    bool isDestroying = false;

  private:
    void IntegratePendingObjects();
};