#include "Texture2D.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D::~Texture2D() {
  if (ID != 0) {
    glDeleteTextures(1, &ID);
  }
}

bool Texture2D::LoadFromFile(const std::string& filePath, bool pixelArt) {
  // 👉 Forzamos '4' (RGBA) para garantizar alineación de memoria perfecta en la GPU
  unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 4);
  if (!data) {
    std::cerr << "Error al cargar la textura: " << filePath << std::endl;
    return false;
  }

  // Ahora siempre sube como RGBA con 4 bytes por píxel alineados
  channels = 4;
  GLenum internalFormat = GL_RGBA;
  GLenum dataFormat = GL_RGBA;

  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);

  GLint filter = pixelArt ? GL_NEAREST : GL_LINEAR;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
  return true;
}

void Texture2D::Bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, ID);
}

std::shared_ptr<Texture2D> Texture2D::GetWhiteTexture() {
  if (whiteTexture != nullptr) return whiteTexture;

  whiteTexture = std::make_shared<Texture2D>();
  whiteTexture->width = 1;
  whiteTexture->height = 1;
  whiteTexture->channels = 4;

  uint32_t whitePixel = 0xFFFFFFFF; // RGBA: (255, 255, 255, 255)

  glGenTextures(1, &whiteTexture->ID);
  glBindTexture(GL_TEXTURE_2D, whiteTexture->ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whitePixel);
  return whiteTexture;
}