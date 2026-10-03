cmake_minimum_required(VERSION 3.20)
project(FirstGame CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)

# 1. El ejecutable ahora busca Game.cpp dentro de src/Core/
add_executable(FirstGame 
    src/main.cpp
    src/Core/Game.cpp
)

# 2. La línea mágica: toma "src" como raíz de búsqueda para todos los includes
target_include_directories(FirstGame PRIVATE src)

target_link_libraries(FirstGame PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main
)