//
// Created by Vlad on 09.10.2016.
//

#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <SDL_net.h>
#include "TCPClient.h"

class TCPServer {
private:
    int32_t port;
    IPaddress ipAddress;
    TCPsocket tcpSocket;
    SDLNet_SocketSet serverSocketSet;
    std::vector<std::unique_ptr<TCPClient> > serverConnections;

public:
    TCPServer (int32_t port_);

    ~TCPServer();

    bool setupPort();

    bool openPortForListening();

    bool canAcceptConnection();

    void acceptConnection();

    bool checkForActivity();

    void sendFile(TCPsocket socket, std::string file_dir);
};
