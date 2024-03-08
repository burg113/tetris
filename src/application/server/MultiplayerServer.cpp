//
// Created by lucpp on 04.03.24.
//

#include "spdlog/spdlog.h"
#include "MultiplayerServer.h"
#include "networking/BinarySerialize.h"
#include "application/io/input/SDLInputAdapter.h"
#include <thread>

MultiplayerServer::MultiplayerServer(ServerHelper *server, Window *window) : server(server), window(window) {
    server->addConnectCallback([this](SocketWrapper *socket) {
        games.emplace(socket->getId(), GameLogic());
        socket->addReadCallback([this](SocketWrapper *socket, const std::string &data) {
            handleSocketRead(socket, data);
        });
        socket->addCloseCallback([this](SocketWrapper *socket) {
            games.erase(socket->getId());
        });
    });
}

void MultiplayerServer::handleSocketRead(SocketWrapper *socket, const std::string &data) {
    BinaryStream stream(data);
    unsigned char info;
    stream >> info;
    if (info == 0) {
        int frameCount;
        InputData inputData;
        stream >> frameCount >> inputData;
        GameLogic& game = games[socket->getId()];
        game.update(inputData);

        // respond with game state after input
        stream.reset();
        stream << game;

        std::vector<GameLogic> players;
        for(auto [ind, logic] : games) if (ind != socket->getId())players.emplace_back(logic);
        stream << players;

        socket->send(stream.getData());

        render();
    }
    else{
        SPDLOG_ERROR("Invalid info flag {}", (int)info);
        assert(false);
    }
}

void MultiplayerServer::run() {
    auto t1 = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    server->startAccepting();
    SDLInputAdapter *sdlAdapter = SDLInputAdapter::get();

    while (!sdlAdapter->quit()) {
        while (std::chrono::duration_cast<std::chrono::microseconds>(
                t1 - std::chrono::high_resolution_clock::now()).count() + (int) 1e6 / FRAMERATE > 0)
            continue;

        SPDLOG_TRACE("Frame took: {} ms", (int) std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - t1).count());

        t1 = std::chrono::high_resolution_clock::now();
        frameCount++;

        server->getIoService().poll();

        render();

        sdlAdapter->update();
        window->render();


    }
}

void MultiplayerServer::render() {
//    Board *board = &game.board;
//
//    float fill = .9;
//    int size = std::min(window->width / board->width, window->height / board->height) * fill;
//
//    Vec2 upperCorner(window->width / 2 - size * board->width / 2,
//                     window->height / 2 - size * board->height / 2);
//    for (int y = 0; y < board->height; y++) {
//        for (int x = 0; x < board->width; x++) {
//            SDL_Color col;
//
//            if ((*board)[x][y] == BOARD_INDEX_EMTPY)
//                col = {40,40,40,255};
//            else
//                col = {120, 120, 120, 255};
//
//            window->draw(upperCorner.x + size * x, upperCorner.y + size * y, size, size, col);
//        }
//    }
//
//    Vec2 offset(0, size - (size * game.framesToFall) / game.framesPerFall);
//    if (!game.checkOffset(Vec2(0, 1)) || !smoothAnimation) offset = Vec2(0, 0);
//
//    for (Vec2 v: game.pieces[game.pieceInd].tiles) {
//        Vec2 p = upperCorner + offset + (v.rot(game.rotation) + game.position) * size;
//        window->draw(p.x, p.y, size, size, colors[game.pieces[game.pieceInd].colorId]);
//    }
}
