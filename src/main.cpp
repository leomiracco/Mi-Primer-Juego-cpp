#include "Core/Game.h" // 👈 Ahora la ruta es limpia y explícita
#include "Core/SceneManager.h"
#include "Scenes/Level1Scene.h" // 👈 Solo main conoce el nivel inicial

int main(int argc, char* argv[]) {
    
  Game game;

  // Inicializamos la ventana y el motor
    
  if (!game.Init("Mi Primer Juego C++", 800, 600)) {
    return -1;
  }

  // El juego decide qué escena arrancar, el motor es 100% genérico
  SceneManager::LoadScene<Level1Scene>();

  game.Run();
  game.Clean(); // 👈 Liberamos la memoria al salir
  
  return 0;
}