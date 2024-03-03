cmake_minimum_required(VERSION 3.20)
project(tetris)

set(CMAKE_CXX_STANDARD 17)

add_executable(tetris
        src/main.cpp

        src/application/client/Tetris.cpp
        src/application/client/state/game/Board.cpp
        src/application/io/Window.cpp
        src/application/io/input/InputAdapter.cpp
        src/application/io/input/VirtualInputAdapter.cpp
        src/application/io/input/SDLInputAdapter.cpp

        src/application/client/state/menu/Menu.cpp
        src/application/client/state/main_menu/MainMenu.cpp
        src/application/tetris/GameLogic.cpp
        src/application/client/state/game/Game.cpp

        src/utils/Vec2.cpp
        src/application/client/Tetris.cpp

        examples/Networking.cpp
        src/networking/BinarySerialize.cpp src/networking/SocketWrapper.cpp src/networking/Server.cpp src/application/server/MirrorServer.cpp)

include_directories(
        lib/asio/asio/include
        src
)

include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    add_compile_definitions(IS_BIG_ENDIAN)
else()
    add_compile_definitions(IS_LITTLE_ENDIAN)
endif()

if(MINGW)
    target_link_libraries(tetris ws2_32)
    target_link_libraries(tetris wsock32)
endif()

#list(APPEND CMAKE_PREFIX_PATH P:/tool/SDL2-2.28.5)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(tetris ${SDL2_LIBRARIES})