#pragma once
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Transform.h"
#include "Core/Vector3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraProjection {
  Orthographic, // 2D
  Perspective   // 3D
};

class Camera : public Component {
  public:
    CameraProjection projection = CameraProjection::Orthographic;
    float zoom = 1.0f;
    float fov = 45.0f; // Para 3D

    int screenWidth = 800;
    int screenHeight = 600;

    Transform* transform = nullptr;

    Camera() = default;
    Camera(int width, int height) : screenWidth(width), screenHeight(height) {}

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
    }

    // Matriz de Proyección (la lente de la cámara)
    glm::mat4 GetProjectionMatrix() const {
      if (projection == CameraProjection::Orthographic) {
        // En 2D: (0,0) arriba a la izquierda, igual que SDL
        return glm::ortho(0.0f, static_cast<float>(screenWidth), 
          static_cast<float>(screenHeight), 0.0f, 
          -1000.0f, 1000.0f);
      } else {
        // En 3D: Perspectiva con profundidad humana y punto de fuga
        float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
        return glm::perspective(glm::radians(fov), aspect, 0.1f, 1000.0f);
      }
    }

    // Matriz de Vista (la posición en el mundo de la cámara)
    glm::mat4 GetViewMatrix() const {
      glm::mat4 view = glm::mat4(1.0f);
      if (transform != nullptr) {
        // La cámara invierte el movimiento: si la cámara va a la derecha, el mundo parece ir a la izquierda
        view = glm::translate(view, -glm::vec3(transform->position.x, transform->position.y, transform->position.z));
      }
      return view;
    }
};