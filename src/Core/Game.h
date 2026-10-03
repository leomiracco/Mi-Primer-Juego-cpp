#pragma once
#include <SDL.h>
#include <memory>
#include "ECS/GameObject.h" // 👈 Incluimos nuestro contenedor

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
    SDL_Renderer* renderer = nullptr;

    // 👈 Ahora el personaje es una entidad formal completa
    std::unique_ptr<GameObject> player = nullptr;
};