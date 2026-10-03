#include "Core/Game.h" // 👈 Ahora la ruta es limpia y explícita

int main(int argc, char* argv[]) {
    
    Game game;

    // Inicializamos la ventana y el motor
    if (game.Init("Mi Primer Juego C++", 800, 600)) {
        game.Run(); // 👈 Aquí se queda latiendo el juego hasta que cierres la ventana
    }

    game.Clean(); // 👈 Liberamos la memoria al salir
    return 0;
}