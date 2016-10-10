//
// Created by Vlad on 09.10.2016.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "TCPClient.h"
#include "TCPServer.h"

TCPClient *client;
TCPServer *server;

std::string ip = "";
int port;
bool isServer;

void GetSettingsFromUser();

int main(int argv, char **args) {
    if (SDLNet_Init() == -1) {
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
        server->openPortForListening();
        if (server->canAcceptConnection())
            server->acceptConnection();
        while (true) {
            std::cout << "Choose one and hit enter : "
                      << "\n\tc -> check connections to accept"
                      << "\n\tn -> quit"
                              "\nYou choice : ";
            char state;
            std::cin >> state;
            switch (state) {
                case 'c':
                    if (server->canAcceptConnection()) {
                        server->acceptConnection();
                    }
                    break;
                case 'n':
                    SDLNet_Quit();
                    return 0;
                default:
                    continue;
            }
        }
    } else {
        client->setupIpAddress();
        std::cout << "trying to connect\n";
        client->openConnectionToServer();
        while (true) {
            std::cout << "Choose one and hit enter : "
                      << "\n\tr -> read file"
                      << "\n\tn -> quit"
                              "\nYou choice : ";
            char state;
            std::cin >> state;
            switch (state) {
                case 'n':
                    client->close();
                    SDLNet_Quit();
                    return 0;
                case 'r':
                    client->readFile();
                    break;
                default:
                    continue;
            }
        }
        client->close();
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