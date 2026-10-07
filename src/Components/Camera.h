#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Core/Scene.h"
#include "Core/Screen.h"
#include "Graphics/Shader.h"
#include "Components/Transform.h"

enum class CameraProjection {
    Orthographic, // 2D: Sin punto de fuga
    Perspective   // 3D: Con perspectiva humana
};

class Camera : public Component {
  public:
    CameraProjection projection = CameraProjection::Orthographic;
    bool depthTest = false;
    float zoom = 1.0f;
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

    // Color de fondo azul oscuro por defecto
    glm::vec4 backgroundColor = glm::vec4(30.0f / 255.0f, 35.0f / 255.0f, 45.0f / 255.0f, 1.0f);
    Transform* transform = nullptr;

    Camera() = default;
    Camera(CameraProjection proj)
      : projection(proj), depthTest(proj == CameraProjection::Perspective) {}

    // 👉 SERVICIO UNIVERSAL DE CÁMARA (Estándar Unity Camera.main)
    static Camera* GetMain() {
      return mainCamera;
    }

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
      mainCamera = this;

      if (gameObject && gameObject->scene) {
        gameObject->scene->mainCamera = this;
      }
      if (projection == CameraProjection::Perspective) {
        depthTest = true;
      }
    }

    ~Camera() override {
      if (mainCamera == this) {
        mainCamera = nullptr;
      }
      if (gameObject && gameObject->scene && gameObject->scene->mainCamera == this) {
        gameObject->scene->mainCamera = nullptr;
      }
    }

    // Limpia el lienzo de la GPU según el modo 2D o 3D
    void Clear() {
      if (depthTest) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
      } else {
        glDisable(GL_DEPTH_TEST);
      }

      glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
      glClear(GL_COLOR_BUFFER_BIT | (depthTest ? GL_DEPTH_BUFFER_BIT : 0));

      // Calculamos las matrices una sola vez al inicio del fotograma
      cachedProjection = CalculateProjectionMatrix();
      cachedView = CalculateViewMatrix();
    }

    // Inyecta las matrices calculadas en el Shader activo
    void ApplyToShader(const std::shared_ptr<Shader>& shader) const {
      if (shader) {
        shader->SetMat4("projection", cachedProjection);
        shader->SetMat4("view", cachedView);
      }
    }

    glm::mat4 GetProjectionMatrix() const { return cachedProjection; }
    glm::mat4 GetViewMatrix() const { return cachedView; }

    // Conversión de coordenadas de Pantalla a Mundo
    Vector3 ScreenToWorldPoint(const Vector3& screenPos) const {
      glm::vec4 viewport(0.0f, 0.0f, Screen::GetWidthF(), Screen::GetHeightF());
      glm::vec3 winCoord(screenPos.x, Screen::GetHeightF() - screenPos.y, screenPos.z);
      glm::vec3 world = glm::unProject(winCoord, cachedView, cachedProjection, viewport);
      return Vector3(world.x, world.y, world.z);
    }

    // Conversión de coordenadas de Mundo a Pantalla
    Vector3 WorldToScreenPoint(const Vector3& worldPos) const {
      glm::vec4 viewport(0.0f, 0.0f, Screen::GetWidthF(), Screen::GetHeightF());
      glm::vec3 objCoord(worldPos.x, worldPos.y, worldPos.z);
      glm::vec3 win = glm::project(objCoord, cachedView, cachedProjection, viewport);
      return Vector3(win.x, Screen::GetHeightF() - win.y, win.z);
    }

  private:
    static inline Camera* mainCamera = nullptr;
    glm::mat4 cachedProjection = glm::mat4(1.0f);
    glm::mat4 cachedView = glm::mat4(1.0f);

    // 👉 PROYECCIÓN CANÓNICA SIN DESPLAZAMIENTOS DUPLICADOS
    glm::mat4 CalculateProjectionMatrix() const {
      float currentZoom = (zoom > 0.001f) ? zoom : 1.0f;

      if (projection == CameraProjection::Orthographic) {
        float w = Screen::GetWidthF() / currentZoom;
        float h = Screen::GetHeightF() / currentZoom;
        return glm::ortho(0.0f, w, h, 0.0f, -farPlane, farPlane);
      } else {
        float currentFov = glm::clamp(fov / currentZoom, 1.0f, 179.0f);
        return glm::perspective(glm::radians(currentFov), Screen::GetAspectRatio(), nearPlane, farPlane);
      }
    }

    // 👉 VISTA CANÓNICA
    glm::mat4 CalculateViewMatrix() const {
      glm::mat4 view = glm::mat4(1.0f);
      if (transform != nullptr) {
        Vector3 pos = transform->GetWorldPosition();
        Vector3 rot = transform->rotation;

        view = glm::rotate(view, glm::radians(-rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(-rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(-rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // Traslación estándar del observador
        view = glm::translate(view, -glm::vec3(pos.x, pos.y, pos.z));
      }
      return view;
    }
};