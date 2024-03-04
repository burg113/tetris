//
// Created by lucas on 09.12.2023.
//

#pragma once

#ifdef _WIN32
    #include <winsock2.h>
#endif

#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;


class SocketWrapper;

using SocketConnectCallback = std::function<void(SocketWrapper*)>;
using SocketReadCallback = std::function<void(SocketWrapper*, const std::string&)>;
using SocketCloseCallback = std::function<void(SocketWrapper*)>;

class SocketWrapper {
public:

    explicit SocketWrapper(asio::io_service &ioService);

    void addReadCallback(const SocketReadCallback& callback);

    void addCloseCallback(const SocketCloseCallback& callback);

    void connectToIp(const asio::ip::address& ip, short port);

    // service = port
    void connect(const std::string& host, const std::string &service);

    void startListening();

    void sendRaw(const std::string &str);

    void send(const std::string &str);

    tcp::socket& getSocket();

    asio::io_service& getIoService();

    int getId();

    void close();

private:
    // The maximum amount of data which can be received at once
    static constexpr int BUFFER_SIZE = 512;
    // The number of bytes appended to the data buffer before we check if
    static constexpr int APPEND_SIZE = 8;

    static int numSessions;

    asio::io_service &ioService;

    tcp::socket socket;
    char buf[BUFFER_SIZE] = {};
    uint32_t len;
    std::string data;
    int id;

    std::vector<SocketReadCallback> readCallbacks;
    std::vector<SocketCloseCallback> closeCallbacks;

    void appendData(const std::string &newData);
    void handleRead(const asio::error_code &err, size_t numBytes);
    void doReadSome();
};


