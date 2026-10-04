#pragma once
#include <vector>
#include <memory>
#include <string>
#include <SDL.h>
#include "Component.h"

class GameObject {
  public:
    std::string name;

    GameObject(const std::string& name = "GameObject") : name(name) {}
    ~GameObject() = default;

    // 1. Ejecuta el Update de todos los componentes que tenga adentro
    void Update(float deltaTime) {
      for (auto& component : components) {
        component->Update(deltaTime);
      }
    }

    // 2. Ejecuta el Render de todos los componentes que sepan dibujarse
    void Render(SDL_Renderer* renderer) {
      for (auto& component : components) {
        component->Render(renderer);
      }
    }

    // 3. Agregar un componente (como Unity: player.AddComponent<Transform>())
    template <typename T>
    T* AddComponent() {
      auto newComponent = std::make_unique<T>();
      newComponent->gameObject = this; // 👈 Le decimos al componente: "yo soy tu padre"
        
      T* rawPtr = newComponent.get(); // 👈 Guardamos el puntero directo para devolverlo
      components.push_back(std::move(newComponent)); // 👈 Acá std::move newComponent queda vacío nullptr
       
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
      return nullptr; // No tiene ese componente
    }

  private:
    // El GameObject es el dueño absoluto de la memoria de sus componentes
    std::vector<std::unique_ptr<Component>> components;
};