#include <iostream>
#include <sqlite3.h>  // SQLite C API header

int main() {
    sqlite3* db;
    char* errMsg = 0;

    // Open database (creates if doesn't exist)
    int rc = sqlite3_open("BDD_demonstrateur.db", &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 0;
    } else {
        std::cout << "Opened database successfully" << std::endl;
    }

    // SQL statement to create a table
     char createTableSQL[] = "INSERT INTO Campagne__de_photo (date, Chemin_d_acces,id_Piece) VALUES ('03/04/2024','/test',9)";

    // Execute SQL statement
    rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created successfully" << std::endl;
    }

    // Close database
    sqlite3_close(db);

    return 0;
}

