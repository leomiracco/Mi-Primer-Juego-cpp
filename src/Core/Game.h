#pragma once
#include <SDL.h>

// 🌟 LA TRÍADA DE SUBSISTEMAS DEL MOTOR (El Core Loop los orquesta formalmente)
#include "Core/Time.h"
#include "Core/Input.h"
#include "Core/Screen.h"
#include "Core/ResourceManager.h"
#include "Core/SceneManager.h"

class Game {
  public:
    Game() = default;
    ~Game() = default;

    bool Init(const char* title, int width, int height);
    void Run();
    void Clean();

  private:
    void HandleEvents();
    void Update();
    void Render();

    bool isRunning = false;
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
};