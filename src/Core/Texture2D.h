#pragma once
#include <glad/glad.h>
#include <string>
#include <memory>

class Texture2D {
  public:
    GLuint ID = 0;
    int width = 0;
    int height = 0;
    int channels = 0;

    Texture2D() = default;
    ~Texture2D();

    // Prohibir copias accidentales de IDs de OpenGL
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    // Carga un archivo PNG o JPG desde disco
    // pixelArt = true usa GL_NEAREST (bordes nítidos para retro/pixel art)
    bool LoadFromFile(const std::string& filePath, bool pixelArt = true);

    // Enlazar la textura en la GPU
    void Bind(unsigned int slot = 0) const;

    // Genera una textura blanca de 1x1 en memoria (para objetos sin imagen)
    static std::shared_ptr<Texture2D> GetWhiteTexture();

  private:
    static inline std::shared_ptr<Texture2D> whiteTexture = nullptr;
};