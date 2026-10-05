#pragma once
#include <chrono>
#include <thread>
#include <algorithm>

class Time {
  public:
    static void Update() {
      auto currentTime = std::chrono::steady_clock::now();

      // 1. SI EL LIMITADOR ESTÁ ENCENDIDO: Esperamos el tiempo necesario para clavar los FPS
      if (limitFPS && targetFPS > 0) {
        double targetFrameTimeSec = 1.0 / targetFPS;
        std::chrono::duration<double> elapsedSinceLast = currentTime - lastTime;

        // Espera híbrida de alta precisión: 
        // Si sobra más de 2ms, cedemos CPU durmiendo 1ms; el resto lo afinamos al microsegundo.
        while (elapsedSinceLast.count() < targetFrameTimeSec) {
          double remainingSec = targetFrameTimeSec - elapsedSinceLast.count();
          if (remainingSec > 0.002) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
          }
          currentTime = std::chrono::steady_clock::now();
          elapsedSinceLast = currentTime - lastTime;
        }
      }

      // 2. Calculamos el deltaTime real
      std::chrono::duration<float> elapsed = currentTime - lastTime;
      float rawDelta = elapsed.count();
      deltaTime = std::min(rawDelta, 0.05f);

      // 3. Calculamos FPS actuales para el menú o diagnóstico
      if (rawDelta > 0.0f) {
        currentFPS = static_cast<int>((1.0f / rawDelta) + 0.5f);
      }

      lastTime = currentTime;
    }

    // Consultas estándar de juego
    static float GetDeltaTime() { return deltaTime; }
    static int GetFPS() { return currentFPS; }

    // 🎛️ CONTROLES PARA EL FUTURO MENÚ DE OPCIONES
    // Interruptor ON / OFF (como una casilla en el menú)
    static void SetLimitFPS(bool enable) { limitFPS = enable; }
    static bool IsLimitFPSEnabled() { return limitFPS; }

    // Ajuste de tasa objetivo (60, 144, 240, etc.)
    static void SetTargetFPS(int fps) { targetFPS = fps; }
    static int GetTargetFPS() { return targetFPS; }

  private:
    static inline std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    static inline float deltaTime = 0.0f;
    static inline int currentFPS = 0;

    // Variables de configuración (con valores por defecto)
    static inline bool limitFPS = true;
    static inline int targetFPS = 144;
};