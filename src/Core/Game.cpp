#include "Game.h"
#include "Time.h" // 👈 1. Incluimos nuestro reloj propio
#include "Components/Transform.h" // 👈 Traemos la posición
#include "Components/SquareRenderer.h" // 👈 Traemos el dibujado
#include "Components/Hero.h" // 👈 1. Incluimos el componente del héroe
#include <iostream>

bool Game::Init(const char* title, int width, int height) {
  
  // la ventana, los gráficos y los eventos del teclado/ratón  
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "Error iniciando SDL: " << SDL_GetError() << std::endl;
    return false;
  }

  window = SDL_CreateWindow(
    title,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    SDL_WINDOW_SHOWN
  );

  if (!window) {
    std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
    return false;
  }

  renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );

  if (!renderer) {
    std::cerr << "Error al crear el renderer: " << SDL_GetError() << std::endl;
    return false;
  }

  // ==========================================
  // ENSAMBLANDO NUESTRO GAMEOBJECT (ESTILO UNITY)
  // ==========================================
  player = std::make_unique<GameObject>("Player");

  // 1. Le agregamos el componente de posición y tamaño
  auto* transform = player->AddComponent<Transform>();
  transform->x = 50.0f;
  transform->y = 250.0f;
  transform->width = 100;
  transform->height = 100;

  // 2. Le agregamos el componente que lo dibuja de verde
  player->AddComponent<SquareRenderer>();

  // Pieza 3: Cerebro / Movimiento 👈
  player->AddComponent<Hero>();

  isRunning = true;
  
  return true;
}

void Game::Run() {
  // El bucle principal que hace latir al juego
  while (isRunning) {
    Time::Update();   // 1. Medir tiempo
    HandleEvents();   // 2. Escuchar teclado/ratón
    Update();         // 3. Mover cosas con deltaTime
    Render();         // 4. Dibujar en la GPU
  }
}

void Game::HandleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      isRunning = false;
    }
  }
}

void Game::Update() {
  // Actualizamos al GameObject (él se encarga de actualizar a todos sus componentes)
  player->Update(Time::GetDeltaTime());
}

void Game::Render() {
  // A. Pintamos el fondo azul oscuro
  SDL_SetRenderDrawColor(renderer, 30, 35, 45, 255);
  SDL_RenderClear(renderer);

  // B. 👈 ¡MIRA QUÉ LIMPIEZA! El GameObject se dibuja solo
  player->Render(renderer);

  // C. Presentamos en pantalla
  SDL_RenderPresent(renderer);
}

void Game::Clean() {
  if (renderer) SDL_DestroyRenderer(renderer);
  if (window) SDL_DestroyWindow(window);
  SDL_Quit();
}