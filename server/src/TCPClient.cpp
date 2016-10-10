//
// Created by Vlad on 09.10.2016.
//

#include "TCPClient.h"
#include <iostream>
#include <ostream>
#include <memory>
#include <fstream>

TCPClient::TCPClient(const std::string &ip_, int32_t port_) {
    ip = ip_;
    port = port_;
    std::cout << "CLIENT created\n" <<
            "==================================================\n";
}

TCPClient::TCPClient(TCPsocket newSocket) {
    tcpSocket = newSocket;
    std::cout << "CLIENT for server created\n" <<
              "==================================================\n";
}

bool TCPClient::setupIpAddress() {
    std::cout << "Trying to initialize\n";
    int result = SDLNet_ResolveHost(&ipAddress, ip.c_str(), port);

    if (result == -1) {
        std::cout << "ERROR: Failed to resolve host\n" <<
                  "==================================================\n";
        return false;
    }

    std::cout << "Host resolved\n" <<
              "==================================================\n";
    return true;
}

bool TCPClient::openConnectionToServer() {
    std::cout << "Opening connection\n";

    tcpSocket = SDLNet_TCP_Open(&ipAddress);

    if (tcpSocket == nullptr) {
        std::cout << "ERROR: Failed to open connection:\n" << SDLNet_GetError() << "\n" <<
                  "==================================================\n";
        return false;
    }

    isConnectedFlag = true;

    std::cout << "Connection opened!\n" <<
              "==================================================\n";

    return true;
}

void TCPClient::close() {
    isConnectedFlag = false;
    std::cout << "Closing connection\n" <<
              "==================================================\n";
    SDLNet_TCP_Close(tcpSocket);
}

bool TCPClient::isConnected() {
    return isConnectedFlag;
}

void TCPClient::readFile() {
    int buffer_size = 2048;
    char *buffer = new char[buffer_size];
    std::ofstream out("xml_for_client.xml");

    int bytes_achieved = SDLNet_TCP_Recv(tcpSocket, buffer, bufferSize);


    while (bytes_achieved > 0) {
        std::cout << "Received : " << bytes_achieved << " bytes\n";
        out.write(buffer, bytes_achieved);
        bytes_achieved = SDLNet_TCP_Recv(tcpSocket, buffer, bufferSize);
    }
    out.close();
    delete[] buffer;
    std::cout << "Information successfully achieved!\n" <<
              "==================================================\n";
}
