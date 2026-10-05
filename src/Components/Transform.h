#pragma once
#include "ECS/Component.h"
#include "Core/Vector3.h"

class Transform : public Component {
  public:
    // Vectores tridimensionales puros
    Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f); // En 2D solo rota en Z
    Vector3 scale    = Vector3(1.0f, 1.0f, 1.0f);

    // Dimensiones base en píxeles (ancho y alto)
    int width = 32;
    int height = 32;

    Transform() = default;

    Transform(float x, float y, int width = 32, int height = 32, float z = 0.0f)
      : position(x, y, z), width(width), height(height) {}

    // Método estilo Unity: gameObject->transform->Translate(desplazamiento)
    void Translate(const Vector3& delta) {
      position += delta;
    }

    void Translate(float deltaX, float deltaY, float deltaZ = 0.0f) {
      position.x += deltaX;
      position.y += deltaY;
      position.z += deltaZ;
    }
};