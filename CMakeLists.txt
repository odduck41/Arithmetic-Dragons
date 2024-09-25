cmake_minimum_required(VERSION 3.28)
project(Arithmetic_Dragons)

set(CMAKE_CXX_STANDARD 26)

add_executable(Arithmetic_Dragons main.cpp
        Game.h
        Game.cpp
        App.cpp
        App.h)

set(SFML_DIR "C:/Users/GDcat/CLionProjects/Mandelbrot_set/cmake-build-debug/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

target_link_libraries(Arithmetic_Dragons PRIVATE
        sfml-system
        sfml-network
        sfml-graphics
        sfml-window
)