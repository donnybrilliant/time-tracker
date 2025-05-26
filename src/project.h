#ifndef PROJECT_H
#define PROJECT_H

#include <sqlite3.h>

// Add a project. Returns 1 on success, 0 on error.
int project_add(sqlite3 *db, const char *name);

// List all projects.
void project_list(sqlite3 *db);

#endif
