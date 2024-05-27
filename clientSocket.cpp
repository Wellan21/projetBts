#include "clientSocket.h"
#include <arpa/inet.h> // for inet_addr
#include <unistd.h> // for close, write, read
#include <iostream> // for perror
#include <cstring> // for strlen, memset

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

    // Create socket
    sockfd_client = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_client == -1) {
        perror("Erreur lors de la création du socket");
        return false;
    }

    // Set up the server address
    memset(&adresse, 0, sizeof(adresse));
    adresse.sin_family = AF_INET;
    adresse.sin_addr.s_addr = inet_addr(ip.c_str());
    adresse.sin_port = htons(port);

    // Connect to the server
    resultat = connect(sockfd_client, (struct sockaddr*)&adresse, sizeof(adresse));
    if (resultat == -1) {
        perror("Erreur lors de la connexion au serveur");
        close(sockfd_client);
        return false;
    }

    // Send the message
    size_t msg_length = strlen(msg);
    ssize_t bytes_sent = write(sockfd_client, msg, msg_length);
    if (bytes_sent == -1 || static_cast<size_t>(bytes_sent) != msg_length) {
        perror("Erreur lors de l'envoi du message");
        close(sockfd_client);
        return false;
    }

    // Receive response from the server
    char response;
    ssize_t bytes_received = read(sockfd_client, &response, 1);
    if (bytes_received == -1) {
        perror("Erreur lors de la réception de la réponse");
        close(sockfd_client);
        return false;
    }

    // Close the socket
    close(sockfd_client);

    // Check the response
    return response == '1';
}

