#pragma once
#include <SDL.h>

// Declaración adelantada para evitar dependencias circulares
class GameObject;

class Component {
  public:
    virtual ~Component() = default;

    // Ciclo de vida que cualquier componente puede sobreescribir
    virtual void Init() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {}

    // Puntero de referencia hacia el GameObject dueño de este componente
    GameObject* gameObject = nullptr;
};