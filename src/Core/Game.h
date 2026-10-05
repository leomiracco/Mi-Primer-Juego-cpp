#pragma once
#include <SDL.h>
#include <vector>
#include <memory>
#include <string>
#include "ECS/GameObject.h" // 👈 Incluimos nuestro contenedor

class Game {
  public:
    Game() = default;
    ~Game() = default;

    // Métodos públicos del ciclo de vida
    bool Init(const char* title, int width, int height);
    void Run();
    void Clean();

    // 👈 Fábrica para crear y registrar nuevos GameObjects en el mundo
    GameObject* CreateGameObject(const std::string& name = "GameObject");

  private:
    // Métodos internos privados que se ejecutan en cada vuelta del bucle
    void HandleEvents();
    void Update();
    void Render();

    bool isRunning = false;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // 👈 La Escena: ahora el mundo guarda TODOS los GameObjects que existan
    std::vector<std::unique_ptr<GameObject>> gameObjects;
};