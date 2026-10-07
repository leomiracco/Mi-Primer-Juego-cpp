#pragma once
#include <SDL.h>
#include <cstring>
#include "Core/Vector3.h"

// 👉 Enum de Teclado
enum class KeyCode {
    A = SDL_SCANCODE_A, B = SDL_SCANCODE_B, C = SDL_SCANCODE_C, D = SDL_SCANCODE_D,
    E = SDL_SCANCODE_E, F = SDL_SCANCODE_F, G = SDL_SCANCODE_G, H = SDL_SCANCODE_H,
    I = SDL_SCANCODE_I, J = SDL_SCANCODE_J, K = SDL_SCANCODE_K, L = SDL_SCANCODE_L,
    M = SDL_SCANCODE_M, N = SDL_SCANCODE_N, O = SDL_SCANCODE_O, P = SDL_SCANCODE_P,
    Q = SDL_SCANCODE_Q, R = SDL_SCANCODE_R, S = SDL_SCANCODE_S, T = SDL_SCANCODE_T,
    U = SDL_SCANCODE_U, V = SDL_SCANCODE_V, W = SDL_SCANCODE_W, X = SDL_SCANCODE_X,
    Y = SDL_SCANCODE_Y, Z = SDL_SCANCODE_Z,

    Up = SDL_SCANCODE_UP, Down = SDL_SCANCODE_DOWN,
    Left = SDL_SCANCODE_LEFT, Right = SDL_SCANCODE_RIGHT,

    Space = SDL_SCANCODE_SPACE,
    Escape = SDL_SCANCODE_ESCAPE,
    Return = SDL_SCANCODE_RETURN,
    LeftShift = SDL_SCANCODE_LSHIFT,
    LeftControl = SDL_SCANCODE_LCTRL
};

// 👉 Enum de Botones del Ratón (Estándar Unity)
enum class MouseButton {
    Left   = SDL_BUTTON_LEFT,   // Clic Izquierdo (1)
    Middle = SDL_BUTTON_MIDDLE, // Clic Rueda (2)
    Right  = SDL_BUTTON_RIGHT   // Clic Derecho (3)
};

class Input {
  public:
    static void Update() {
      // 1. Doble buffer de teclado
      if (!currentState) {
        currentState = SDL_GetKeyboardState(nullptr);
      }
      std::memcpy(previousState, currentState, SDL_NUM_SCANCODES);

      // 2. Doble buffer de botones de ratón
      previousMouseButtons = currentMouseButtons;
      prevMouseX = mouseX;
      prevMouseY = mouseY;
      mouseScroll = 0.0f;

      // 3. Lectura de hardware del ratón
      currentMouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

      // 👉 ELIMINA EL LATIGAZO DEL PRIMER FRAME:
      // Si es el primer fotograma, sincroniza la posición previa para que el delta sea (0, 0)
      if (firstMouseFrame) {
        prevMouseX = mouseX;
        prevMouseY = mouseY;
        firstMouseFrame = false;
      }
    }

    // Procesa eventos instantáneos del sistema operativo (Rueda del ratón)
    static void ProcessEvent(const SDL_Event& event) {
      if (event.type == SDL_MOUSEWHEEL) {
        mouseScroll = static_cast<float>(event.wheel.y);
      }
    }

    // ==============================================================
    // ⌨️ CONSULTAS DE TECLADO
    // ==============================================================
    static bool GetKey(KeyCode key) {
      EnsureInitialized();
      int scancode = static_cast<int>(key);
      return currentState[scancode] != 0;
    }

    static bool GetKeyDown(KeyCode key) {
      EnsureInitialized();
      int scancode = static_cast<int>(key);
      return currentState[scancode] && !previousState[scancode];
    }

    static bool GetKeyUp(KeyCode key) {
      EnsureInitialized();
      int scancode = static_cast<int>(key);
      return !currentState[scancode] && previousState[scancode];
    }

    static float GetAxisHorizontal() {
      float axis = 0.0f;
      if (GetKey(KeyCode::D) || GetKey(KeyCode::Right)) axis += 1.0f;
      if (GetKey(KeyCode::A) || GetKey(KeyCode::Left))  axis -= 1.0f;
      return axis;
    }

    static float GetAxisVertical() {
      float axis = 0.0f;
      if (GetKey(KeyCode::S) || GetKey(KeyCode::Down)) axis += 1.0f;
      if (GetKey(KeyCode::W) || GetKey(KeyCode::Up))   axis -= 1.0f;
      return axis;
    }

    static bool GetButtonFire() {
      return GetKey(KeyCode::Space) || GetMouseButton(MouseButton::Left);
    }

    // ==============================================================
    // 🖱️ CONSULTAS DE RATÓN (MOUSE)
    // ==============================================================
    
    // Coordenadas absolutas del cursor en la ventana (en píxeles)
    static Vector3 GetMousePosition() {
      return Vector3(static_cast<float>(mouseX), static_cast<float>(mouseY), 0.0f);
    }

    // Delta relativo de movimiento (Cuánto se movió en este frame - Esencial para cámaras 3D)
    static Vector3 GetMouseDelta() {
      return Vector3(static_cast<float>(mouseX - prevMouseX), 
                     static_cast<float>(mouseY - prevMouseY), 0.0f);
    }

    // Clic sostenido
    static bool GetMouseButton(MouseButton button) {
      Uint32 mask = SDL_BUTTON(static_cast<int>(button));
      return (currentMouseButtons & mask) != 0;
    }

    // Clic único (al bajar el botón)
    static bool GetMouseButtonDown(MouseButton button) {
      Uint32 mask = SDL_BUTTON(static_cast<int>(button));
      return (currentMouseButtons & mask) != 0 && (previousMouseButtons & mask) == 0;
    }

    // Clic liberado (al soltar el botón)
    static bool GetMouseButtonUp(MouseButton button) {
      Uint32 mask = SDL_BUTTON(static_cast<int>(button));
      return (currentMouseButtons & mask) == 0 && (previousMouseButtons & mask) != 0;
    }

    // Rueda del ratón (+1.0 arriba, -1.0 abajo)
    static float GetMouseScroll() {
      return mouseScroll;
    }

  private:
    static inline const Uint8* currentState = nullptr;
    static inline Uint8 previousState[SDL_NUM_SCANCODES] = { 0 };

    static inline int mouseX = 0, mouseY = 0;
    static inline int prevMouseX = 0, prevMouseY = 0;
    static inline float mouseScroll = 0.0f;
    static inline Uint32 currentMouseButtons = 0;
    static inline Uint32 previousMouseButtons = 0;
    static inline bool firstMouseFrame = true; // 👈 Bandera de arranque suave

    static void EnsureInitialized() {
      if (!currentState) {
        currentState = SDL_GetKeyboardState(nullptr);
      }
    }
};