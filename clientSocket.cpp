#include "clientSocket.h"
#include <arpa/inet.h> 
#include <unistd.h> 
#include <cstring> 
#include <iostream> 

clientSocket::clientSocket(const char* ipSrv, int portSrv) {
    this->ip = std::string(ipSrv);
    this->port = portSrv;
}

clientSocket::clientSocket() {
    this->ip = "1.1.1.1";
    this->port = 80;
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
    adresse.sin_addr.s_addr = inet_addr(this->ip.c_str());
    adresse.sin_port = htons(this->port);

    int lg_client = sizeof(adresse);

    resultat = connect(sockfd_client, (struct sockaddr*)&adresse, lg_client);
    if (resultat == -1) {
        perror("Erreur lors de la connexion au serveur");
        close(sockfd_client);
        return false;
    }

    write(sockfd_client, &ch, 1);

    read(sockfd_client, &ch, 1);

    if (ch == '1') {
        close(sockfd_client);
        return true;
    } else {
        close(sockfd_client);
        return false;
    }
}
