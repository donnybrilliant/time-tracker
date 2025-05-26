#ifndef TASK_H
#define TASK_H

#include <sqlite3.h>

int task_add(sqlite3 *db, int project_id, const char *name); // Add new task, not running yet
int task_start(sqlite3 *db, int project_id, const char *name); // Create and start a new task
int task_stop(sqlite3 *db, int project_id, int task_id); // Stop (finish) a running task
void task_list(sqlite3 *db, int project_id); // List all tasks for a project
void task_report(sqlite3 *db, int project_id); // Summarize total time
int task_delete(sqlite3 *db, int task_id); // Delete a task
int task_edit(sqlite3 *db, int task_id, const char *new_name); // Edit a task
void task_export_csv(sqlite3 *db, int project_id, const char *filename); // Export tasks to CSV


#endif
