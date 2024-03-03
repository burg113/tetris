//
// Created by Burg on 08.12.2023.
//

#include "Networking.h"
#include <thread>

void onReadServer(SocketWrapper* socket, const std::string &data) {
    std::cout << "Socket " << socket->getId() << " received input." << std::endl;
    std::cout << "Data: " << data.size() << " " << data << std::endl;
    std::vector<short> vec;
    BinaryStream streamIn(data);
    streamIn >> vec;
    int sum = 0;
    for(short x : vec) sum += x;
    BinaryStream streamOut;
    streamOut << sum;
    socket->send(streamOut.getData());
    std::cout << "Sent" << std::endl;
}

void onKillServer(SocketWrapper* socket) {
    std::cout << "Socket " << socket->getId() << " was killed." << std::endl;
}

void onConnectServer(SocketWrapper* socket) {
    std::cout << "Socket " << socket->getId() << " connected!" << std::endl;
    socket->addReadCallback(onReadServer);
    socket->addKillCallback(onKillServer);
}

void onReadClient(SocketWrapper* socket, const std::string &data) {
    int sum = 0;
    BinaryStream stream(data);
    stream >> sum;
    std::cout << "Client received data: " << sum << std::endl;
}

void startClient() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    asio::io_service ioService;
    SocketWrapper clientSocket(ioService);
    clientSocket.addReadCallback(onReadClient);
    clientSocket.connectToIp(asio::ip::address::from_string("127.0.0.1"), 2024);
    for(int it = 0; it < 5; it++){
        std::vector<short> vec;
        for(short s = 0; s < 199; s++) vec.push_back(s);
        BinaryStream stream;
        stream << vec;
        clientSocket.send(stream.getData());
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    clientSocket.startListening();
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ioService.poll();
    }
}

void testNetworking(){
    std::cerr << "Test networking" << std::endl;
    std::thread clientThread(startClient);
    try {
        asio::io_service ioService;

        ServerHelper s(ioService, 2024);
        s.addConnectCallback(onConnectServer);
        s.startAccepting();

        while(true){
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            ioService.poll();
        }
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cerr << "Test networking done" << std::endl;
}