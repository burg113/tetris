//
// Created by lucas on 09.12.2023.
//

#pragma once

#include "SocketWrapper.h"

using SocketConnectCallback = std::function<void(SocketWrapper*)>;

class ServerHelper {
public:
    ServerHelper(asio::io_service &ioService, short port);

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


