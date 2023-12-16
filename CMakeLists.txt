cmake_minimum_required(VERSION 3.20)
project(tetris)

set(CMAKE_CXX_STANDARD 17)

add_executable(tetris
        src/main.cpp

        src/game/client/Tetris.cpp
        src/game/client/state/Board.cpp
        src/game/io/Window.cpp
        src/game/io/input/InputAdapter.cpp
        src/game/io/input/VirtualInputAdapter.cpp
        src/game/io/input/SDLInputAdapter.cpp

        src/game/client/state/Menu.cpp
        src/game/client/state/MainMenu.cpp
        src/game/GameLogic.cpp
        src/game/client/state/Game.cpp

        src/utils/Vec2.cpp
        src/game/client/Tetris.cpp

        examples/Networking.cpp
        src/networking/BinarySerialize.cpp src/networking/SocketWrapper.cpp src/networking/Server.cpp)

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