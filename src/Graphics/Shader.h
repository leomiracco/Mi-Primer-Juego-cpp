#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>
#include <iostream>

class Shader {
  public:
    GLuint ID = 0;

    Shader() = default;

    ~Shader() {
      if (ID != 0) {
        glDeleteProgram(ID);
      }
    }

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    bool LoadFromSource(const char* vertexSource, const char* fragmentSource) {
      if (ID != 0) {
        glDeleteProgram(ID);
        ID = 0;
        uniformLocationCache.clear(); // Limpia la caché si el shader se recarga
      }

      GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
      GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

      if (!vertex || !fragment) {
        if (vertex) glDeleteShader(vertex);
        if (fragment) glDeleteShader(fragment);
        return false;
      }

      ID = glCreateProgram();
      glAttachShader(ID, vertex);
      glAttachShader(ID, fragment);
      glLinkProgram(ID);

      GLint success;
      glGetProgramiv(ID, GL_LINK_STATUS, &success);
      if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "Error enlazando Shader Program:\n" << infoLog << std::endl;
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(ID);
        ID = 0;
        return false;
      }

      glDeleteShader(vertex);
      glDeleteShader(fragment);
      return true;
    }

    void Use() const {
      glUseProgram(ID);
    }

    // 👉 MÉTODOS DE ALTO RENDIMIENTO CON CACHÉ DE MEMORIA
    void SetMat4(const std::string& name, const glm::mat4& mat) const {
      glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void SetVec4(const std::string& name, float x, float y, float z, float w) const {
      glUniform4f(GetUniformLocation(name), x, y, z, w);
    }

    void SetInt(const std::string& name, int value) const {
      glUniform1i(GetUniformLocation(name), value);
    }

  private:
    // 👉 CACHÉ DE UBICACIONES: Cero consultas lentas de strings a la GPU
    mutable std::unordered_map<std::string, GLint> uniformLocationCache;

    GLint GetUniformLocation(const std::string& name) const {
      auto it = uniformLocationCache.find(name);
      if (it != uniformLocationCache.end()) {
        return it->second; // Ya estaba en memoria (Costo = 0)
      }

      // Se consulta a la GPU una sola vez y se memoriza
      GLint location = glGetUniformLocation(ID, name.c_str());
      uniformLocationCache[name] = location;
      return location;
    }

    GLuint CompileShader(GLenum type, const char* source) {
      GLuint shader = glCreateShader(type);
      glShaderSource(shader, 1, &source, nullptr);
      glCompileShader(shader);

      GLint success;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error compilando Shader:\n" << infoLog << std::endl;
        return 0;
      }
      return shader;
    }
};