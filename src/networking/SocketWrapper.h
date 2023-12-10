//
// Created by lucas on 09.12.2023.
//

#ifndef TETRIS_SOCKETWRAPPER_H
#define TETRIS_SOCKETWRAPPER_H

#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;


class SocketWrapper;

using SocketReadCallback = std::function<void(SocketWrapper*, uint8_t, std::string)>;

constexpr int BUFFER_SIZE = 1;

class SocketWrapper {
public:

    SocketWrapper(asio::io_service &ioService, SocketReadCallback callback);

    void startListening();

    void sendRaw(const std::string &str);

    void send(uint8_t header, const std::string &str);

    tcp::socket& getSocket();

    int getId();

    bool isAlive();

    void kill();

private:
    static int numSessions;

    tcp::socket socket;
    char buf[BUFFER_SIZE] = {};
    bool firstRead;
    uint8_t header;
    int lenlen;
    uint32_t len;
    std::string data;
    int id;
    bool alive;

    SocketReadCallback callback;

    void handleRead(const asio::error_code &err, size_t numBytes);
    void doReadSome();
};


#endif //TETRIS_SOCKETWRAPPER_H
