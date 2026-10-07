#pragma once
#include <vector>
#include <memory>
#include <string>
#include <utility>
#include "Component.h"

class Scene;

class GameObject {
  public:
    std::string name;
    std::string tag = "Untagged";
    bool isAlive = true;
    bool isActive = true;
    Scene* scene = nullptr;

    GameObject(const std::string& name = "GameObject") : name(name) {}
    ~GameObject() = default;

    void Destroy() {
      if (!isAlive) return;
      isAlive = false;
      SetActive(false);
    }

    void SetActive(bool active) {
      if (isActive == active) return;
      isActive = active;

      for (size_t i = 0; i < components.size(); ++i) {
        if (isActive) {
          components[i]->OnEnable();
        } else {
          components[i]->OnDisable();
        }
      }
    }

    void Start() {
      for (size_t i = 0; i < components.size(); ++i) {
        if (!components[i]->hasStarted) {
          components[i]->hasStarted = true;
          components[i]->Start();
        }
      }
    }

    void Update(float deltaTime) {
      for (auto& component : components) {
        component->Update(deltaTime);
      }
    }

    void Render() {
      for (auto& component : components) {
        component->Render();
      }
    }

    void OnCollisionEnter(GameObject* other) {
      for (size_t i = 0; i < components.size(); ++i) {
        components[i]->OnCollisionEnter(other);
      }
    }

    void OnCollisionStay(GameObject* other) {
      for (size_t i = 0; i < components.size(); ++i) {
        components[i]->OnCollisionStay(other);
      }
    }

    void OnCollisionExit(GameObject* other) {
      for (size_t i = 0; i < components.size(); ++i) {
        components[i]->OnCollisionExit(other);
      }
    }

    template <typename T, typename... TArgs>
    T* AddComponent(TArgs&&... args) {
      auto newComponent = std::make_unique<T>(std::forward<TArgs>(args)...);
      newComponent->gameObject = this;
      newComponent->typeID = GetComponentTypeID<T>();

      T* rawPtr = newComponent.get();
      components.push_back(std::move(newComponent));
       
      rawPtr->Init();

      if (isActive) {
        rawPtr->OnEnable();
      }

      return rawPtr;
    }

    template <typename T>
    T* GetComponent() const {
      ComponentTypeID targetID = GetComponentTypeID<T>();
      
      for (const auto& component : components) {
        if (component->typeID == targetID) {
          return static_cast<T*>(component.get());
        }
      }
      return nullptr;
    }

    template <typename T>
    bool HasComponent() const {
      return GetComponent<T>() != nullptr;
    }

  private:
    std::vector<std::unique_ptr<Component>> components;
};