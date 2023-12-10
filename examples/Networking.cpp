//
// Created by Burg on 08.12.2023.
//

#include "Networking.h"
//#include <asio.hpp>
//#include <iostream>
//
//using asio::ip::tcp;
//
//constexpr int BUFFER_SIZE = 10;
//
//int numSessions = 0;
//
//struct Session{
//
//    Session(asio::io_service &io_service): socket(io_service), id(numSessions++) {}
//
//    void start() {
//        std::cout << "New session: " << id << std::endl;
//        socket.async_read_some(asio::buffer(data, BUFFER_SIZE), [this](const asio::error_code &e, size_t nb) { this->handle_read(e, nb); });
//    }
//
//    void handle_read(const asio::error_code &err, size_t numBytes) {
//        if(!err) {
//            std::string s(data, data + numBytes);
//            std::cout << "Session " << id << " received input: " << s << std::endl;
//            socket.async_read_some(asio::buffer(data, BUFFER_SIZE),[this](const asio::error_code &e, size_t nb) { this->handle_read(e, nb); });
//        }
//        else {
//            std::cout << "Session " << id << " received error " << err.value() << ": " << err.message() << std::endl;
//            delete this;
//        }
//    }
//
//    tcp::socket socket;
//    char data[BUFFER_SIZE];
//    int id;
//};
//
//struct Server{
//
//    Server(asio::io_service &io_service, short port)
//        :   io_service(io_service),
//            acceptor(tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), port))) {
//
//        Session* newSession = new Session(io_service);
//        acceptor.async_accept(newSession->socket, [this, newSession](const asio::error_code& e){this->handle_accept(newSession, e);});
//    }
//
//    void handle_accept(Session* newSession, const asio::error_code& err) {
//        if (!err) {
//            newSession->start();
//            newSession = new Session(io_service);
//            acceptor.async_accept(newSession->socket, [this, newSession](const asio::error_code& e){this->handle_accept(newSession, e);});
//        }
//        else {
//            delete newSession;
//        }
//    }
//
//    asio::io_service &io_service;
//    tcp::acceptor acceptor;
//};

void onConnect(SocketWrapper* socket) {
    std::cout << "Socket " << socket->getId() << " connected!" << std::endl;
}

void onRead(SocketWrapper* socket, uint8_t header, std::string data) {
    std::cout << "Socket " << socket->getId() << " received input." << std::endl;
    std::cout << "Header: " << (int)header << std::endl;
    std::cout << "Data: " << data << std::endl;
    std::vector<short> vec;
    std::stringstream strstr(data);
    strstr >> binr(vec);
    int sum = 0;
    for(short x : vec) sum += x;
    std::stringstream out;
    out << binw(sum);
    socket->send(0,out.str());
    socket->kill();
}

void testNetworking(){
    std::cerr << "Test networking" << std::endl;
    try {
        asio::io_service io_service;

        Server s(io_service, 13, onConnect, onRead);
        s.startAccepting();

        io_service.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cerr << "Test networking done" << std::endl;
}