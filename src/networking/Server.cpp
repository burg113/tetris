//
// Created by lucas on 09.12.2023.
//

#include "Server.h"

#include <utility>

Server::Server(asio::io_service &ioService, short port, SocketConnectCallback socketConnectCallback,
               SocketReadCallback socketReadCallback) : ioService(ioService),
                                                        acceptor(tcp::acceptor(ioService, tcp::endpoint(tcp::v4(), port))),
                                                        socketConnectCallback(std::move(socketConnectCallback)), socketReadCallback(std::move(socketReadCallback)) {

}

void Server::startAccepting() {
    auto *newSocket = new SocketWrapper(ioService, socketReadCallback);
    sockets.push_back(newSocket);
    acceptor.async_accept(newSocket->getSocket(), [this, newSocket](const asio::error_code& e){this->handleAccept(newSocket, e);});
}

void Server::handleAccept(SocketWrapper *newSocket, const asio::error_code &err) {
    if (err) {
        std::cerr << "Socket acceptor received error " << err.value() << ": " << err.message() << std::endl;
        newSocket->kill();
        return;
    }
    newSocket->startListening();
    socketConnectCallback(newSocket);
    startAccepting();
}


