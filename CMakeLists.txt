cmake_minimum_required(VERSION 3.20)
project(FirstGame CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Buscar librerías requeridas
find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Stb REQUIRED)

# 2. Archivos fuente del ejecutable
add_executable(FirstGame 
  src/main.cpp
  src/Core/Game.cpp
  src/Core/Scene.cpp
  src/Core/CollisionSystem.cpp # 👈 NUEVO: El subsistema de físicas
  src/Graphics/Texture2D.cpp # 👈 Ahora en Graphics
)

# 3. La línea mágica: toma "src" como raíz de búsqueda para todos los includes
target_include_directories(FirstGame PRIVATE src ${Stb_INCLUDE_DIR})

# 4. Enlazar librerías a FirstGame
target_link_libraries(FirstGame PRIVATE 
  SDL2::SDL2 
  SDL2::SDL2main
  glad::glad
  glm::glm
  OpenGL::GL
)