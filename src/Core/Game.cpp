#include "Game.h"
#include "Time.h" // 👈 1. Incluimos nuestro reloj propio
#include "Scenes/Level1Scene.h" // 👈 Único nivel a cargar
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

  // 👈 Cargamos el Nivel 1 de forma limpia y polimórfica
  currentScene = std::make_unique<Level1Scene>();
  currentScene->Init();

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
  if (currentScene != nullptr) {
    currentScene->Update(Time::GetDeltaTime());
  }
}

void Game::Render() {


  // A. Eligimos el color del pincel. Le decimos a la GPU
  // Moja el pincel en pintura azul oscuro (R:30, G:35, B:45)
  SDL_SetRenderDrawColor(renderer, 30, 35, 45, 255);

  // ESTA es la línea que realmente PINTA todo el fondo
  // azul oscuro borrando todo lo que había en el ciclo
  // anterior para que se vuelve a pintar el rectángulo
  // verde!
  SDL_RenderClear(renderer);

  if (currentScene != nullptr) {
    currentScene->Render(renderer);
  }

  // C. Presentamos en pantalla: Finalmente acá, vemos el
  // nuevo escenario, con la nueva información en pantalla.
  SDL_RenderPresent(renderer);
}

void Game::Clean() {
  currentScene.reset(); // Destruye la escena y todos
  // sus personajes antes de apagar SDL
  if (renderer) SDL_DestroyRenderer(renderer);
  if (window) SDL_DestroyWindow(window);
  SDL_Quit();
}