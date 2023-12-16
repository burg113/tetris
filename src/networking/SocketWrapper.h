//
// Created by lucas on 09.12.2023.
//

#ifndef TETRIS_SOCKETWRAPPER_H
#define TETRIS_SOCKETWRAPPER_H

#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;


class SocketWrapper;

using SocketConnectCallback = std::function<void(SocketWrapper*)>;
using SocketReadCallback = std::function<void(SocketWrapper*, const std::string&)>;
using SocketKillCallback = std::function<void(SocketWrapper*)>;

constexpr int BUFFER_SIZE = 1;

class SocketWrapper {
public:

    explicit SocketWrapper(asio::io_service &ioService);

    void addReadCallback(const SocketReadCallback& callback);

    void addKillCallback(const SocketKillCallback& callback);

    void connectToIp(const asio::ip::address& ip, short port);

    // service = port
    void connect(const std::string& host, const std::string &service);

    void startListening();

    void sendRaw(const std::string &str);

    void send(const std::string &str);

    tcp::socket& getSocket();

    asio::io_service& getIoService();

    int getId();

    bool isAlive();

    void kill();

private:
    static int numSessions;

    asio::io_service &ioService;

    tcp::socket socket;
    char buf[BUFFER_SIZE] = {};
    uint32_t len;
    std::string data;
    int id;
    bool alive;

    std::vector<SocketReadCallback> readCallbacks;
    std::vector<SocketKillCallback> killCallbacks;

    void handleRead(const asio::error_code &err, size_t numBytes);
    void doReadSome();
};


#endif //TETRIS_SOCKETWRAPPER_H
