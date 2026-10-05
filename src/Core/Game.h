#pragma once
#include <SDL.h>
#include <memory>
#include "Core/Scene.h" // 👈 Incluimos el contenedor de GameObjects Scene

class Game {
  public:
    Game() = default;
    ~Game() = default;

    // Métodos públicos del ciclo de vida
    bool Init(const char* title, int width, int height);
    void Run();
    void Clean();

  private:
    // Métodos internos privados que se ejecutan en cada vuelta del bucle
    void HandleEvents();
    void Update();
    void Render();

    bool isRunning = false;
    SDL_Window* window = nullptr;
    
    // 👉 Adiós SDL_Renderer, bienvenido el contexto nativo de OpenGL
    SDL_GLContext glContext = nullptr; 

    // 👈 Game solo conoce a la Escena activa, no a los personajes sueltos
    std::unique_ptr<Scene> currentScene = nullptr;
};