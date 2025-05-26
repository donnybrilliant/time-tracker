#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#endif

int db_open(const char *filename, sqlite3 **db) {
  if (sqlite3_open(filename, db) != SQLITE_OK) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
    return 0;
  }
  return 1;
}

int db_init(sqlite3 *db) {
  const char *sql = "CREATE TABLE IF NOT EXISTS projects ("
                    " id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    " name TEXT NOT NULL UNIQUE,"
                    " wage REAL DEFAULT 0.0"
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
  // Try to add wage column if missing (safe if exists)
  sqlite3_exec(db, "ALTER TABLE projects ADD COLUMN wage REAL DEFAULT 0.0;", 0,
               0, 0);
  return 1;
}

// Returns a malloc'd string (caller must free)
char *get_database_path(void) {
  char *path = NULL;
#ifdef _WIN32
  char *appdata = getenv("APPDATA");
  if (appdata) {
    path = malloc(strlen(appdata) + 32);
    sprintf(path, "%s\\tracker", appdata);
    mkdir(path);
    strcat(path, "\\tracker.db");
  }
#else
  char *home = getenv("HOME");
  if (home) {
    path = malloc(strlen(home) + 32);
    sprintf(path, "%s/.tracker", home);
    mkdir(path, 0700);
    strcat(path, "/tracker.db");
  }
#endif
  return path;
}

void db_close(sqlite3 *db) {
  if (db)
    sqlite3_close(db);
}
