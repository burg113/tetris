cmake_minimum_required(VERSION 3.20)
project(tetris)

set(CMAKE_CXX_STANDARD 17)

add_executable(tetris
        main.cpp
        )

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(tetris ${SDL2_LIBRARIES})