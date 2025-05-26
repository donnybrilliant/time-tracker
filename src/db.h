// src/db.h

#ifndef DB_H
#define DB_H

#include <sqlite3.h>

// Opens the database, returns 1 on success, 0 on failure
int db_open(const char *filename, sqlite3 **db);

// Creates tables if they don't exist, returns 1 on success, 0 on failure
int db_init(sqlite3 *db);

// Closes the database
void db_close(sqlite3 *db);

#endif
