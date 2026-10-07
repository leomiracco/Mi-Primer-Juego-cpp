#pragma once
#include <chrono>
#include <thread>
#include <algorithm>

class Time {
  public:
    // 👉 REINICIA EL CRONÓMETRO JUSTO ANTES DE ENTRAR AL BUCLE DE JUEGO
    static void Init() {
      lastTime = std::chrono::steady_clock::now();
    }

    static void Update() {
      auto currentTime = std::chrono::steady_clock::now();

      // 1. Sincronización híbrida de fotogramas (limitador a 144 Hz)
      if (limitFPS && targetFPS > 0) {
        double targetFrameTimeSec = 1.0 / targetFPS;
        std::chrono::duration<double> elapsedSinceLast = currentTime - lastTime;

        while (elapsedSinceLast.count() < targetFrameTimeSec) {
          double remainingSec = targetFrameTimeSec - elapsedSinceLast.count();
          if (remainingSec > 0.002) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
          }
          currentTime = std::chrono::steady_clock::now();
          elapsedSinceLast = currentTime - lastTime;
        }
      }

      // 2. Calculamos el tiempo real transcurrido (sin escalar)
      std::chrono::duration<float> elapsed = currentTime - lastTime;
      float rawDelta = elapsed.count();
      unscaledDeltaTime = std::min(rawDelta, 0.05f);

      // 👉 3. CALCULAMOS EL DELTATIME ESCALADO (Afectado por pausa o slow-motion)
      deltaTime = unscaledDeltaTime * timeScale;

      // 4. Calculamos FPS usando el tiempo REAL (evita división por cero en pausa)
      if (unscaledDeltaTime > 0.0f) {
        currentFPS = static_cast<int>((1.0f / unscaledDeltaTime) + 0.5f);
      }

      lastTime = currentTime;
    }

    // 👉 Consultas de DeltaTime
    static float GetDeltaTime() { return deltaTime; }                   // Usado por personajes y físicas
    static float GetUnscaledDeltaTime() { return unscaledDeltaTime; }   // Usado por menús y UI
    static int GetFPS() { return currentFPS; }

    // 🎛️ CONTROL DE ESCALA DE TIEMPO (Estándar Unity Time.timeScale)
    static void SetTimeScale(float scale) { timeScale = std::max(0.0f, scale); }
    static float GetTimeScale() { return timeScale; }

    // Atajos de conveniencia profesional
    static void Pause() { timeScale = 0.0f; }
    static void Resume() { timeScale = 1.0f; }
    static bool IsPaused() { return timeScale == 0.0f; }

    // Controles de tasa de refresco
    static void SetLimitFPS(bool enable) { limitFPS = enable; }
    static bool IsLimitFPSEnabled() { return limitFPS; }
    static void SetTargetFPS(int fps) { targetFPS = fps; }
    static int GetTargetFPS() { return targetFPS; }

  private:
    static inline std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    static inline float unscaledDeltaTime = 0.0f; // Tiempo real
    static inline float deltaTime = 0.0f;         // Tiempo escalado
    static inline float timeScale = 1.0f;         // 1.0 = normal, 0.0 = pausa, 0.2 = slow-mo
    static inline int currentFPS = 0;

    static inline bool limitFPS = true;
    static inline int targetFPS = 144;
};