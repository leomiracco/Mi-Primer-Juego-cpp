#pragma once
#include <SDL.h>

class Game {
  public:
    Game() = default;
    ~Game() = default;

    // Métodos públicos del ciclo de vida
    bool Init(const char* title, int width, int height);
    void Run();
    void Clean();

  private:
    // Métodos internos que se ejecutan en cada vuelta del bucle
    void HandleEvents();
    void Update();
    void Render();

    bool isRunning = false;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Variables temporales de nuestro personaje cuadrado
    float playerX = 50.0f;
    float speed = 250.0f;
    SDL_Rect playerRect = { 50, 250, 100, 100 };
};