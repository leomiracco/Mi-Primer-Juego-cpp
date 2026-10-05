#pragma once
#include <vector>
#include <memory>
#include <string>
#include <SDL.h>
#include "Component.h"

// 👈 1. Declaración adelantada de Scene (evita dependencias circulares)
class Scene;

class GameObject {
  public:
    std::string name;
    bool isAlive = true; // 👈 1. Bandera de vida
    Scene* scene = nullptr; // 👈 2. Puntero a la escena a la que pertenece este objeto
    std::string tag = "Untagged"; // 👈 Identificador rápido estilo Unity

    GameObject(const std::string& name = "GameObject") : name(name) {}
    ~GameObject() = default;

    // 👈 2. Método para marcar para morir
    void Destroy() {
      isAlive = false;
    }

    // Propaga el choque a todos los componentes que tenga adentro
    void OnCollisionEnter(GameObject* other) {
      for (auto& component : components) {
        component->OnCollisionEnter(other);
      }
    }

    // 1. Ejecuta el Update de todos los componentes que tenga adentro
    void Update(float deltaTime) {
      for (auto& component : components) {
        component->Update(deltaTime);
      }
    }

    // 2. Ejecuta el Render de todos los componentes que sepan dibujarse
    void Render() {
      for (auto& component : components) {
        component->Render();
      }
    }

    // 3. Agregar un componente con cualquier cantidad de parámetros (o ninguno)
    template <typename T, typename... TArgs>
    T* AddComponent(TArgs&&... args) {
      // std::forward pasa exactamente los mismos parámetros al constructor de T
      auto newComponent = std::make_unique<T>(std::forward<TArgs>(args)...);
      newComponent->gameObject = this;
    
      T* rawPtr = newComponent.get();
      components.push_back(std::move(newComponent));
   
      rawPtr->Init();
      return rawPtr;
    }

    // 4. Buscar un componente (como Unity: player.GetComponent<Transform>())
    template <typename T>
    T* GetComponent() {
      for (auto& component : components) {
        // dynamic_cast comprueba si este componente es del tipo 'T' buscado
        T* target = dynamic_cast<T*>(component.get());
        if (target != nullptr) {
          return target;
        }
      }
      return nullptr; // No tiene ese componente. Puntero nulo.
    }

  private:
    // El GameObject es el dueño absoluto de la memoria de sus componentes
    std::vector<std::unique_ptr<Component>> components;
};