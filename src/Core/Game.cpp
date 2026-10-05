#include <glad/glad.h> // 👈 1. SIEMPRE PRIMERO: GLAD conecta con los drivers de tu GPU
#include "Game.h"
#include "Time.h" // 👈 1. Incluimos nuestro reloj propio
#include "Scenes/Level1Scene.h" // 👈 Único nivel a cargar
#include <iostream>

bool Game::Init(const char* title, int width, int height) {
  // 1. Iniciar subsistema de video de SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "Error iniciando SDL: " << SDL_GetError() << std::endl;
    return false;
  }

  // 2. Configurar atributos de OpenGL Moderno (Versión 3.3 Core Profile)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // 3. Activar Doble Buffer (para evitar parpadeos) y Buffer de Profundidad (Z-Buffer para 3D)
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  // 4. Crear ventana con la bandera de OpenGL activada
  window = SDL_CreateWindow(
    title,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN // 👈 Le dice a Windows que dibuje con la GPU
  );

  if (!window) {
    std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
    return false;
  }

  // 👉 DETECCIÓN AUTOMÁTICA DE HERTZ DEL MONITOR
  SDL_DisplayMode displayMode;
  int displayIndex = SDL_GetWindowDisplayIndex(window);
  if (SDL_GetCurrentDisplayMode(displayIndex, &displayMode) == 0 && displayMode.refresh_rate > 0) {
    Time::SetTargetFPS(displayMode.refresh_rate);
    std::cout << " Monitor detectado a: " << displayMode.refresh_rate << " Hz. Limitando a esa tasa." << std::endl;
  } else {
    Time::SetTargetFPS(60); // Respaldo seguro si no se puede leer el monitor
  }

  // 5. Crear el Contexto de OpenGL sobre la ventana
  glContext = SDL_GL_CreateContext(window);
  if (!glContext) {
    std::cerr << "Error al crear el contexto de OpenGL: " << SDL_GetError() << std::endl;
    return false;
  }

  // 6. Cargar todas las funciones de la GPU usando GLAD
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::cerr << "Error al inicializar GLAD!" << std::endl;
    return false;
  }

  // 7. Área de dibujo (Viewport) y VSync (60 FPS estables)
  glViewport(0, 0, width, height);
  SDL_GL_SetSwapInterval(1); // 1 = VSync activado

  // 8. Mensaje de diagnóstico: Te dirá qué tarjeta gráfica está usando tu juego
  std::cout << "========================================" << std::endl;
  std::cout << " OpenGL inicializado con éxito!" << std::endl;
  std::cout << " GPU: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << " Versión OpenGL: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "========================================" << std::endl;

  // 👈 9. Cargar escena
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

    // Mostramos los FPS limpios leídos desde la clase Time
    std::string title = "Mi Motor C++ | 2K | FPS: " + std::to_string(Time::GetFPS());
    SDL_SetWindowTitle(window, title.c_str());
  }
}

void Game::Render() {
  // 👉 1. Le decimos a la GPU: "Pinta el fondo con este color azul oscuro"
  // En OpenGL los colores van de 0.0f a 1.0f (30/255 = ~0.11f)
  glClearColor(30.0f / 255.0f, 35.0f / 255.0f, 45.0f / 255.0f, 1.0f);
  
  // 👉 2. Limpia el buffer de color y el buffer de profundidad (Z)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 👉 3. Renderizamos la escena
  if (currentScene != nullptr) {
    currentScene->Render();
  }

  // 👉 4. Intercambiamos el lienzo oculto con el visible (Doble Buffer)
  SDL_GL_SwapWindow(window);
}

void Game::Clean() {
  currentScene.reset(); // Destruye la escena y todos
  // sus personajes antes de apagar SDL
  if (glContext) SDL_GL_DeleteContext(glContext);
  if (window) SDL_DestroyWindow(window);
  SDL_Quit();
}