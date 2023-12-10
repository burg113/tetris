//
// Created by lucas on 09.12.2023.
//

#include "SocketWrapper.h"

#include <utility>

int SocketWrapper::numSessions = 0;

SocketWrapper::SocketWrapper(asio::io_service &ioService, SocketReadCallback callback)
        : socket(ioService), id(numSessions++), callback(std::move(callback)) {
    firstRead = true;
    header = 0;
    lenlen = 0;
    len = 0;
    alive = true;
}

void SocketWrapper::startListening() {
    std::cout << "Socket " << id << " started listening." << std::endl;
    doReadSome();
}

void SocketWrapper::sendRaw(const std::string &str) {
    asio::write(socket, asio::buffer(str));
}

void SocketWrapper::send(uint8_t header, const std::string &str) {
    assert(header < 64);
    std::string msg(1, ' ');
    header <<= 2;
    if(str.size() == 1) header |= 1;
    else if(1 < str.size() && str.size() <= 255){
        header |= 2;
        msg.resize(2);
        msg[1] = static_cast<char>(str.size());
    }
    else if(256 <= str.size()){
        header |= 3;
        msg.resize(5);
        uint32_t x = str.size();
        for(int i = 4; i >= 1; i--){
            msg[i] = static_cast<char>(x & 0xff);
            x /= 256;
        }
    }
    msg += str;
    msg[0] = static_cast<char>(header);
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
    }
}

void SocketWrapper::handleRead(const asio::error_code &err, size_t numBytes) {
    if (err) {
        std::cerr << "Socket " << id << " received error " << err.value() << ": " << err.message() << std::endl;
        kill();
        return;
    }
//    std::cout << "Socked " << id << " received " << std::string(buf, buf + numBytes) << std::endl;
    uint32_t oldLen = data.size();
    if (firstRead) {
        firstRead = false;
        header = static_cast<uint8_t>(buf[0]);
        uint8_t lenInfo = header & 3;
        if(lenInfo == 3) lenlen = 4;
        else if(lenInfo == 2) lenlen = 1;
        else {
            lenlen = 0;
            len = lenInfo;
        }
        data += std::string(buf + 1, buf + numBytes);
    }
    else data += std::string(buf, buf + numBytes);
    if (oldLen < lenlen && lenlen <= data.size()) {
        for (int i = lenlen-1; i >= 0; i--) {
            len *= 256;
            len += static_cast<uint8_t>(data[i]);
        }
        len += lenlen;
    }
    if(data.size() >= lenlen) {
        if (data.size() > len) {
            std::cerr << "WARNING: Socket " << id << " received message of length " << data.size() << " but header said that it has length " << len << std::endl;
        }
        if (data.size() >= len) {
            // handle message
            callback(this, header >> 2, data.substr(lenlen));
            // reset state
            firstRead = true;
            header = 0;
            lenlen = 0;
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