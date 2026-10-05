#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

class Shader {
  public:
    GLuint ID = 0;

    Shader() = default;

    // Compila directamente código de Shader GLSL
    bool LoadFromSource(const char* vertexSource, const char* fragmentSource) {
      GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
      GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

      if (!vertex || !fragment) return false;

      ID = glCreateProgram();
      glAttachShader(ID, vertex);
      glAttachShader(ID, fragment);
      glLinkProgram(ID);

      // Comprobar errores de enlace
      GLint success;
      glGetProgramiv(ID, GL_LINK_STATUS, &success);
      if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "Error enlazando Shader Program:\n" << infoLog << std::endl;
        return false;
      }

      glDeleteShader(vertex);
      glDeleteShader(fragment);
      return true;
    }

    void Use() const {
      glUseProgram(ID);
    }

    // Métodos para enviar datos a la GPU (Uniforms)
    void SetMat4(const std::string& name, const glm::mat4& mat) const {
      glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void SetVec4(const std::string& name, float x, float y, float z, float w) const {
      glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }

  private:
    GLuint CompileShader(GLenum type, const char* source) {
      GLuint shader = glCreateShader(type);
      glShaderSource(shader, 1, &source, nullptr);
      glCompileShader(shader);

      GLint success;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error compilando Shader (" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "):\n"
                  << infoLog << std::endl;
        return 0;
      }
      return shader;
    }
};