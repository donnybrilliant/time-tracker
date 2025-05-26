// src/main.c

#include <stdio.h>
#include "db.h"

int main() {
    sqlite3 *db;
    if (!db_open("tracker.db", &db)) {
        return 1;
    }
    if (!db_init(db)) {
        db_close(db);
        return 1;
    }
    printf("Database initialized successfully!\n");
    db_close(db);
    return 0;
}
