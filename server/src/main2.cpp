//
// Created by Vlad on 09.10.2016.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "TCPClient.h"
#include "TCPServer.h"

TCPClient* client;
TCPServer* server;

std::string ip = "";
int port;
bool isServer;

void GetSettingsFromUser();

int main(int argv, char **args) {
    if(!SDLNet_Init() == -1) {
        std::cout << "Can't init\n";
        std::cout << SDLNet_GetError() << "\n";
    }
    GetSettingsFromUser();
    if (isServer) {
        server = new TCPServer(port);
    } else {
        client = new TCPClient(ip, port);
    }


    if (isServer) {
        server->setupPort();
        server->OpenPortForListening();
        while(true) {
            if (server->canAcceptConnection())
                server->AcceptConnection();
            std::cout << "waiting\n";
            std::cout << "If you want to close put n\n";
            char state;
            std::cin >> state;
            if (state == 'n') {
                break;
            } else {
                continue;
            }
        }
    } else {
        client->setupIpAddress();
        while (true) {
            std::cout << "trying to connect\n";
            client->openConnectionToServer();
            std::cout << "If you want to close put n\n";
            char state;
            std::cin >> state;
            if (state == 'n') {
                break;
            } else {
                continue;
            }
        }
        client->Close();
        return 0;
    }
}

void GetSettingsFromUser() {
    std::cout << "Is this a server? ( Type \'y\' for server, otherwise \'n\' and hit enter) : \n";
    char isServerInput = 'n';
    std::cin >> isServerInput;
    isServer = (isServerInput == 'y');

    if (!isServer) {
        std::cout << "Enter remote IP ( 127.0.0.1  for local connections ) : \n";
        std::cin >> ip;
    }

    std::cout << "Enter the port number ( this needs to be the same for server and client ) : \n";
    std::cin >> port;
    std::cout << "\nport: " << port << "\n";
    std::cout << "ip: " << ip << "\n\n";
}