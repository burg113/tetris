//
// Created by lucas on 09.12.2023.
//

#include "Server.h"

#include <utility>

Server::Server(asio::io_service &ioService, short port) : ioService(ioService),
                                                        acceptor(tcp::acceptor(ioService, tcp::endpoint(tcp::v4(), port))) {

}

void Server::addConnectCallback(const SocketConnectCallback &callback) {
    connectCallbacks.push_back(callback);
}


void Server::startAccepting() {
    auto *newSocket = new SocketWrapper(ioService);
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
    for(const SocketConnectCallback &callback : connectCallbacks) {
        callback(newSocket);
    }
    startAccepting();
}



