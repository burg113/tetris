//
// Created by lucas on 09.12.2023.
//

#include "spdlog/spdlog.h"
#include "ServerHelper.h"

ServerHelper::ServerHelper(asio::io_service &ioService, short port) : ioService(ioService),
                                                                      acceptor(tcp::acceptor(ioService, tcp::endpoint(tcp::v4(), port))) {

}

void ServerHelper::addConnectCallback(const SocketConnectCallback &callback) {
    connectCallbacks.push_back(callback);
}

asio::io_service& ServerHelper::getIoService(){
    return ioService;
}

void ServerHelper::startAccepting() {
    auto *newSocket = new SocketWrapper(ioService);
    sockets.push_back(newSocket);
    acceptor.async_accept(newSocket->getSocket(), [this, newSocket](const asio::error_code& e){this->handleAccept(newSocket, e);});
}

void ServerHelper::handleAccept(SocketWrapper *newSocket, const asio::error_code &err) {
    if (err) {
        SPDLOG_ERROR("Socket acceptor received error: {} {}", err.value(), err.message());
        newSocket->close();
        return;
    }
    newSocket->startListening();
    for(const SocketConnectCallback &callback : connectCallbacks) {
        callback(newSocket);
    }
    startAccepting();
}



