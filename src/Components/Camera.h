#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Transform.h"
#include "Core/Vector3.h"

enum class CameraProjection {
    Orthographic, // 2D (sin punto de fuga)
    Perspective   // 3D (objetos lejanos se ven pequeños)
};

class Camera : public Component {
  public:
    CameraProjection projection = CameraProjection::Orthographic;
    float zoom = 1.0f; // 1.0f = tamaño normal, 2.0f = zoom in, 0.5f = zoom out

    // Ancho y alto de la resolución de la pantalla visible
    int screenWidth = 800;
    int screenHeight = 600;

    Transform* transform = nullptr;

    Camera() = default;
    Camera(int width, int height) : screenWidth(width), screenHeight(height) {}

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
    }

    // Convierte una posición del Mundo a la posición en la Pantalla del usuario
    // Centra la cámara en el objetivo (estilo Unity/Cine)
    Vector3 WorldToScreenPoint(const Vector3& worldPos) const {
      Vector3 camPos = (transform != nullptr) ? transform->position : Vector3(0, 0, 0);

      // El centro de la pantalla es el punto de mira de la cámara
      float screenX = (worldPos.x - camPos.x) * zoom + (screenWidth * 0.5f);
      float screenY = (worldPos.y - camPos.y) * zoom + (screenHeight * 0.5f);

      return Vector3(screenX, screenY, worldPos.z);
    }
};