#ifndef PROJECT_H
#define PROJECT_H

#include <sqlite3.h>

// Add a project. Returns 1 on success, 0 on error.
int project_add(sqlite3 *db, const char *name, double wage);

// List all projects.
void project_list(sqlite3 *db);

// Delete a project. Returns 1 on success, 0 on error.
int project_delete(sqlite3 *db, int project_id);

// Edit a project. Returns 1 on success, 0 on error.
int project_edit(sqlite3 *db, int project_id, const char *new_name);

// Edit the wage for a project. Returns 1 on success, 0 on error.
int project_edit_wage(sqlite3 *db, int project_id, double wage);

// Get the wage for a project.
double project_get_wage(sqlite3 *db, int project_id);

// Look up a project ID by name.
int project_id_from_name(sqlite3 *db, const char *name);

#endif
