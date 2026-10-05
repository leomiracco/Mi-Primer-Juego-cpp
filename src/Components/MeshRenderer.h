#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Core/Scene.h"
#include "Core/Shader.h"
#include "Core/Mesh.h"
#include "Core/Texture2D.h" // 👈 Incluimos Texture2D
#include "Components/Camera.h"
#include "Transform.h"

class MeshRenderer : public Component {
  public:
    std::shared_ptr<Mesh> mesh = nullptr;
    std::shared_ptr<Texture2D> texture = nullptr; // 👈 Textura opcional
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    Transform* transform = nullptr;

    MeshRenderer() = default;

    // Constructor 1: Solo malla y color (usa textura blanca por defecto)
    MeshRenderer(std::shared_ptr<Mesh> mesh, const glm::vec4& color = glm::vec4(1.0f))
      : mesh(mesh), color(color), texture(Texture2D::GetWhiteTexture()) {}

    // Constructor 2: Malla + Textura + Color de tinte
    MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f))
      : mesh(mesh), texture(texture), color(color) {}

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
      InitDefaultShader();
      if (!texture) {
        texture = Texture2D::GetWhiteTexture();
      }
    }

    void Render() override {
      if (!mesh || !transform || !shader || !gameObject->scene || !gameObject->scene->mainCamera) return;

      shader->Use();

      // 1. Activar y enlazar textura en el slot 0
      if (texture) {
        texture->Bind(0);
        shader->SetInt("imageTexture", 0);
      }

      // 2. Matrices de Cámara
      Camera* cam = gameObject->scene->mainCamera;
      shader->SetMat4("projection", cam->GetProjectionMatrix());
      shader->SetMat4("view", cam->GetViewMatrix());

      // 3. Matriz Modelo
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(transform->position.x, transform->position.y, transform->position.z));
      model = glm::rotate(model, glm::radians(transform->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::rotate(model, glm::radians(transform->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::rotate(model, glm::radians(transform->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
      model = glm::scale(model, glm::vec3(transform->width * transform->scale.x, 
          transform->height * transform->scale.y, 
          transform->scale.z));

      shader->SetMat4("model", model);
      shader->SetVec4("objectColor", color.r, color.g, color.b, color.a);

      // 4. Dibujar
      mesh->Draw();
    }

  private:
    static inline Shader* shader = nullptr;

    void InitDefaultShader() {
      if (shader != nullptr) return;

      shader = new Shader();

      const char* vShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTexCoords;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        out vec2 TexCoords;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            TexCoords = aTexCoords;
        }
      )";

      const char* fShader = R"(
        #version 330 core
        in vec2 TexCoords;
        out vec4 FragColor;

        uniform vec4 objectColor;
        uniform sampler2D imageTexture;

        void main() {
            // El color final es la textura multiplicada por el color de tinte
            FragColor = texture(imageTexture, TexCoords) * objectColor;
        }
      )";

      shader->LoadFromSource(vShader, fShader);
    }
};