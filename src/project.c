#include "project.h"
#include <stdio.h>

int project_add(sqlite3 *db, const char *name, double wage) {
  const char *sql = "INSERT INTO projects (name, wage) VALUES (?, ?);";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
  sqlite3_bind_double(stmt, 2, wage);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to add project: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  printf("Project '%s' added successfully with wage %.2f.\n", name, wage);
  return 1;
}

void project_list(sqlite3 *db) {
  const char *sql = "SELECT id, name, wage FROM projects;";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return;
  }

  printf("Projects:\n");
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    const unsigned char *name = sqlite3_column_text(stmt, 1);
    double wage = sqlite3_column_double(stmt, 2);
    printf("  [%d] %s (Wage: %.2f/hour)\n", id, name, wage);
  }
  sqlite3_finalize(stmt);
}

int project_delete(sqlite3 *db, int project_id) {
  // Delete tasks
  const char *sql1 = "DELETE FROM tasks WHERE project_id=?;";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql1, -1, &stmt, NULL);
  if (rc == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, project_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
  // Delete project
  const char *sql2 = "DELETE FROM projects WHERE id=?;";
  rc = sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare delete project: %s\n",
            sqlite3_errmsg(db));
    return 0;
  }
  sqlite3_bind_int(stmt, 1, project_id);
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to delete project: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  printf("Project %d deleted.\n", project_id);
  return 1;
}

int project_edit(sqlite3 *db, int project_id, const char *new_name) {
  const char *sql = "UPDATE projects SET name=? WHERE id=?;";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare edit project: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 2, project_id);
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to update project: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  printf("Project %d renamed to '%s'.\n", project_id, new_name);
  return 1;
}

int project_edit_wage(sqlite3 *db, int project_id, double wage) {
  const char *sql = "UPDATE projects SET wage=? WHERE id=?;";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare edit wage: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  sqlite3_bind_double(stmt, 1, wage);
  sqlite3_bind_int(stmt, 2, project_id);
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to update wage: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  printf("Project %d wage updated to %.2f.\n", project_id, wage);
  return 1;
}

double project_get_wage(sqlite3 *db, int project_id) {
  const char *sql = "SELECT wage FROM projects WHERE id=?;";
  sqlite3_stmt *stmt;
  double wage = 0.0;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, project_id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      wage = sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);
  }
  return wage;
}
