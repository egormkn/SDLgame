//
// Created by Vlad on 09.10.2016.
//


#include <SDL_net.h>
#include <fstream>
#include "TCPServer.h"

int TCPServer::fileSize(const char *filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return (int) in.tellg();

}

TCPServer::TCPServer(int32_t port_) {
    clientSocketSet = SDLNet_AllocSocketSet(10);
    port = port_;
    std::cout << "SERVER create with port :" << port << "\n" <<
              "==================================================\n";
}

TCPServer::~TCPServer() {
    SDLNet_FreeSocketSet(clientSocketSet);
}

bool TCPServer::setupPort() {
    std::cout << "Resolving port\n";

    int result = SDLNet_ResolveHost(&ipAddress, nullptr, port);
    std::cout << ipAddress.port << '\n';

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
    //SDLNet_TCP_AddSocket(clientSocketSet, tcpSocket);

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
    //int j = SDLNet_CheckSockets(clientSocketSet, 0);
    //std::cout << j << '\n';
    int i = SDLNet_SocketReady(tcpSocket);
    std::cout << "Activity : " << i << std::endl;
    bool canAccept = (i != 0);

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
        //std::cout << "ERROR: Cannot accept TCP connection : " << SDLNet_GetError() << "\n" <<
        //          "==================================================\n";
        return;
    }

    std::unique_ptr<TCPClient> connection(new TCPClient(newSocket));
    SDLNet_TCP_AddSocket(clientSocketSet, newSocket);
    clientConnections.push_back(std::move(connection));

    //sendFile(newSocket, "xml_for_server.xml");

    std::cout << "Connection accepted\n" <<
              "==================================================\n";
}

bool TCPServer::checkForRequests() {
    return SDLNet_CheckSockets(clientSocketSet, 3) != 0;
}

void TCPServer::getInformation() {
    std::cout << "Information about server:" <<
              "\nport: " << port <<
              "\nNumber of clients: " << clientConnections.size() <<
              "\n==================================================\n";
}

void TCPServer::sendFile(int number, std::string file_dir) {
    if (number >= clientConnections.size()) {
        std::cout << "ERROR: No client with such number: " << number << '\n' <<
                  "==================================================\n";
    }
    TCPsocket socket = clientConnections[number]->getSocket();
    int buffer_size = 1024 * 128;

    int32_t size = fileSize(file_dir.c_str());
    SDLNet_TCP_Send(socket, &size, 4);
    std::cout << "summary size of packets: " << size << '\n';

    std::cout << "Starting sending file!\n";
    char *buffer = new char[buffer_size];
    std::ifstream in;
    in.open(file_dir.c_str(), std::ifstream::binary);
    int bytes_send = 0;
    int messageSize = 0;
    std::cout << "Sending file:" << file_dir << "\n";
    while (in) {
        in.read(buffer, buffer_size);
        messageSize = in.gcount();
        bytes_send = SDLNet_TCP_Send(socket, buffer, messageSize);

        std::cout << "Tried to send " << messageSize << "\tsent : " << bytes_send << "\n";
        if (bytes_send < messageSize) {
            std::cout << "\tSend failed : " << SDLNet_GetError() << "\n";
            break;
        }
    }
    std::cout << "Information successfully sent!\n" <<
              "==================================================\n";
    delete[] buffer;
    in.close();
}

     

void TCPServer::answerRequests() {
    int activityCounter = SDLNet_CheckSockets(clientSocketSet, 0);
    if (activityCounter == 0) {
        std::cout << "There is no requests for this server\n" <<
                  "==================================================\n";
        return;
    }
    int i = 0;
    int32_t code_request = 0;
    TCPsocket socket;
    while (activityCounter > 0) {
        socket = clientConnections[i]->getSocket();
        if (!SDLNet_SocketReady(socket)) {
            ++i;
            continue;
        }
        --activityCounter;
        SDLNet_TCP_Recv(socket, &code_request, 4);

        switch (code_request) {
            case 1:
                std::cout << "Sending information about server to " << i << " client\n";
                sendFile(i, "info.xml");
                break;
            case 2:
                std::cout << i << "client wants to join the game" <<
                          "==================================================\n";
                break;
            default:
                std::cout << "ERROR: unknown request code received from" << i << "client\n" <<
                          "==================================================\n";
        }
        ++i;
        if (activityCounter == 0) {
            activityCounter = SDLNet_CheckSockets(clientSocketSet, 0);
            i = 0;
        }
    }
}