// cmodelisateur.cpp
// Marcoux Tristan Avril 2024
#include "cmodelisteur.h"
#include <wiringPi.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <math.h>

// Constructeur
Cmodelisateur::Cmodelisateur(int pinCapteur, int pinSteep, int pinDir, const char *pathDB, int nbPasMaxRasp, int nbPasMaxEsp,  const char ip, int port)
{
    // Initialisation des attributs de la classe
    this->pinCapteur = pinCapteur;
    this->pinSteep = pinSteep;
    this->pinDirection = pinDir;
    this->pathDB = pathDB;
    this->nbPasMaxEsp = nbPasMaxEsp;
    this->nbPasMaxRasp = nbPasMaxRasp;
    this->client=clientSocket(ip,port) 
        // Configuration des broches
    pinMode(this->pinSteep, OUTPUT);
    pinMode(this->pinDirection, OUTPUT);
    pinMode(this->pinCapteur, INPUT);
}

// Méthode pour écrire dans la base de données SQLite
int Cmodelisateur::ecrireDB(const char *requete)
{
    sqlite3 *db;
    char *errMsg = 0;

    // Ouverture de la base de données
    int rc = sqlite3_open("BDD_demonstrateur.db", &db);

    if (rc)
    {
        std::cerr << "Impossible d'ouvrir la base de données : " << sqlite3_errmsg(db) << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Base de données ouverte avec succès" << std::endl;
    }

    // Requête SQL pour créer une table
    char createTableSQL[] = "INSERT INTO Campagne__de_photo (date, Chemin_d_acces,id_Piece) VALUES ('03/04/2024','/test',9)";

    // Exécution de la requête SQL
    rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Erreur SQL : " << errMsg << std::endl;
        sqlite3_free(errMsg);
        // Fermeture de la base de données
        sqlite3_close(db);
        return 1;
    }
    else
    {
        std::cout << "Table créée avec succès" << std::endl;
        // Fermeture de la base de données
        sqlite3_close(db);
        return 0;
    }
}

// Méthode pour déplacer le moteur pas à pas
void Cmodelisateur::deplacerMot(int nbpas, int dir)
{
    wiringPisetup();

    pinMode(this->pinSteep, OUTPUT);
    pinMode(DIR, OUTPUT);
    digitalWrite(this->pinDirection, dir);
    for (int i = 0; i < nbpas; i++)
    {
        digitalWrite(this->pinSteep, HIGH); // Activé
        delay(5);                           //
        digitalWrite(this->pinSteep, LOW);  // Désactivé
        delay(5);
    }
    return;
}

// Méthode pour initialiser le système
int Cmodelisateur::initSys()
{
    while (digitalRead(this->pinCapteur))
    {
        digitalWrite(this->pinSteep, HIGH); // Activé
        delay(5);                           //
        digitalWrite(this->pinSteep, LOW);  // Désactivé
        delay(5);
    }
}

// Méthode pour envoyer une requête via un socket
int Cmodelisateur::envoiRequeteSocket(const char *requete)
{
    return client.envoiMess(requete);
}
int Cmodelisateur : prendrePhoto(std::string *nomphoto)
{
    string commande = "ffmpeg -f v4l2 -video_size 1920x1080 -i /dev/video0 -frames 1 test/test" + nomPhoto + ".jpg";
    return system(command.c_str());
}

// Méthode pour initialiser le système et envoyer une requête
int Cmodelisateur::init()
{
    if (initSys() && envoiRequeteSocket("i"))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Méthode pour réaliser une campagne de photos
int Cmodelisateur::camp(int nbImage, const char *iPiece, const char *date)
{
    std::stringstream ss;
    ss << "" << idPiece << "-" << date;
    std::string dossier = ss.str();
    system(("mkdir " + dossier).c_str());
    std::string requete = "INSERT INTO Campagne__de_photo (date, Chemin_d_acces, id_Piece) VALUES ('" + std::string(date) + "','" + dossier + "','" + std::string(iPiece) + "')";

    // Appel de la méthode ecrireDB avec la chaîne de requête
    this->ecrireDB(requete.c_str());

    if (init())
    {
        int nbdeplacement = int(floor(sqrt(nbimg)));
        int intervalleRasp = int(floor(nbPasMaxRasp / nbdeplacement));
        int intervalleEsp = int(floor(nbPasMaxEsp / nbdeplacement));
        int cpt = 0;
        int posEsp ; 

        for (int i = 0; i < (nbpasmax); i += intervalleRasp)
        {
            posEsp=0; 
            if (this->deplacerMot(i, 1);)
            {

                for (int j = 0; j < (nbpasmax - intervalle); j += intervalleEp)
                {
                    if (this->envoiRequeteSocket(("p." + std::to_string(j) + ".1").c_str());)
                    {
                        posEsp= j; 
                        if (this->prendrePhoto((dossier + "/" + cpt).c_str()))
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
                        } 
                        // rajouter retour esp 
                  if (!(this->envoiRequeteSocket(("p." + std::to_string(posEsp) + ".0").c_str());)){
                        return 4 ; 
                  }

            else
            {
                return 1;
            }
        }
    }
    else
    {
        return 0;
    }
}
