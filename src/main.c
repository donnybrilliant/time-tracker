#include <stdio.h>
#include <string.h>
#include "db.h"
#include "project.h"

int main(int argc, char *argv[]) {
    sqlite3 *db;
    if (!db_open("tracker.db", &db)) return 1;
    if (!db_init(db)) {
        db_close(db);
        return 1;
    }

    if (argc < 2) {
        printf("Usage:\n"
               "  tracker add-project \"Project Name\"\n"
               "  tracker list-projects\n");
        db_close(db);
        return 0;
    }

    if (strcmp(argv[1], "add-project") == 0 && argc >= 3) {
        project_add(db, argv[2]);
    } else if (strcmp(argv[1], "list-projects") == 0) {
        project_list(db);
    } else {
        printf("Unknown command. Try 'tracker' for usage.\n");
    }

    db_close(db);
    return 0;
}
