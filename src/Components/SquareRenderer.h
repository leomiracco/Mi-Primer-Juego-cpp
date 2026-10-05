#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "Core/Scene.h"
#include "Core/Shader.h"
#include "Components/Camera.h"
#include "Transform.h"

class SquareRenderer : public Component {
  public:
    float r = 50.0f / 255.0f;
    float g = 205.0f / 255.0f;
    float b = 50.0f / 255.0f;
    float a = 1.0f;

    Transform* transform = nullptr;

    SquareRenderer() = default;
    SquareRenderer(float r, float g, float b, float a = 1.0f)
      : r(r), g(g), b(b), a(a) {}

    ~SquareRenderer() {
      if (VAO) glDeleteVertexArrays(1, &VAO);
      if (VBO) glDeleteBuffers(1, &VBO);
      if (EBO) glDeleteBuffers(1, &EBO);
    }

    void Init() override {
      transform = gameObject->GetComponent<Transform>();
      InitShader();
      InitMesh();
    }

    void Render() override {
      // Si no hay transform o no hay cámara activa en la escena, no hay nada que proyectar
      if (!transform || !shader || !gameObject->scene || !gameObject->scene->mainCamera) return;

      shader->Use();

      // 1. Tomamos las matrices de la Cámara activa (Arquitectura limpia)
      Camera* cam = gameObject->scene->mainCamera;
      glm::mat4 projection = cam->GetProjectionMatrix();
      glm::mat4 view = cam->GetViewMatrix();

      // 2. Construir la Matriz Modelo del GameObject
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(transform->position.x, transform->position.y, transform->position.z));
      model = glm::rotate(model, glm::radians(transform->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
      model = glm::scale(model, glm::vec3(transform->width * transform->scale.x, 
                                          transform->height * transform->scale.y, 1.0f));

      // 3. Enviar a la GPU
      shader->SetMat4("projection", projection);
      shader->SetMat4("view", view);
      shader->SetMat4("model", model);
      shader->SetVec4("objectColor", r, g, b, a);

      // 4. Dibujar
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }

  private:
    GLuint VAO = 0, VBO = 0, EBO = 0;
    static inline Shader* shader = nullptr;

    void InitShader() {
      if (shader != nullptr) return;
      shader = new Shader();

      const char* vShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
      )";

      const char* fShader = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 objectColor;
        void main() {
            FragColor = objectColor;
        }
      )";

      shader->LoadFromSource(vShader, fShader);
    }

    void InitMesh() {
      float vertices[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
      };

      unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
      };

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glBindVertexArray(0);
    }
};