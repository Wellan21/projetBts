// main.cpp 
// Marcoux Tristan Avril 2024 
#include "cmodelisteur.h" 
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <chrono>

// Definition des pins Raspberry  
#define	STEEP 12 // MOSI 
#define	DIR	11 // CE1
#define CAPTEUR 17
#define DB "/www/BDD_demonstrateur.db" //Chemin d'accès BDD 
#define PASRASP 512 //nombre de pas max Raspberry 
#define PASESP  512 // Nombre de pas max esp32 
#define IP "192.168.1.2" // ip ESP32
#define PORT "5546" // port serveur ESP32 

using namespace std;


int main() {
    /* Déclaration des variables */
    char buffer[50];

    int sockfd_ecoute, sockfd_service;
    int lg_ecoute, lg_service;
    struct sockaddr_in adresse_serveur;
    struct sockaddr_in adresse_client;
    char nbImgChar[3];
    char idPieceChar[3];
    int nbImgInt = 0;
    char date [20]; 
    int resultat; 
    Cmodelisateur modelisateur(CAPTEUR,STEEP, DIR,DB,PASRASP, PASESP, IP , PORT) ; 
    
    /* Création du socket pour le serveur */
    sockfd_ecoute = socket(AF_INET, SOCK_STREAM, 0);

    /* Nommage de la socket serveur de type réseau */
    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_addr.s_addr = htonl(INADDR_ANY); // Utilisation de htonl pour l'adresse IP
    adresse_serveur.sin_port = htons(9734); // Utilisation de htons pour le numéro de port
    lg_ecoute = sizeof(adresse_serveur);

    /* Attachement du socket serveur à une adresse */
    bind(sockfd_ecoute, (struct sockaddr*)&adresse_serveur, lg_ecoute);

    /* Création d'une file d'attente de connexion */
    listen(sockfd_ecoute, 5);

    /* Attente d'un client */
    while (1) {
        try {
            /* Affichage d'un message d'accueil */
            std::cout << "Serveur 1 en attente" << std::endl;

            /* Acceptation de la connexion */
            lg_service = sizeof(adresse_client);
            sockfd_service = accept(sockfd_ecoute, (struct sockaddr*)&adresse_client, (socklen_t*)&lg_service);

            /* Lecture du client */
            int nbre_octets_rec = recv(sockfd_service, buffer, sizeof(buffer) - 1, 0);

            if (nbre_octets_rec > 0) {
                buffer[nbre_octets_rec] = '\0'; // Ajout du caractère de fin de chaîne
                std::cout << "Chaîne reçue du client : " << buffer << std::endl;

                if (strcmp(buffer, "!init") == 0) {
                    modelisateur.init(); 
                }
                else {
                    char *token = strtok(buffer, ".");
                    
                    if (token != nullptr) {
                        char testeur[50];
                        strcpy(testeur, token);

                        if(strcmp(testeur, "!camp") == 0) {
                            token = strtok(nullptr, ".");
                            
                            if (token != nullptr) {
                                strcpy(nbImgChar, token);
                                token = strtok(nullptr, ".");
                                
                                if (token != nullptr) {
                                    strcpy(idPieceChar, token);
                                    std::cout << nbImgChar << std::endl;
                                    std::cout << idPieceChar << std::endl;
                                    try {   // Convertit  nbImgChar vers  int
                                         nbImgInt = std::stoi(nbImgChar);
                                    }
                                    catch () {
                                         nbImgInt=50; 
                                         
                                    }
                                    if (token  !nullptr){
                                        strcpy(date, token);
                                         resultat = modelisateur.camp(nbImgInt,idPieceChar,date); 
                                    }
                                   

                                    // Print nbImgInt
                                    


                                } else {
                                    std::cout << "Missing idPieceChar token." << std::endl;
                                }
                            } else {
                                std::cout << "Missing nbImgChar token." << std::endl;
                            }
                        } else {
                            std::cout << "Unknown command received." << std::endl;
                        }
                    } else {
                        std::cout << "Empty or invalid command received." << std::endl;
                    }
                }
                write(sockfd_service, &type, sizeof(type)); // Utilisation de l'adresse de la variable
            }
            else {
                std::cout << "Erreur lors de la réception de la chaîne de caractères." << std::endl;
                char error = '0';
                write(sockfd_service, &error, sizeof(error));
            }
            write(sockfd_service, &resultat, sizeof(resultat));
            /* Fermeture de la connexion */
            close(sockfd_service);
        } 
        catch(const std::exception& e) {
            std::cout << "An exception occurred: " << e.what() << std::endl;
            char error = '0';
            write(sockfd_service, &error, sizeof(error));
        }
        catch(...) {
            std::cout << "An unknown exception occurred." << std::endl;
            char error = '0';
            write(sockfd_service, &error, sizeof(error));
        }
    }
    return 0;
}
