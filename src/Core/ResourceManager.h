#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include "Graphics/Material.h"
#include "Graphics/Texture2D.h" // 👈 Ahora en Graphics
#include "Graphics/Shader.h"    // 👈 Ahora en Graphics

class ResourceManager {
  public:
    // 👉 OBTENER TEXTURA CON CACHÉ: Si ya existe en VRAM la reutiliza, si no, la carga
    static std::shared_ptr<Texture2D> GetTexture(const std::string& filePath, bool pixelArt = true) {
      auto it = textures.find(filePath);
      if (it != textures.end()) {
        return it->second; // Devuelve la que ya está en memoria (Cero costo)
      }

      // Si no existe, la cargamos por primera vez
      auto texture = std::make_shared<Texture2D>();
      if (texture->LoadFromFile(filePath, pixelArt)) {
        textures[filePath] = texture;
        return texture;
      }

      std::cerr << "⚠️ [ResourceManager] No se pudo cargar la textura: " << filePath << std::endl;
      return Texture2D::GetWhiteTexture(); // Fallback seguro
    }

    // 👉 OBTENER SHADER CON CACHÉ
    static std::shared_ptr<Shader> GetShader(const std::string& name, const char* vSource, const char* fSource) {
      auto it = shaders.find(name);
      if (it != shaders.end()) {
        return it->second;
      }

      auto shader = std::make_shared<Shader>();
      if (shader->LoadFromSource(vSource, fSource)) {
        shaders[name] = shader;
        return shader;
      }

      return nullptr;
    }

    // 👉 LIMPIEZA TOTAL ANTES DE APAGAR OPENGL
    static void Clean() {
      textures.clear();
      shaders.clear();
      
      // Vaciamos los recursos por defecto del motor mientras la GPU aún nos escucha
      Texture2D::UnloadWhiteTexture();
      Material::UnloadDefaultShader();
    }

  private:
    static inline std::unordered_map<std::string, std::shared_ptr<Texture2D>> textures;
    static inline std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
};