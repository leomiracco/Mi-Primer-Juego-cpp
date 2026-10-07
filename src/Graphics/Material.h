#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "Graphics/Shader.h"
#include "Graphics/Texture2D.h"

class Material {
  public:
    std::shared_ptr<Shader> shader = nullptr;
    std::shared_ptr<Texture2D> texture = nullptr;
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    Material() 
      : shader(GetDefaultShader()), 
        texture(Texture2D::GetWhiteTexture()), 
        color(glm::vec4(1.0f)) {}

    Material(const glm::vec4& color, std::shared_ptr<Texture2D> texture = nullptr, std::shared_ptr<Shader> customShader = nullptr)
      : color(color),
        texture(texture ? texture : Texture2D::GetWhiteTexture()),
        shader(customShader ? customShader : GetDefaultShader()) {}

    // 👉 SRP PURO: El Material SOLO aplica propiedades ópticas de superficie
    // Cero matrices de cámara, cero matrices de posición.
    void Apply() const {
      if (!shader) return;

      shader->Use();

      if (texture) {
        texture->Bind(0);
        shader->SetInt("imageTexture", 0);
      }

      shader->SetVec4("objectColor", color.r, color.g, color.b, color.a);
    }

    // 👉 Variable estática accesible para liberación limpia
    static std::shared_ptr<Shader> GetDefaultShader() {
      if (!defaultShader) {
        defaultShader = std::make_shared<Shader>();

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
              FragColor = texture(imageTexture, TexCoords) * objectColor;
          }
        )";

        defaultShader->LoadFromSource(vShader, fShader);
      }
      return defaultShader;
    }

    // 👉 Libera el shader por defecto mientras OpenGL aún está vivo
    static void UnloadDefaultShader() {
      defaultShader.reset();
    }
  private:
    static inline std::shared_ptr<Shader> defaultShader = nullptr; // 👈 Pasa a ser miembro estático  
};