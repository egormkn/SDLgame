//
// Created by Vlad on 09.10.2016.
//

#include "TCPClient.h"

TCPClient::TCPClient(const std::string &ip_, int32_t port_) {
    ip = ip_;
    port = port_;
    std::cout << "CLIENT created\n\n";
}

TCPClient::TCPClient(TCPsocket newSocket) {
    tcpSocket = newSocket;
    std::cout << "CLIENT created\n\n";
}

bool TCPClient::setupIpAddress() {
    std::cout << "Trying to initialize\n";
    int result = SDLNet_ResolveHost(&ipAddress, ip.c_str(), port);

    if (result == -1) {
        std::cout << "Failed to resolve host\n\n";
        return false;
    }

    std::cout << "Host resolved\n\n";
    return true;
}

bool TCPClient::openConnectionToServer() {
    std::cout << "Opening connection\n";

    tcpSocket = SDLNet_TCP_Open(&ipAddress);

    if (tcpSocket == nullptr) {
        std::cout << "Failed to open connection:\n" << SDLNet_GetError() << "\n\n";
        return false;
    }

    isConnected = true;
    std::cout << "Connection opened!\n\n";

    return true;
}

void TCPClient::Close() {
    isConnected = false;
    std::cout << "Closing connection\n\n";
    SDLNet_TCP_Close(tcpSocket);
}

bool TCPClient::IsConnected() {
    return isConnected;
}
