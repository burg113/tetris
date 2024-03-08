//
// Created by lucpp on 04.03.24.
//

#include "spdlog/spdlog.h"
#include "MultiplayerServer.h"
#include "networking/BinarySerialize.h"
#include "application/io/input/SDLInputAdapter.h"
#include <thread>

MultiplayerServer::MultiplayerServer(ServerHelper *server) : server(server) {
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
    }
}

void MultiplayerServer::render() {

}
