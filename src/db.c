// src/db.c

#include "db.h"
#include <stdio.h>

int db_open(const char *filename, sqlite3 **db) {
    if (sqlite3_open(filename, db) != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
        return 0;
    }
    return 1;
}

int db_init(sqlite3 *db) {
    const char *sql =
        "CREATE TABLE IF NOT EXISTS projects ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " name TEXT NOT NULL UNIQUE"
        ");"
        "CREATE TABLE IF NOT EXISTS tasks ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " project_id INTEGER,"
        " name TEXT,"
        " start_time INTEGER,"
        " end_time INTEGER,"
        " is_running INTEGER,"
        " FOREIGN KEY (project_id) REFERENCES projects(id)"
        ");";
    char *errmsg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 0;
    }
    return 1;
}

void db_close(sqlite3 *db) {
    if (db) sqlite3_close(db);
}
