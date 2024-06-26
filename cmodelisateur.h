//cmodelisateur.h 
// Marcoux Tristan Avril 2024 
#ifndef CMODELISATEUR_H
#define CMODELISATEUR_H

#include "clientSocket.h"
#include <iostream>
#include <sqlite3.h>

class Cmodelisateur {
private: 
    clientSocket client; 
    int pinCapteur; 
    int pinSteep; 
    int pinDirection;
    const char* pathDB; 
    int positionMot;
    int nbPasMaxRasp;
    int nbPasMaxEsp;
    int ecrireDB(const char* requete);
    bool deplacerMot(int nbpas, int dir); 
    int envoiRequeteSocket(const char* requete); 
    int initSys(); 
    int prendrePhoto(std::string nomPhoto);
    
public: 
    int init(); 
    char camp(int nbImage, const char* idPiece, const char* date); 
    Cmodelisateur(int pinCapteur, int pinSteep, int pinDir, const char* pathDB, int nbPasMaxRasp, int nbPasMaxEsp, const char* ip, int port);
};

#endif // CMODELISATEUR_H
