//
// Created by Vlad on 09.10.2016.
//

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <SDL_net.h>

class TCPClient {
private:
    bool isConnected = false;

    std::string ip;
    int32_t port;

    int32_t waitTime = 60000;
    const int bufferSize = 2048;

    // Holds the address and port to the server
    IPaddress ipAddress;
    TCPsocket tcpSocket;

public:
    TCPClient(const std::string &ip_, int32_t port_) {
        ip = ip_;
        port = port_;
        std::cout << "CLIENT created\n";
    }

    TCPClient (TCPsocket newSocket) {
        tcpSocket = newSocket;
    }

    //Initialization with current ip and port
    bool setupIpAddress() {
        std::cout << "Trying to initialize\n";

        int result = SDLNet_ResolveHost(&ipAddress, ip.c_str(), port);

        if (result == -1) {
            std::cout << "Failed to resolve host\n";
            return false;
        }

        std::cout << "Host resolved\n";
        return true;
    }

    bool openConnectionToServer() {
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

    void Close() {
        isConnected = false;
        std::cout << "Closing connection\n\n";
        SDLNet_TCP_Close(tcpSocket);
    }

    bool IsConnected() {
        return isConnected;
    }
};