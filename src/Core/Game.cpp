#include "Game.h"
#include "Time.h" // 👈 1. Incluimos nuestro reloj propio
#include "Components/Transform.h" // 👈 Traemos la posición
#include "Components/SquareRenderer.h" // 👈 Traemos el dibujado
#include "Components/PlayerController.h" // 👈 1. Incluimos el componente del héroe
#include "Components/BoxCollider.h"
#include <iostream>

GameObject* Game::CreateGameObject(const std::string& name) {
  auto newObj = std::make_unique<GameObject>(name);
  GameObject* rawPtr = newObj.get();
  gameObjects.push_back(std::move(newObj));
  return rawPtr;
}

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

  // ==============================================
  // 1. CREAMOS AL JUGADOR (Verde y con controles)
  // ==============================================
  auto* player = CreateGameObject("Player");
  auto* playerTransform = player->AddComponent<Transform>();
  playerTransform->x = 100.0f;
  playerTransform->y = 250.0f;
  playerTransform->width = 80;
  playerTransform->height = 80;
  // 2. Le agregamos el componente que lo dibuja de verde
  player->AddComponent<SquareRenderer>();
  // Pieza 3: Cerebro / Movimiento 👈
  player->AddComponent<PlayerController>(); // Tiene teclado
  player->AddComponent<BoxCollider>(); // 👈 Le damos cuerpo físico

  // ==========================================
  // 2. CREAMOS A UN ENEMIGO (Rojo y estático)
  // ==========================================
  auto* enemy = CreateGameObject("Enemy");
  auto* enemyTransfom = enemy->AddComponent<Transform>();
  enemyTransfom->x = 500.0f;
  enemyTransfom->y = 250.0f;
  enemyTransfom->width = 80;
  enemyTransfom->height = 80;

  auto* enemyRender = enemy->AddComponent<SquareRenderer>();
  enemyRender->r = 220;
  enemyRender->g = 50;
  enemyRender->b = 50; // Le cambiamos el color a rojo
  enemy->AddComponent<BoxCollider>(); // 👈 Le damos cuerpo físico
  // 👈 ¡Fíjate que al enemigo NO le agregamos PlayerController!

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
  // 👈 Actualiza a TODOS los GameObjects del mundo de un solo golpe
  for (auto& obj : gameObjects) {
    obj->Update(Time::GetDeltaTime());
  }

  // 2. 👈 DETECCIÓN DE COLISIÓN EN TIEMPO REAL
  auto* pCol = gameObjects[0]->GetComponent<BoxCollider>();
  auto* eCol = gameObjects[1]->GetComponent<BoxCollider>();
  auto* eRender = gameObjects[1]->GetComponent<SquareRenderer>();

  if (pCol && eCol && eRender) {
    if (pCol->CheckCollision(*eCol)) {
      // ¡Chocaron! Cambiamos al enemigo a color AMARILLO de alerta
      eRender->r = 255; eRender->g = 255; eRender->b = 0;
    } else {
      // No hay choque: vuelve a color ROJO
      eRender->r = 220; eRender->g = 50; eRender->b = 50;
    }
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

  // B. 👈 Acá le pasa por argumento a la Clase GameObject
  // y éste a la Clase SquareRenderer el pincel, para que
  // pinte el rectángulo verde.
  // 👈 Dibuja a TODOS los GameObjects del mundo
  for (auto& obj : gameObjects) {
    obj->Render(renderer);
  }

  // C. Presentamos en pantalla: Finalmente acá, vemos el
  // nuevo escenario, con la nueva información en pantalla.
  SDL_RenderPresent(renderer);
}

void Game::Clean() {
  if (renderer) SDL_DestroyRenderer(renderer);
  if (window) SDL_DestroyWindow(window);
  SDL_Quit();
}