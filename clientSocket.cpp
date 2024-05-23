#include "clientSocket.h" 

 clientSocket(char* ipSrv, int portsrv) {
        this->ip = ipSrv;
        this->port = portsrv;
    }
clientSocket(){
      this->ip ="1.1.1.1";
        this->port = 80;
    }

    bool envoyerMessage( char* msg) {
        /* Déclaration des variables */
        int sockfd_client;
        int lg_client;
        struct sockaddr_in adresse;
        int resultat;
        char ch = 'X';

        /* Création socket pour le client */
        sockfd_client = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_client == -1) {
            perror("Erreur lors de la création du socket");
            return false;
        }

        /* Nommage du socket avec les valeurs du constructeur */
        adresse.sin_family = AF_INET;
        adresse.sin_addr.s_addr = inet_addr(this->ip);
        adresse.sin_port = htons(this->port);

        lg_client = sizeof(adresse);

        /* Connexion du socket à celui du serveur */
        resultat = connect(sockfd_client, (struct sockaddr*)&adresse, lg_client);
        if (resultat == -1) {
            perror("Erreur lors de la connexion au serveur");
            close(sockfd_client);
            return 0;
        }

        /* Ecriture via sockfd_client */
        write(sockfd_client, &ch, 1);

        /* Lecture via sockfd_client */
        read(sockfd_client, &ch, 1);

       if (ch=='1'){
        return true;
       }
       else [
        return false;
       ]

        /* Fermeture connexion côté client */
        close(sockfd_client);

        return true;
    }