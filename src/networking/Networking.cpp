//
// Created by Burg on 08.12.2023.
//

#include "Networking.h"
#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

constexpr int BUFFER_SIZE = 256;

int numSessions = 0;

struct Session{

    Session(asio::io_service &io_service): socket(io_service), id(numSessions++) {}

    void start() {
        socket.async_read_some(asio::buffer(data, BUFFER_SIZE), [this](const asio::error_code &e, size_t nb) { this->handle_read(e, nb); });
    }

    void handle_read(const asio::error_code &err, size_t numBytes) {
        if(!err) {
            std::string s(data, data + numBytes);
            std::cout << "Session " << id << " received input: " << s << std::endl;
            socket.async_read_some(asio::buffer(data, BUFFER_SIZE),[this](const asio::error_code &e, size_t nb) { this->handle_read(e, nb); });
        }
        else {
            std::cout << "Session " << id << " received error " << err.value() << ": " << err.message() << std::endl;
            delete this;
        }
    }

    tcp::socket socket;
    char data[BUFFER_SIZE];
    int id;
};

struct Server{

    Server(asio::io_service &io_service, short port)
        :   io_service(io_service),
            acceptor(tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), port))) {

        Session* newSession = new Session(io_service);
        acceptor.async_accept(newSession->socket, [this, newSession](const asio::error_code& e){this->handle_accept(newSession, e);});
    }

    void handle_accept(Session* newSession, const asio::error_code& err) {
        if (!err) {
            newSession->start();
            newSession = new Session(io_service);
            acceptor.async_accept(newSession->socket, [this, newSession](const asio::error_code& e){this->handle_accept(newSession, e);});
        }
        else {
            delete newSession;
        }
    }

    asio::io_service &io_service;
    tcp::acceptor acceptor;
};

void testNetworking(){
    try {
        asio::io_service io_service;
        Server s(io_service, 13);
        io_service.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}