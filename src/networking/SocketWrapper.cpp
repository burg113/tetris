//
// Created by lucas on 09.12.2023.
//

#include "SocketWrapper.h"

int SocketWrapper::numSessions = 0;

SocketWrapper::SocketWrapper(asio::io_service &ioService)
        : socket(ioService), id(numSessions++) {
    len = 0;
    alive = true;
}

void SocketWrapper::addReadCallback(const SocketReadCallback &callback) {
    readCallbacks.push_back(callback);
}

void SocketWrapper::addKillCallback(const SocketKillCallback &callback) {
    killCallbacks.push_back(callback);
}

void SocketWrapper::connectToIp(const asio::ip::address &ip, short port) {
    socket.connect(tcp::endpoint(ip, port));
}

void SocketWrapper::startListening() {
    std::cout << "Socket " << id << " started listening." << std::endl;
    doReadSome();
}

void SocketWrapper::sendRaw(const std::string &str) {
    asio::write(socket, asio::buffer(str));
}

void SocketWrapper::send(const std::string &str) {
    std::string msg(4, '0');
    uint32_t x = str.size();
    for(int i = 3; i >= 0; i--){
        msg[i] = static_cast<char>(x & 0xff);
        x /= 256;
    }
    msg += str;
    sendRaw(msg);
}

tcp::socket& SocketWrapper::getSocket() {
    return socket;
}

int SocketWrapper::getId() {
    return id;
}

bool SocketWrapper::isAlive() {
    return alive;
}

void SocketWrapper::kill() {
    if(alive) {
        alive = false;
        socket.shutdown(asio::socket_base::shutdown_type::shutdown_both);
        socket.close();
        for (const SocketKillCallback &callback : killCallbacks) {
            callback(this);
        }
    }
}

void SocketWrapper::handleRead(const asio::error_code &err, size_t numBytes) {
    if (err) {
        std::cerr << "Socket " << id << " received error " << err.value() << ": " << err.message() << std::endl;
        kill();
        return;
    }
//    std::cout << "Socked " << id << " received " << (int)buf[0] << std::endl;
    uint32_t oldLen = data.size();
    data += std::string(buf, buf + numBytes);
    if (oldLen < 4 && 4 <= data.size()) {
        for (int i = 0; i < 4; i++) {
            len *= 256;
            len += static_cast<uint8_t>(data[i]);
        }
        len += 4;
    }
    if(data.size() >= 4) {
        if (data.size() > len) {
            std::cerr << "WARNING: Socket " << id << " received message of length " << data.size() << " but header said that it has length " << len << std::endl;
        }
        if (data.size() >= len) {
            // handle message
            for(const SocketReadCallback &callback : readCallbacks) {
                callback(this, data.substr(4));
            }
            // reset state
            len = 0;
            data.clear();
        }
    }
    if(alive) {
        doReadSome();
    }
}



void SocketWrapper::doReadSome() {
    socket.async_read_some(asio::buffer(buf, BUFFER_SIZE),
                           [this](const asio::error_code &e, size_t nb) {this->handleRead(e, nb);});
}