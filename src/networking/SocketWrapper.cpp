//
// Created by lucas on 09.12.2023.
//

#include "spdlog/spdlog.h"
#include "SocketWrapper.h"

int SocketWrapper::numSessions = 0;

SocketWrapper::SocketWrapper(asio::io_service &ioService)
        : socket(ioService), id(numSessions++), ioService(ioService) {
    len = 0;
}

void SocketWrapper::addReadCallback(const SocketReadCallback &callback) {
    readCallbacks.push_back(callback);
}

void SocketWrapper::addCloseCallback(const SocketCloseCallback &callback) {
    closeCallbacks.push_back(callback);
}

void SocketWrapper::connectToIp(const asio::ip::address &ip, short port) {
    socket.connect(tcp::endpoint(ip, port));
}

void SocketWrapper::connect(const std::string &host, const std::string &service) {
    tcp::resolver resolver(ioService);
    tcp::resolver::results_type endpoint = resolver.resolve(host, service);
    socket.connect(endpoint->endpoint());
}

void SocketWrapper::startListening() {
    SPDLOG_INFO("Socket (id {}) started listening.", id);
    doReadSome();
}

void SocketWrapper::sendRaw(const std::string &str) {
    if(socket.is_open()) {
        asio::async_write(socket, asio::buffer(str), [this](const asio::error_code &e, std::size_t numBytes){
            if(e){
                SPDLOG_ERROR("Couldn't write to socket (id {}): {} {}", this->id, e.value(), e.message());
            }
        });
    }
    else{
        SPDLOG_WARN("Attempt to write to closed socket (id {})", getId());
    }
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

asio::io_service& SocketWrapper::getIoService() {
    return ioService;
}

int SocketWrapper::getId() {
    return id;
}

void SocketWrapper::close() {
    if(socket.is_open()) {
        asio::error_code ec;
        socket.shutdown(asio::socket_base::shutdown_type::shutdown_both, ec);
        if(ec){
            SPDLOG_ERROR("error shutdown of socket (id {}): {} {}", id, ec.value(), ec.message());
        }
        socket.close();
        for (const SocketCloseCallback &callback : closeCallbacks) {
            callback(this);
        }
    }
}

void SocketWrapper::appendData(const std::string &newData) {
    uint32_t oldLen = data.size();
    data += newData;
    if (oldLen < 4 && 4 <= data.size()) {
        for (int i = 0; i < 4; i++) {
            len *= 256;
            len += static_cast<uint8_t>(data[i]);
        }
        len += 4;
    }
    if(data.size() >= 4){
        if (data.size() >= len) {
            for(const SocketReadCallback &callback : readCallbacks) {
                callback(this, data.substr(4, len-4));
            }
            std::string startOfNextMessage = data.substr(len);
            len = 0;
            data.clear();
            if(!startOfNextMessage.empty()) appendData(startOfNextMessage);
        }
    }
}

void SocketWrapper::handleRead(const asio::error_code &err, size_t numBytes) {
    if (err) {
        SPDLOG_ERROR("Error while reading from socket (id {}): {} {}", id, err.value(), err.message());
        close();
        return;
    }
    std::string newData = std::string(buf, buf + numBytes);
    for(int i = 0; i < newData.size(); i += APPEND_SIZE){
        appendData(newData.substr(i, APPEND_SIZE));
    }
    if(socket.is_open()) {
        doReadSome();
    }
}

void SocketWrapper::doReadSome() {
    socket.async_read_some(asio::buffer(buf, BUFFER_SIZE),
                           [this](const asio::error_code &e, size_t nb) {this->handleRead(e, nb);});
}


