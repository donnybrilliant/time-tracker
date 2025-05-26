#include "task.h"
#include <stdio.h>
#include <time.h>

// Add a new task (not running yet)
int task_add(sqlite3 *db, int project_id, const char *name) {
  const char *sql = "INSERT INTO tasks (project_id, name, start_time, "
                    "end_time, is_running) VALUES (?, ?, 0, 0, 0);";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  sqlite3_bind_int(stmt, 1, project_id);
  sqlite3_bind_text(stmt, 2, name, -1, SQLITE_TRANSIENT);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to add task: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  printf("Task '%s' added to project %d.\n", name, project_id);
  return 1;
}

// Start a new running task
int task_start(sqlite3 *db, int project_id, const char *name) {
  time_t now = time(NULL);
  const char *sql = "INSERT INTO tasks (project_id, name, start_time, "
                    "end_time, is_running) VALUES (?, ?, ?, 0, 1);";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  sqlite3_bind_int(stmt, 1, project_id);
  sqlite3_bind_text(stmt, 2, name, -1, SQLITE_TRANSIENT);
  sqlite3_bind_int64(stmt, 3, (sqlite3_int64)now);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to start task: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  printf("Task '%s' started for project %d.\n", name, project_id);
  return 1;
}

// Stop a running task (by id)
int task_stop(sqlite3 *db, int project_id, int task_id) {
  time_t now = time(NULL);
  // Check if task is running and belongs to project
  const char *check_sql =
      "SELECT is_running FROM tasks WHERE id=? AND project_id=?;";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, check_sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare check statement: %s\n",
            sqlite3_errmsg(db));
    return 0;
  }
  sqlite3_bind_int(stmt, 1, task_id);
  sqlite3_bind_int(stmt, 2, project_id);

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_ROW) {
    printf("No such running task (ID %d in project %d).\n", task_id,
           project_id);
    sqlite3_finalize(stmt);
    return 0;
  }
  int is_running = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  if (!is_running) {
    printf("Task %d is not running.\n", task_id);
    return 0;
  }

  // Stop the task
  const char *sql =
      "UPDATE tasks SET end_time=?, is_running=0 WHERE id=? AND project_id=?;";
  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare update statement: %s\n",
            sqlite3_errmsg(db));
    return 0;
  }
  sqlite3_bind_int64(stmt, 1, (sqlite3_int64)now);
  sqlite3_bind_int(stmt, 2, task_id);
  sqlite3_bind_int(stmt, 3, project_id);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to stop task: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  printf("Task %d stopped.\n", task_id);
  return 1;
}

// List all tasks for a project
void task_list(sqlite3 *db, int project_id) {
  const char *sql = "SELECT id, name, start_time, end_time, is_running FROM "
                    "tasks WHERE project_id=? ORDER BY id;";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return;
  }
  sqlite3_bind_int(stmt, 1, project_id);

  printf("Tasks for project %d:\n", project_id);
  printf("%-5s %-30s %-20s %-20s %-10s\n", "ID", "Name", "Start", "End",
         "Status");

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    const unsigned char *name = sqlite3_column_text(stmt, 1);
    sqlite3_int64 start_time = sqlite3_column_int64(stmt, 2);
    sqlite3_int64 end_time = sqlite3_column_int64(stmt, 3);
    int is_running = sqlite3_column_int(stmt, 4);

    char start_buf[20] = "N/A";
    char end_buf[20] = "N/A";
    if (start_time) {
      struct tm *tm_info = localtime((time_t *)&start_time);
      strftime(start_buf, sizeof(start_buf), "%Y-%m-%d %H:%M", tm_info);
    }
    if (end_time) {
      struct tm *tm_info = localtime((time_t *)&end_time);
      strftime(end_buf, sizeof(end_buf), "%Y-%m-%d %H:%M", tm_info);
    }
    printf("%-5d %-30s %-20s %-20s %-10s\n", id, name, start_buf, end_buf,
           is_running ? "Running" : "Stopped");
  }
  sqlite3_finalize(stmt);
}

// Report total tracked time per project
void task_report(sqlite3 *db, int project_id) {
  const char *sql =
      "SELECT name, start_time, end_time FROM tasks WHERE project_id=?;";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return;
  }
  sqlite3_bind_int(stmt, 1, project_id);

  printf("Task report for project %d:\n", project_id);
  printf("%-30s %-12s\n", "Task", "Duration (min)");

  int total_minutes = 0;

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    const unsigned char *name = sqlite3_column_text(stmt, 0);
    sqlite3_int64 start_time = sqlite3_column_int64(stmt, 1);
    sqlite3_int64 end_time = sqlite3_column_int64(stmt, 2);

    int minutes = 0;
    if (start_time && end_time && end_time > start_time) {
      minutes = (int)((end_time - start_time) / 60);
      total_minutes += minutes;
    }
    printf("%-30s %-12d\n", name, minutes);
  }
  sqlite3_finalize(stmt);

  printf("%-30s %-12d\n", "TOTAL", total_minutes);
}

// Delete a task
int task_delete(sqlite3 *db, int task_id) {
  const char *sql = "DELETE FROM tasks WHERE id=?;";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare delete task: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  sqlite3_bind_int(stmt, 1, task_id);
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to delete task: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  printf("Task %d deleted.\n", task_id);
  return 1;
}

// Edit a task
int task_edit(sqlite3 *db, int task_id, const char *new_name) {
  const char *sql = "UPDATE tasks SET name=? WHERE id=?;";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare edit task: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 2, task_id);
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to update task: %s\n", sqlite3_errmsg(db));
    return 0;
  }
  printf("Task %d renamed to '%s'.\n", task_id, new_name);
  return 1;
}

// Export tasks to CSV
void task_export_csv(sqlite3 *db, int project_id, const char *filename) {
  const char *sql = "SELECT id, name, start_time, end_time, is_running FROM "
                    "tasks WHERE project_id=?;";
  sqlite3_stmt *stmt;
  FILE *fp = fopen(filename, "w");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s' for writing.\n", filename);
    return;
  }
  fprintf(fp, "id,name,start_time,end_time,is_running\n");
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    fclose(fp);
    return;
  }
  sqlite3_bind_int(stmt, 1, project_id);

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    const unsigned char *name = sqlite3_column_text(stmt, 1);
    sqlite3_int64 start_time = sqlite3_column_int64(stmt, 2);
    sqlite3_int64 end_time = sqlite3_column_int64(stmt, 3);
    int is_running = sqlite3_column_int(stmt, 4);

    fprintf(fp, "%d,\"%s\",%lld,%lld,%d\n", id, name, start_time, end_time,
            is_running);
  }
  sqlite3_finalize(stmt);
  fclose(fp);
  printf("Exported tasks to '%s'.\n", filename);
}
