#include "clientSocket.h"
#include <arpa/inet.h> // for inet_addr
#include <unistd.h> // for close, write, read
#include <iostream> // for perror

clientSocket::clientSocket(const char* ipSrv, int portSrv) 
    : ip(ipSrv), port(portSrv) {
}

clientSocket::clientSocket() 
    : ip("1.1.1.1"), port(80) {
}

bool clientSocket::envoyerMessage(const char* msg) {
    int sockfd_client;
    struct sockaddr_in adresse;
    int resultat;

    sockfd_client = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_client == -1) {
        perror("Erreur lors de la création du socket");
        return false;
    }

    adresse.sin_family = AF_INET;
    adresse.sin_addr.s_addr = inet_addr(ip.c_str());
    adresse.sin_port = htons(port);

    int lg_client = sizeof(adresse);

    resultat = connect(sockfd_client, (struct sockaddr*)&adresse, lg_client);
    if (resultat == -1) {
        perror("Erreur lors de la connexion au serveur");
        close(sockfd_client);
        return false;
    }

    write(sockfd_client, &msg, strlen(msg),0);
    read(sockfd_client, &ch, 1);

    if (ch == 1) {
        close(sockfd_client);
        return true;
    } else {
        close(sockfd_client);
        return false;
    }
}
