#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Core/Scene.h"
#include "Graphics/Mesh.h"     // 👈 Graphics
#include "Graphics/Material.h" // 👈 Graphics
#include "Components/Camera.h"
#include "Transform.h"

class MeshRenderer : public Component {
  public:
    std::shared_ptr<Mesh> mesh = nullptr;
    std::shared_ptr<Material> material = nullptr;
    Transform* transform = nullptr;

    // 👉 CAPA DE ORDEN DE DIBUJADO (Estilo Unity sortingOrder)
    // Menor número = se dibuja antes (fondo). Mayor número = se dibuja encima (frente).
    int sortingOrder = 0;

    MeshRenderer() = default;

    MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, int sortingOrder = 0)
      : mesh(mesh), material(material), sortingOrder(sortingOrder) {}

    MeshRenderer(std::shared_ptr<Mesh> mesh, const glm::vec4& color = glm::vec4(1.0f), int sortingOrder = 0)
      : mesh(mesh), material(std::make_shared<Material>(color)), sortingOrder(sortingOrder) {}

    MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f), int sortingOrder = 0)
      : mesh(mesh), material(std::make_shared<Material>(color, texture)), sortingOrder(sortingOrder) {}

    // 👉 AUTO-REGISTRO EN LA ESCENA
    void Init() override {
      transform = gameObject->GetComponent<Transform>();
      if (!material) {
        material = std::make_shared<Material>();
      }
    }

    // 👉 SE REGISTRA AL ACTIVARSE
    void OnEnable() override {
      if (gameObject && gameObject->scene) {
        gameObject->scene->RegisterRenderer(this);
      }
    }

    // 👉 SE DESREGISTRA AL APAGARSE (Cero carga en la escena mientras duerme en el Pool)
    void OnDisable() override {
      if (gameObject && gameObject->scene) {
        gameObject->scene->UnregisterRenderer(this);
      }
    }

    // 👉 AUTO-BAJA AL DESTRUIRSE (RAII)
    ~MeshRenderer() override {
      OnDisable();
    }

    void Render() override {
      if (!mesh || !material || !transform) return;

      material->Apply();

      Camera* cam = Camera::GetMain();
      if (cam != nullptr) {
        cam->ApplyToShader(material->shader);
      } else {
        // 👉 SALVAVIDAS PROFESIONAL: Si la cámara aún no está lista, proyecta en 2D igual
        glm::mat4 defaultProj = glm::ortho(0.0f, Screen::GetWidthF(), Screen::GetHeightF(), 0.0f, -1000.0f, 1000.0f);
        material->shader->SetMat4("projection", defaultProj);
        material->shader->SetMat4("view", glm::mat4(1.0f));
      }

      material->shader->SetMat4("model", transform->GetWorldMatrix());
      mesh->Draw();
    }
};