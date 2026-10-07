#pragma once

class Screen {
  public:
    static void Init(int width, int height) {
      SetSize(width, height);
    }

    // Actualiza las dimensiones y recalcula la relación de aspecto
    static void SetSize(int width, int height) {
      screenWidth = width;
      screenHeight = height;
      aspectRatio = (height > 0) ? (static_cast<float>(width) / static_cast<float>(height)) : 1.0f;
    }

    // Consultas estáticas estándar (como Screen.width en Unity)
    static int GetWidth() { return screenWidth; }
    static int GetHeight() { return screenHeight; }
    static float GetWidthF() { return static_cast<float>(screenWidth); }
    static float GetHeightF() { return static_cast<float>(screenHeight); }
    static float GetAspectRatio() { return aspectRatio; }

  private:
    static inline int screenWidth = 800;
    static inline int screenHeight = 600;
    static inline float aspectRatio = 800.0f / 600.0f;
};