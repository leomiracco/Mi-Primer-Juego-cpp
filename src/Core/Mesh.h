#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

// Estructura de Vértice Universal (Sirve para 2D, 3D, Iluminación y Texturas)
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
  public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
      indexCount = static_cast<GLsizei>(indices.size());
      SetupMesh(vertices, indices);
    }

    ~Mesh() {
      if (VAO) glDeleteVertexArrays(1, &VAO);
      if (VBO) glDeleteBuffers(1, &VBO);
      if (EBO) glDeleteBuffers(1, &EBO);
    }

    // Prohibimos la copia accidental de punteros de OpenGL
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Dibujado directo en GPU
    void Draw() const {
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }

    // ==============================================================
    // FÁBRICAS DE GEOMETRÍA ESTÁNDAR (Primitivas listas para usar)
    // ==============================================================

    // 1. QUAD 2D: Plano unitario con origen (0,0) en la esquina superior izquierda
    static std::shared_ptr<Mesh> CreateQuad() {
      std::vector<Vertex> vertices = {
        // Posición                  Normal                 UV
        { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} }, // Arriba-Izquierda
        { {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} }, // Arriba-Derecha
        { {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }, // Abajo-Derecha
        { {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }  // Abajo-Izquierda
      };

      std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
      };

      return std::make_shared<Mesh>(vertices, indices);
    }

    // 2. CUBO 3D: Cubo unitario centrado en (0,0,0) con normales para iluminación
    static std::shared_ptr<Mesh> CreateCube() {
      std::vector<Vertex> vertices = {
        // Cara Frontal (Z = +0.5)
        { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
        { { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },
        { { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },

        // Cara Trasera (Z = -0.5)
        { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f} },
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f} },
        { { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f} },
        { { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f} },

        // Cara Superior (Y = +0.5)
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} },
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
        { { 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
        { { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },

        // Cara Inferior (Y = -0.5)
        { {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f} },
        { { 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f} },
        { { 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f} },
        { {-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f} },

        // Cara Derecha (X = +0.5)
        { { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
        { { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
        { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },
        { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },

        // Cara Izquierda (X = -0.5)
        { {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
        { {-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
        { {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },
        { {-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} }
      };

      std::vector<unsigned int> indices = {
         0,  1,  2,      2,  3,  0,    // Frontal
         4,  5,  6,      6,  7,  4,    // Trasera
         8,  9, 10,     10, 11,  8,    // Superior
        12, 13, 14,     14, 15, 12,    // Inferior
        16, 17, 18,     18, 19, 16,    // Derecha
        20, 21, 22,     22, 23, 20     // Izquierda
      };

      return std::make_shared<Mesh>(vertices, indices);
    }

  private:
    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLsizei indexCount = 0;

    void SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

      // Location 0: Posición (x, y, z)
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
      glEnableVertexAttribArray(0);

      // Location 1: Normal (nx, ny, nz)
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
      glEnableVertexAttribArray(1);

      // Location 2: Coordenadas UV (u, v)
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
      glEnableVertexAttribArray(2);

      glBindVertexArray(0);
    }
};