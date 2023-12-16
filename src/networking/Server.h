//
// Created by lucas on 09.12.2023.
//

#ifndef TETRIS_SERVER_H
#define TETRIS_SERVER_H

#include "SocketWrapper.h"

using SocketConnectCallback = std::function<void(SocketWrapper*)>;

class Server {
public:
    Server(asio::io_service &ioService, short port);

    void addConnectCallback(const SocketConnectCallback &callback);

    asio::io_service& getIoService();

    void startAccepting();

private:
    asio::io_service &ioService;
    tcp::acceptor acceptor;
    std::vector<SocketWrapper*> sockets;

    std::vector<SocketConnectCallback> connectCallbacks;

    void handleAccept(SocketWrapper* newSocket, const asio::error_code& err);
};


#endif //TETRIS_SERVER_H
