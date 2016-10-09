//
// Created by Vlad on 09.10.2016.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "MyTCPClient.h"
#include "MyTCPServer.h"

TCPClient* client;
TCPServer* server;

std::string ip;
int port;
bool isServer;

void GetSettingsFromUser();

int main(int argv, char **args) {
    GetSettingsFromUser();
    if (isServer) {
        server = new TCPServer(port);
    } else {
        client = new TCPClient(ip, port);
    }


    if (isServer) {
        while(true) {
            if (server->canAcceptConnection())
                server->AcceptConnection();
            SDL_Delay(1000);
        }
    } else {
        std::cout << "trying to connect\n";
        client->setupIpAddress();
        client->openConnectionToServer();
        std::cout << "If you want to close put n\n";
        char state;
        std::cin >> state;
        client->Close();
        return 0;
    }
}

void GetSettingsFromUser() {
    std::cout << "Is this a server? ( Type \'y\' for server, otherwise \'n\' and hit enter) : \n";
    char isServerInput = 'n';
    std::cin >> isServerInput;
    isServer = (isServerInput == 'y');

    if (!isServerInput) {
        std::cout << "Enter remote IP ( 127.0.0.1  for local connections ) : \n";
        std::cin >> ip;
    }

    std::cout << "Enter the port number ( this needs to be the same for server and client ) : \n";
    std::cin >> port;
}