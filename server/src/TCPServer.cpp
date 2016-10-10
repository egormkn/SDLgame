//
// Created by Vlad on 09.10.2016.
//

#include <SDL_net.h>
#include <fstream>
#include "TCPServer.h"

TCPServer::TCPServer(int32_t port_) {
    serverSocketSet = SDLNet_SocketSet((int32_t) 10);
    port = port_;
    std::cout << "SERVER create with port :" << port << "\n" <<
              "==================================================\n";
}

TCPServer::~TCPServer() {
    SDLNet_FreeSocketSet(serverSocketSet);
}

bool TCPServer::setupPort() {
    std::cout << "Resolving port\n";

    int result = SDLNet_ResolveHost(&ipAddress, nullptr, port);

    if (result == -1) {
        std::cout << "ERROR: Failed to resolve port!\n\n" <<
                  "==================================================\n";
        return false;
    }

    std::cout << "Port resolved\n" <<
              "==================================================\n";
    return true;
}

bool TCPServer::openPortForListening() {
    std::cout << "Opening connection\n";

    tcpSocket = SDLNet_TCP_Open(&ipAddress);

    if (tcpSocket == nullptr) {
        std::cout << "ERROR: Filed opening port for listening\n" <<
                  "==================================================\n";
        return false;
    }

    std::cout << "Port opened for listening\n" <<
              "==================================================\n";
    return true;
}

bool TCPServer::canAcceptConnection() {
    if (tcpSocket == nullptr) {
        std::cout << "ERROR: TCP Socket is null\n" <<
                  "==================================================\n";
        return false;
    }

    int i = SDLNet_SocketReady(tcpSocket);
    std::cout << "Activity : " << i << std::endl;
    bool canAccept = i != 0;

    if (canAccept) {
        std::cout << "Can accept connection\n" <<
                  "==================================================\n";
    } else {
        std::cout << "Can't accept connection\n" <<
                  "==================================================\n";
    }
    return canAccept;
}

void TCPServer::acceptConnection() {
    TCPsocket newSocket = SDLNet_TCP_Accept(tcpSocket);

    if (newSocket == nullptr) {
        std::cout << "ERROR: Cannot accept TCP connection : " << SDLNet_GetError() << "\n" <<
                  "==================================================\n";
        return;
    }


    std::unique_ptr<TCPClient> connection(new TCPClient(newSocket));
    SDLNet_TCP_AddSocket(serverSocketSet, newSocket);
    serverConnections.push_back(std::move(connection));

    //sendFile(newSocket, "xml_for_server.xml");

    std::cout << "Connection accepted\n" <<
              "==================================================\n";
}

bool TCPServer::checkForActivity() {
    return SDLNet_CheckSockets(serverSocketSet, 3) != 0;
}

void TCPServer::sendFile(TCPsocket socket, std::string file_dir) {
    std::cout << "Starting sending file!\n";
    int buffer_size = 2048;
    char *buffer = new char[buffer_size];
    std::ifstream in(file_dir);
    int bytes_send = 0;
    int messageSize = 0;
    std::cout << "Sending file:" << file_dir << "\n";
    while (in.read(buffer, buffer_size)) {
        messageSize = in.gcount();
        bytes_send = SDLNet_TCP_Send(socket, buffer, messageSize);

        std::cout << "Trying to send " << messageSize << "\tsent : " << bytes_send << "\n";
        if (bytes_send < messageSize) {
            std::cout << "\tSend failed : " << SDLNet_GetError() << "\n";
        }
    }
    std::cout << "Information successfully sent!\n" <<
              "==================================================\n";
    delete[] buffer;
    in.close();
}