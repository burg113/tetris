cmake_minimum_required(VERSION 3.20)
project(tetris)

set(CMAKE_CXX_STANDARD 17)

add_executable(tetris
        src/main.cpp

        src/game/Tetris.cpp
        src/game/state/Board.cpp

        src/game/io/Window.cpp
        src/game/io/KeyboardAdapter.cpp

        src/game/state/Menu.cpp
        src/game/state/MainMenu.cpp
        src/game/state/Game.cpp

        src/game/utils/Vec2.cpp

        src/networking/Networking.cpp
        )

include_directories(
        lib/asio/asio/include
)

if(MINGW)
    target_link_libraries(tetris ws2_32)
    target_link_libraries(tetris wsock32)
endif()

#list(APPEND CMAKE_PREFIX_PATH P:/tool/SDL2-2.28.5)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(tetris ${SDL2_LIBRARIES})