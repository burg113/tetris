//
// Created by lucas on 16.12.2023.
//

#ifndef TETRIS_MIRRORSERVER_H
#define TETRIS_MIRRORSERVER_H

#include "game/GameLogic.h"
#include "networking/server.h"
#include "game/io/input/VirtualInputAdapter.h"
#include "game/io/Window.h"

class MirrorServer {
    int FRAMERATE = 60;

    bool smoothAnimation = true;
    std::vector<SDL_Color> colors = {{20, 20, 200, 255},
                                     {240, 180, 20, 255},
                                     {15, 230, 15, 255},
                                     {140, 20, 180, 255},
                                     {200, 20, 20, 255},
                                     {20, 200, 240, 255},
                                     {215, 220, 0, 255}};

    bool updateGame = false;

public:

    explicit MirrorServer(Server *server, Window *window);

    void handleSocketRead(SocketWrapper *socket, const std::string &data);

    void run();

    void render();

    Server *server;
    Window *window;
    VirtualInputAdapter inputAdapter;
    GameLogic game;
};


#endif //TETRIS_MIRRORSERVER_H
