#pragma once
#include <SDL.h>

class Input {
  public:
  // 1. Devuelve true si la tecla física está siendo presionada en este instante
  static bool GetKey(SDL_Scancode key) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    return state != nullptr && state[key];
  }

  // 2. Eje Horizontal: devuelve -1.0 (Izquierda), 1.0 (Derecha) o 0.0 (Quieto)
  static float GetAxisHorizontal() {
    float axis = 0.0f;
      if (GetKey(SDL_SCANCODE_D) || GetKey(SDL_SCANCODE_RIGHT)) axis += 1.0f;
      if (GetKey(SDL_SCANCODE_A) || GetKey(SDL_SCANCODE_LEFT))  axis -= 1.0f;
    return axis;
  }

  // 3. Eje Vertical: devuelve -1.0 (Arriba), 1.0 (Abajo) o 0.0 (Quieto)
  static float GetAxisVertical() {
    float axis = 0.0f;
      if (GetKey(SDL_SCANCODE_S) || GetKey(SDL_SCANCODE_DOWN)) axis += 1.0f;
      if (GetKey(SDL_SCANCODE_W) || GetKey(SDL_SCANCODE_UP))   axis -= 1.0f;
    return axis;
  }
};