#include "db.h"
#include "project.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  sqlite3 *db;
  if (!db_open("tracker.db", &db))
    return 1;
  if (!db_init(db)) {
    db_close(db);
    return 1;
  }

  if (argc < 2) {
    printf("Usage:\n"
           "  tracker add-project \"Project Name\"\n"
           "  tracker list-projects\n"
           "  tracker add-task <project_id> \"Task Name\"\n"
           "  tracker start-task <project_id> \"Task Name\"\n"
           "  tracker stop-task <project_id> <task_id>\n"
           "  tracker list-tasks <project_id>\n"
           "  tracker report <project_id>\n");
    db_close(db);
    return 0;
  }

  if (strcmp(argv[1], "add-project") == 0 && argc >= 3) {
    project_add(db, argv[2]);
  } else if (strcmp(argv[1], "list-projects") == 0) {
    project_list(db);
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
  } else if (strcmp(argv[1], "list-tasks") == 0 && argc >= 3) {
    int project_id = atoi(argv[2]);
    task_list(db, project_id);
  } else if (strcmp(argv[1], "report") == 0 && argc >= 3) {
    int project_id = atoi(argv[2]);
    task_report(db, project_id);
  } else {
    printf("Unknown command. Try 'tracker' for usage.\n");
  }

  db_close(db);
  return 0;
}
