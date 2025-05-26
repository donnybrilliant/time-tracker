#ifndef TASK_H
#define TASK_H

#include <sqlite3.h>

int task_add(sqlite3 *db, int project_id, const char *name);
int task_start(sqlite3 *db, int project_id, const char *name);
int task_stop(sqlite3 *db, int project_id, int task_id);
void task_list(sqlite3 *db, int project_id);
void task_report(sqlite3 *db, int project_id);
int task_delete(sqlite3 *db, int task_id);
int task_edit(sqlite3 *db, int task_id, const char *new_name);
void task_export_csv(sqlite3 *db, int project_id, const char *filename);
void task_list_running(sqlite3 *db);

#endif
