#include "Game.h"
#include "Time.h" // 👈 1. Incluimos nuestro reloj propio
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
  playerX += speed * Time::GetDeltaTime();
  if (playerX > 800.0f) {
    playerX = -100.0f;
  }
  playerRect.x = static_cast<int>(playerX);
}

void Game::Render() {
  // A. Fondo
  SDL_SetRenderDrawColor(renderer, 30, 35, 45, 255);
  SDL_RenderClear(renderer);

  // B. Jugador
  SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
  SDL_RenderFillRect(renderer, &playerRect);

  // C. Presentar
  SDL_RenderPresent(renderer);
}

void Game::Clean() {
  if (renderer) SDL_DestroyRenderer(renderer);
  if (window) SDL_DestroyWindow(window);
  SDL_Quit();
}