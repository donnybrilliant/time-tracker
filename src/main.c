#include "db.h"
#include "project.h"
#include "task.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_database_path(void);

// Helper: check if string is a number
int is_number(const char *s) {
  if (!s || !*s)
    return 0;
  while (*s) {
    if (!isdigit((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

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
           "  tracker project add \"Project Name\" <wage>\n"
           "  tracker project list\n"
           "  tracker project edit <project_id> \"New Name\"\n"
           "  tracker project wage <project_id> <wage>\n"
           "  tracker project delete <project_id>\n"
           "  tracker task add <project_id> \"Task Name\"\n"
           "  tracker task start <project_id> \"Task Name\"\n"
           "  tracker task stop <task_id>\n"
           "  tracker task edit <task_id> \"New Name\"\n"
           "  tracker task delete <task_id>\n"
           "  tracker task list <project_id>\n"
           "  tracker report <project_id>\n"
           "  tracker running\n"
           "  tracker export <project_id> <filename>\n");
    db_close(db);
    return 0;
  }

  // Subcommand-based parsing
  if (strcmp(argv[1], "project") == 0) {
    if (argc >= 4 && strcmp(argv[2], "add") == 0) {
      double wage = atof(argv[4]);
      if (!*argv[3]) {
        printf("Error: Project name is required.\n");
      } else {
        project_add(db, argv[3], wage);
      }
    } else if (argc >= 3 && strcmp(argv[2], "list") == 0) {
      project_list(db);
    } else if (argc >= 5 && strcmp(argv[2], "edit") == 0) {
      int project_id = is_number(argv[3]) ? atoi(argv[3])
                                          : project_id_from_name(db, argv[3]);
      if (!project_id) {
        printf("Error: Project '%s' not found.\n", argv[3]);
      } else {
        project_edit(db, project_id, argv[4]);
      }
    } else if (argc >= 5 && strcmp(argv[2], "wage") == 0) {
      int project_id = is_number(argv[3]) ? atoi(argv[3])
                                          : project_id_from_name(db, argv[3]);
      if (!project_id) {
        printf("Error: Project '%s' not found.\n", argv[3]);
      } else {
        double wage = atof(argv[4]);
        project_edit_wage(db, project_id, wage);
      }
    } else if (argc >= 4 && strcmp(argv[2], "delete") == 0) {
      int project_id = is_number(argv[3]) ? atoi(argv[3])
                                          : project_id_from_name(db, argv[3]);
      if (!project_id) {
        printf("Error: Project '%s' not found.\n", argv[3]);
      } else {
        project_delete(db, project_id);
      }
    } else {
      printf(
          "Unknown or incomplete project command. Try 'tracker' for usage.\n");
    }
  } else if (strcmp(argv[1], "task") == 0) {
    if (argc >= 5 && strcmp(argv[2], "add") == 0) {
      int project_id = is_number(argv[3]) ? atoi(argv[3])
                                          : project_id_from_name(db, argv[3]);
      if (!project_id) {
        printf("Error: Project '%s' not found.\n", argv[3]);
      } else {
        task_add(db, project_id, argv[4]);
      }
    } else if (argc >= 5 && strcmp(argv[2], "start") == 0) {
      int project_id = is_number(argv[3]) ? atoi(argv[3])
                                          : project_id_from_name(db, argv[3]);
      if (!project_id) {
        printf("Error: Project '%s' not found.\n", argv[3]);
      } else {
        task_start(db, project_id, argv[4]);
      }
    } else if (argc >= 4 && strcmp(argv[2], "stop") == 0) {
      int task_id =
          is_number(argv[3]) ? atoi(argv[3]) : task_id_from_name(db, argv[3]);
      if (!task_id) {
        printf("Error: Task '%s' not found.\n", argv[3]);
      } else {
        task_stop(db, task_id);
      }
    } else if (argc >= 5 && strcmp(argv[2], "edit") == 0) {
      int task_id =
          is_number(argv[3]) ? atoi(argv[3]) : task_id_from_name(db, argv[3]);
      if (!task_id) {
        printf("Error: Task '%s' not found.\n", argv[3]);
      } else {
        task_edit(db, task_id, argv[4]);
      }
    } else if (argc >= 4 && strcmp(argv[2], "delete") == 0) {
      int task_id =
          is_number(argv[3]) ? atoi(argv[3]) : task_id_from_name(db, argv[3]);
      if (!task_id) {
        printf("Error: Task '%s' not found.\n", argv[3]);
      } else {
        task_delete(db, task_id);
      }
    } else if (argc >= 4 && strcmp(argv[2], "list") == 0) {
      int project_id = is_number(argv[3]) ? atoi(argv[3])
                                          : project_id_from_name(db, argv[3]);
      if (!project_id) {
        printf("Error: Project '%s' not found.\n", argv[3]);
      } else {
        task_list(db, project_id);
      }
    } else {
      printf("Unknown or incomplete task command. Try 'tracker' for usage.\n");
    }
  } else if (strcmp(argv[1], "report") == 0 && argc >= 3) {
    int project_id = atoi(argv[2]);
    task_report(db, project_id);
  } else if (strcmp(argv[1], "running") == 0) {
    task_list_running(db);
  } else if (strcmp(argv[1], "export") == 0 && argc >= 4) {
    int project_id = atoi(argv[2]);
    task_export_csv(db, project_id, argv[3]);
  } else {
    printf("Unknown command. Try 'tracker' for usage.\n");
  }

  db_close(db);
  return 0;
}
