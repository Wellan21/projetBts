// cmodelisateur.cpp
// Marcoux Tristan Avril 2024

#include "cmodelisteur.h"
#include <wiringPi.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sqlite3.h>

// Constructeur
Cmodelisateur::Cmodelisateur(int pinCapteur, int pinSteep, int pinDir, const char *pathDB, int nbPasMaxRasp, int nbPasMaxEsp, const char *ip, int port)
{
    // Initialisation des attributs de la classe
    this->pinCapteur = pinCapteur;
    this->pinSteep = pinSteep;
    this->pinDirection = pinDir;
    this->pathDB = pathDB;
    this->nbPasMaxEsp = nbPasMaxEsp;
    this->nbPasMaxRasp = nbPasMaxRasp;
    this->client = clientSocket(ip, port);

    // Configuration des broches
    pinMode(this->pinSteep, OUTPUT);
    pinMode(this->pinDirection, OUTPUT);
    pinMode(this->pinCapteur, INPUT);
}

// Méthode pour écrire dans la base de données SQLite
int Cmodelisateur::ecrireDB(const char *requete)
{
    sqlite3 *db;
    char *errMsg = nullptr;

    // Ouverture de la base de données
    int rc = sqlite3_open("BDD_demonstrateur.db", &db);

    if (rc)
    {
        std::cerr << "Impossible d'ouvrir la base de données : " << sqlite3_errmsg(db) << std::endl;
        return 0;
    }

    // Exécution de la requête SQL
    rc = sqlite3_exec(db, requete, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Erreur SQL : " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}

// Méthode pour déplacer le moteur pas à pas
void Cmodelisateur::deplacerMot(int nbpas, int dir)
{
    wiringPiSetup();

    pinMode(this->pinSteep, OUTPUT);
    pinMode(this->pinDirection, OUTPUT);
    digitalWrite(this->pinDirection, dir);
    for (int i = 0; i < nbpas; i++)
    {
        digitalWrite(this->pinSteep, HIGH); // Activé
        delay(5);
        digitalWrite(this->pinSteep, LOW);  // Désactivé
        delay(5);
    }
}

// Méthode pour initialiser le système
int Cmodelisateur::initSys()
{
    while (digitalRead(this->pinCapteur))
    {
        digitalWrite(this->pinSteep, HIGH); // Activé
        delay(5);
        digitalWrite(this->pinSteep, LOW);  // Désactivé
        delay(5);
    }
    return 0;
}

// Méthode pour envoyer une requête via un socket
int Cmodelisateur::envoiRequeteSocket(const char *requete)
{
    return client.envoiMess(requete);
}

// Méthode pour prendre une photo
int Cmodelisateur::prendrePhoto(const std::string &nomphoto)
{
    std::string commande = "ffmpeg -f v4l2 -video_size 1920x1080 -i /dev/video0 -frames 1 " + nomphoto + ".jpg";
    return system(commande.c_str());
}

// Méthode pour initialiser le système et envoyer une requête
int Cmodelisateur::init()
{
    if (initSys() == 0 && envoiRequeteSocket("i") == 0)
    {
        return 1;
    }
    return 0;
}

// Méthode pour réaliser une campagne de photos
int Cmodelisateur::camp(int nbImage, const char *idPiece, const char *date)
{
    std::string dossier = "www/" + std::string(idPiece) + "_" + std::string(date);
    system(("mkdir -p " + dossier).c_str());

    std::string requete = "INSERT INTO Campagne__de_photo (date, Chemin_d_acces, id_Piece) VALUES ('" + std::string(date) + "','" + dossier + "','" + std::string(idPiece) + "')";
    if (ecrireDB(requete.c_str()) != 0)
    {
        return 1;
    }

    if (init() != 1)
    {
        return 2;
    }

    int nbdeplacement = static_cast<int>(std::floor(std::sqrt(nbImage)));
    int intervalleRasp = static_cast<int>(std::floor(nbPasMaxRasp / nbdeplacement));
    int intervalleEsp = static_cast<int>(std::floor(nbPasMaxEsp / nbdeplacement));
    int cpt = 0;
    int posEsp;

    for (int i = 0; i < nbPasMaxRasp; i += intervalleRasp)
    {
        posEsp = 0;

        deplacerMot(i, 1);

        for (int j = 0; j < (nbPasMaxEsp - intervalleEsp); j += intervalleEsp)
        {
            std::string requete = "p." + std::to_string(j) + ".1";
            if (envoiRequeteSocket(requete.c_str()) == 0)
            {
                posEsp = j;

                if (prendrePhoto(dossier + "/" + std::to_string(cpt)) == 0)
                {
                    cpt++;
                }
                else
                {
                    return 3;
                }
            }
            else
            {
                return 2;
            }
        }

        std::string requeteRetour = "p." + std::to_string(posEsp) + ".0";
        if (envoiRequeteSocket(requeteRetour.c_str()) != 0)
        {
            return 4;
        }
    }

    return 0;
}
