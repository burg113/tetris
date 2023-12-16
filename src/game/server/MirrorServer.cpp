//
// Created by lucas on 16.12.2023.
//

#include "MirrorServer.h"
#include "networking/BinarySerialize.h"
#include "game/io/input/SDLInputAdapter.h"
#include <thread>

MirrorServer::MirrorServer(Server *server, Window *window) : server(server), window(window),
                                                             inputAdapter(GameLogic::Key::SIZE) {
    server->addConnectCallback([this](SocketWrapper *socket) {
        updateGame = true;
        socket->addReadCallback([this](SocketWrapper *socket, const std::string &data) {
            handleSocketRead(socket, data);
        });
    });
    game.setInputAdapter(&inputAdapter);
}

void MirrorServer::handleSocketRead(SocketWrapper *socket, const std::string &data) {
    std::stringstream strstr(data);
    int key;
    bool set;
    unsigned char info;
    strstr >> binr(info);
    if (info == 0) {
        strstr >> binr(key) >> binr(set);
        inputAdapter.update(key, set);
    }
    if (info == 1) {
        game.update();
        render();
    }
}

void MirrorServer::run() {
    auto t1 = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    server->startAccepting();
    SDLInputAdapter *sdlAdapter = SDLInputAdapter::get();

    while (!sdlAdapter->quit()) {
        while (std::chrono::duration_cast<std::chrono::microseconds>(
                t1 - std::chrono::high_resolution_clock::now()).count() + (int) 1e6 / FRAMERATE > 0)
            continue;

        std::cerr << "Frame took: " << (int) std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - t1).count() << " ms" << std::endl;

        t1 = std::chrono::high_resolution_clock::now();
        frameCount++;

        server->getIoService().poll();

        /*if (updateGame) {
            game.update();
        }*/
        render();

        sdlAdapter->update();
        window->render();


    }
}

void MirrorServer::render() {
    Board *board = &game.board;


    float fill = .9;
    int size = std::min(window->width / board->width, window->height / board->height) * fill;

    Vec2 upperCorner(window->width / 2 - size * board->width / 2,
                     window->height / 2 - size * board->height / 2);
    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            SDL_Color col;

            if ((*board)[x][y] == 0)
                col = {(unsigned char) (x * 10 + 50), (unsigned char) (y * 5 + 50), 0, 255};
            else
                col = {200, 200, 200, 255};

            window->draw(upperCorner.x + size * x, upperCorner.y + size * y, size, size, col);
        }
    }

    Vec2 offset(0, size - (size * game.framesToFall) / game.framesPerFall);
    if (!game.checkOffset(Vec2(0, 1)) || !smoothAnimation) offset = Vec2(0, 0);

    for (Vec2 v: game.piece.tiles) {
        Vec2 p = upperCorner + offset + (v.rot(game.rotation) + game.position) * size;
        window->draw(p.x, p.y, size, size, colors[game.piece.colorId]);
    }
}
