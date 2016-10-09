//
// Created by Vlad on 09.10.2016.
//

#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <SDL_net.h>
#include "MyTCPClient.h"

class TCPServer {
private:
    int32_t port;
    IPaddress ipAddress;
    TCPsocket tcpSocket;
    SDLNet_SocketSet* serverSocketSet;
    std::vector<std::unique_ptr<TCPClient> > serverConnections;

public:
    TCPServer (int32_t port_) {
        serverSocketSet = new SDLNet_SocketSet((int32_t) 10);
        port_ = port_;
    }

    ~TCPServer() {
        for (int i = 0; i < serverConnections.size(); ++i) {
            SDLNet_TCP_Close(serverConnections[i]);
        }
        SDLNet_TCP_Close(tcpSocket);
    }

    bool setupPort() {
        std::cout << "Resolving port\n";

        int result = SDLNet_ResolveHost(&ipAddress, nullptr, port);

        if (result == -1) {
            std::cout << "Failed to resolve port\n\n";
            return false;
        }

        std::cout << "Port resolved\n\n";
        return true;
    }

    bool OpenPortForListening() {
        std::cout << "Opening connection\n";

        tcpSocket = SDLNet_TCP_Open(&ipAddress);

        if (tcpSocket == nullptr) {
            std::cout << "Opening port for listening\n\n";
            return false;
        }

        std::cout << "Port opened for listening\n\n";
        return true;
    }

    bool canAcceptConnection() {
        if (tcpSocket != nullptr) {
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

    void AcceptConnection() {
        if(canAcceptConnection())
            std::cout << "Server has input connection\n";

        TCPsocket newSocket = SDLNet_TCP_Accept(tcpSocket);

        if(newSocket == nullptr) {
            std::cout << "Cannot accept TCP connection : " << SDLNet_GetError() << "\n\n";
            return nullptr;
        }

        std::unique_ptr<TCPClient> connection(new TCPServer(newSocket));
        SDLNet_TCP_AddSocket(*serverSocketSet, newSocket);
        serverConnections.emplace_back(connection);

        std::cout << "Connection accepted\n\n";
    }
};
