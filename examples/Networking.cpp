//
// Created by Burg on 08.12.2023.
//

#include "Networking.h"
#include <thread>

void onReadServer(SocketWrapper* socket, uint8_t header, const std::string &data) {
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

void onKillServer(SocketWrapper* socket) {
    std::cout << "Socket " << socket->getId() << " was killed." << std::endl;
}

void onConnectServer(SocketWrapper* socket) {
    std::cout << "Socket " << socket->getId() << " connected!" << std::endl;
    socket->addReadCallback(onReadServer);
    socket->addKillCallback(onKillServer);
}

void onReadClient(SocketWrapper* socket, uint8_t header, const std::string &data) {
    int sum = 0;
    std::stringstream strstr(data);
    strstr >> binr(sum);
    std::cout << "Client received data: " << sum << std::endl;
}

void startClient() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    asio::io_service ioService;
    SocketWrapper clientSocket(ioService);
    clientSocket.addReadCallback(onReadClient);
    clientSocket.connectToIp(asio::ip::address::from_string("127.0.0.1"), 13);
    std::vector<short> vec;
    for(short s = 0; s < 200; s++) vec.push_back(s);
    std::stringstream strstr;
    strstr << binw(vec);
    clientSocket.send(0, strstr.str());
    clientSocket.startListening();
    ioService.run();
}

void testNetworking(){
    std::cerr << "Test networking" << std::endl;
    std::thread clientThread(startClient);
    try {
        asio::io_service ioService;

        Server s(ioService, 13);
        s.addConnectCallback(onConnectServer);
        s.startAccepting();

        ioService.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cerr << "Test networking done" << std::endl;
}