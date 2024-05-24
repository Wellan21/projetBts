// clientSocket.h
// Marcoux Tristan Avril 2024
#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

class clientSocket {
private:
    std::string ip;
    int port;

public:
    clientSocket(const char* ipSrv, int portSrv); // Constructor with parameters
    clientSocket(); // Default constructor

    bool envoyerMessage(const char* msg); // Member function
};

#endif // CLIENTSOCKET_H
