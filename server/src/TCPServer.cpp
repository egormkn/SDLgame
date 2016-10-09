//
// Created by Vlad on 09.10.2016.
//

#include "TCPServer.h"

TCPServer::TCPServer(int32_t port_) {
    serverSocketSet = SDLNet_SocketSet((int32_t) 10);
    port_ = port_;
}

bool TCPServer::setupPort() {
    std::cout << "Resolving port\n";

    int result = SDLNet_ResolveHost(&ipAddress, nullptr, port);

    if (result == -1) {
        std::cout << "Failed to resolve port\n\n";
        return false;
    }

    std::cout << "Port resolved\n\n";
    return true;
}

bool TCPServer::OpenPortForListening() {
    std::cout << "Opening connection\n";

    tcpSocket = SDLNet_TCP_Open(&ipAddress);

    if (tcpSocket == nullptr) {
        std::cout << "Filed opening port for listening\n\n";
        return false;
    }

    std::cout << "Port opened for listening\n\n";
    return true;
}

bool TCPServer::canAcceptConnection() {
    if (tcpSocket == nullptr) {
        std::cout << "TCP Socket is null\n\n";
        return false;
    }

    bool canAccept = SDLNet_SocketReady(tcpSocket) != 0;

    if (canAccept) {
        std::cout << "Can accept connection\n\n";
    } else {
        std::cout << "Can't accept connection\n\n";
    }
    return canAccept;
}

void TCPServer::AcceptConnection() {
    TCPsocket newSocket = SDLNet_TCP_Accept(tcpSocket);

    if(newSocket == nullptr) {
        std::cout << "Cannot accept TCP connection : " << SDLNet_GetError() << "\n\n";
    }

    std::unique_ptr<TCPClient> connection(new TCPClient(newSocket));
    SDLNet_TCP_AddSocket(serverSocketSet, newSocket);
    serverConnections.push_back(std::move(connection));

    std::cout << "Connection accepted\n\n";
}