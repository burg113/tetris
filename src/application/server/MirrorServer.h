//
// Created by lucas on 16.12.2023.
//

#pragma once

#include "application/tetris/GameLogic.h"
#include "networking/ServerHelper.h"
#include "application/io/input/VirtualInputAdapter.h"
#include "application/io/output/sdl/Window.h"

class [[maybe_unused]] MirrorServer {
    int FRAMERATE = 60;

    bool smoothAnimation = true;
    std::vector<SDL_Color> colors = {{20, 20, 200, 255},
                                     {240, 180, 20, 255},
                                     {15, 230, 15, 255},
                                     {140, 20, 180, 255},
                                     {200, 20, 20, 255},
                                     {20, 200, 240, 255},
                                     {215, 220, 0, 255}};

public:

    [[maybe_unused]] explicit MirrorServer(ServerHelper *server, Window *window);

    void handleSocketRead(SocketWrapper *socket, const std::string &data);

    void run();

    void render();

    ServerHelper *server;
    Window *window;
    GameLogic game;
};


