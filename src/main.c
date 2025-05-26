#include "db.h"
#include "project.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_database_path(void);

int main(int argc, char *argv[]) {
  char *db_path = get_database_path();
  sqlite3 *db;
  if (!db_open(db_path, &db)) {
    free(db_path);
    return 1;
  }
  if (!db_init(db)) {
    db_close(db);
    free(db_path);
    return 1;
  }
  free(db_path);

  if (argc < 2) {
    printf("Usage:\n"
           "  tracker add-project \"Project Name\" <wage>\n"
           "  tracker list-projects\n"
           "  tracker edit-project <project_id> \"New Name\"\n"
           "  tracker set-wage <project_id> <wage>\n"
           "  tracker delete-project <project_id>\n"
           "  tracker add-task <project_id> \"Task Name\"\n"
           "  tracker start-task <project_id> \"Task Name\"\n"
           "  tracker stop-task <project_id> <task_id>\n"
           "  tracker edit-task <task_id> \"New Name\"\n"
           "  tracker delete-task <task_id>\n"
           "  tracker list-tasks <project_id>\n"
           "  tracker report <project_id>\n"
           "  tracker running-tasks\n"
           "  tracker export-csv <project_id> <filename>\n");
    db_close(db);
    return 0;
  }

  if (strcmp(argv[1], "add-project") == 0 && argc >= 4) {
    double wage = atof(argv[3]);
    project_add(db, argv[2], wage);
  } else if (strcmp(argv[1], "list-projects") == 0) {
    project_list(db);
  } else if (strcmp(argv[1], "edit-project") == 0 && argc >= 4) {
    int project_id = atoi(argv[2]);
    project_edit(db, project_id, argv[3]);
  } else if (strcmp(argv[1], "set-wage") == 0 && argc >= 4) {
    int project_id = atoi(argv[2]);
    double wage = atof(argv[3]);
    project_edit_wage(db, project_id, wage);
  } else if (strcmp(argv[1], "delete-project") == 0 && argc >= 3) {
    int project_id = atoi(argv[2]);
    project_delete(db, project_id);
  } else if (strcmp(argv[1], "add-task") == 0 && argc >= 4) {
    int project_id = atoi(argv[2]);
    task_add(db, project_id, argv[3]);
  } else if (strcmp(argv[1], "start-task") == 0 && argc >= 4) {
    int project_id = atoi(argv[2]);
    task_start(db, project_id, argv[3]);
  } else if (strcmp(argv[1], "stop-task") == 0 && argc >= 4) {
    int project_id = atoi(argv[2]);
    int task_id = atoi(argv[3]);
    task_stop(db, project_id, task_id);
  } else if (strcmp(argv[1], "edit-task") == 0 && argc >= 4) {
    int task_id = atoi(argv[2]);
    task_edit(db, task_id, argv[3]);
  } else if (strcmp(argv[1], "delete-task") == 0 && argc >= 3) {
    int task_id = atoi(argv[2]);
    task_delete(db, task_id);
  } else if (strcmp(argv[1], "list-tasks") == 0 && argc >= 3) {
    int project_id = atoi(argv[2]);
    task_list(db, project_id);
  } else if (strcmp(argv[1], "report") == 0 && argc >= 3) {
    int project_id = atoi(argv[2]);
    task_report(db, project_id);
  } else if (strcmp(argv[1], "running-tasks") == 0) {
    task_list_running(db);
  } else if (strcmp(argv[1], "export-csv") == 0 && argc >= 4) {
    int project_id = atoi(argv[2]);
    task_export_csv(db, project_id, argv[3]);
  } else {
    printf("Unknown command. Try 'tracker' for usage.\n");
  }

  db_close(db);
  return 0;
}
