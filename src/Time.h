#pragma once
#include <chrono>
#include <algorithm> // 👈 Necesario para std::min

class Time {
  public:
    // Se llamará una vez por fotograma para medir cuánto tiempo pasó
    static void Update() {
      auto currentTime = std::chrono::steady_clock::now();
        
      // Calculamos la diferencia entre ahora y el fotograma anterior (en segundos)
      std::chrono::duration<float> elapsed = currentTime - lastTime;
      
      // 👈 CLAMPING: Tomamos el menor valor entre el tiempo real y 0.05 segundos
      float rawDelta = elapsed.count();
      deltaTime = std::min(rawDelta, 0.05f);

      lastTime = currentTime;
    }

    // Para consultar el deltaTime desde cualquier parte del juego
    static float GetDeltaTime() {
      return deltaTime;
    }

  private:
    // Guarda el punto exacto en el tiempo del fotograma anterior
    static inline std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    static inline float deltaTime = 0.0f;
};