# Tetris

A basic tetris game with multiplayer modes.

## Building
This project uses asio as networking library which is included as a git submodule. Make sure that you also clone the submodules:
- One way is to use `git clone --recurse-submodules` when cloning.
- After cloning normally, you can also call `git submodule init` and `git submodule update`.

We also need SDL2 as a dependency. [GitHub](https://github.com/libsdl-org/SDL/releases/tag/release-2.28.5). (Use a package manager on Linux.)
If you are on Windows, you need to tell CMAKE where to find the package. Put this into CMakeLists.txt
```
list(APPEND CMAKE_PREFIX_PATH path/to/SDL2-2.28.5) # you can also use ${CMAKE_CURRENT_SOURCE_DIR} for relative path
```
After using CMAKE to configure the project, the location will be cached, so you can delete the line again.
Then, you also need to move the .dll in the same folder as the binary.