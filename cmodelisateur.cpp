// cmodelisateur.cpp
// Marcoux Tristan Avril 2024
#include "cmodelisateur.h"
#include <wiringPi.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath> // Use <cmath> instead of <math.h> in C++

Cmodelisateur::Cmodelisateur(int pinCapteur, int pinSteep, int pinDir, const char *pathDB, int nbPasMaxRasp, int nbPasMaxEsp, const char* ip, int port)
    : pinCapteur(pinCapteur),
      pinSteep(pinSteep),
      pinDirection(pinDir),
      pathDB(pathDB),
      nbPasMaxRasp(nbPasMaxRasp),
      nbPasMaxEsp(nbPasMaxEsp),
      client(clientSocket(ip, port)) // Assuming clientSocket is a class with a suitable constructor
{
    // Configuration des broches
    wiringPiSetup() ;
    pinMode(pinSteep, OUTPUT);
    pinMode(pinDirection, OUTPUT);
    pinMode(pinCapteur, INPUT);
    
}
/// Méthode pour écrire dans la base de données SQLite
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

// // Méthode pour envoyer une requête via un socket
// int Cmodelisateur::envoiRequeteSocket(const char *requete)
// {
//     return client.envoiMess(requete);
// }
// int Cmodelisateur : prendrePhoto(std::string *nomphoto)
// {
//     string commande = "ffmpeg -f v4l2 -video_size 1920x1080 -i /dev/video0 -frames 1 test/test" + nomPhoto + ".jpg";
//     return system(command.c_str());
// }

// // Méthode pour initialiser le système et envoyer une requête
// int Cmodelisateur::init()
// {
//     if (initSys() && envoiRequeteSocket("i"))
//     {
//         return 1;
//     }
//     else
//     {
//         return 0;
//     }
// }

// // Méthode pour réaliser une campagne de photos
// int Cmodelisateur::camp(int nbImage, const char *idPiece, const char *date)
// {

//     std::string dossier = "www/" + idPiece + "_" + date;
//     system(("mkdir " + dossier).c_str());
//     std::string requete = "INSERT INTO Campagne__de_photo (date, Chemin_d_acces, id_Piece) VALUES ('" + std::string(date) + "','" + dossier + "','" + std::string(iPiece) + "')";

//     // Appel de la méthode ecrireDB avec la chaîne de requête
//     this->ecrireDB(requete.c_str());

//     if (init())
//     {
//         int nbdeplacement = int(floor(sqrt(nbimg)));
//         // Calcul du nombre de déplacements en prenant la racine carrée du nombre d'images.

//         int intervalleRasp = int(floor(nbPasMaxRasp / nbdeplacement));
//         // Calcul de l'intervalle entre les déplacements pour Raspberry Pi en divisant nbPasMaxRasp par nbdeplacement.

//         int intervalleEsp = int(floor(nbPasMaxEsp / nbdeplacement));
//         // Calcul de l'intervalle entre les déplacements pour ESP (un autre dispositif) en divisant nbPasMaxEsp par nbdeplacement.

//         int cpt = 0;
//         // Compteur pour le nombre de photos prises.

//         int posEsp;
//         // Variable pour garder la position actuelle de l'ESP.

//         for (int i = 0; i < (nbpasmax); i += intervalleRasp)
//         // Boucle pour déplacer le dispositif Raspberry Pi à des intervalles réguliers.
//         {
//             posEsp = 0;
//             // Réinitialiser la position de l'ESP.

//             if (this->deplacerMot(i, 1);)
//             // Vérifie si le déplacement du dispositif Raspberry Pi est réussi.
//             {
//                 for (int j = 0; j < (nbpasmax - intervalle); j += intervalleEsp)
//                 // Boucle pour déplacer l'ESP à des intervalles réguliers.
//                 {
//                     if (this->envoiRequeteSocket(("p." + std::to_string(j) + ".1").c_str());)
//                     // Envoie une requête pour déplacer l'ESP.
//                     {
//                         posEsp = j;
//                         // Met à jour la position actuelle de l'ESP.

//                         if (this->prendrePhoto((dossier + "/" + cpt).c_str()))
//                         // Prend une photo et l'enregistre dans le dossier spécifié avec un compteur.
//                         {
//                             cpt++;
//                             // Incrémente le compteur de photos.
//                         }
//                         else
//                         {
//                             return 3;
//                             // Retourne 3 si la prise de photo échoue.
//                         }
//                     }
//                     else
//                     {
//                         return 2;
//                         // Retourne 2 si l'envoi de la requête de déplacement de l'ESP échoue.
//                     }
//                 }
//             }
//             else
//             {
//                 return 1;
//                 // Retourne 1 si le déplacement du dispositif Raspberry Pi échoue.
//             }

//             // Ajouter retour ESP à sa position initiale
//             if (!(this->envoiRequeteSocket(("p." + std::to_string(posEsp) + ".0").c_str());))
//             {
//                 return 4;
//                 // Retourne 4 si l'ESP ne peut pas être retourné à sa position initiale.
//             }
//         }
//     }
// }
// else
// {
//     return 0;
// }
// }
