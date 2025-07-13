/*
    Define library limitations in one place.
*/

#ifndef LIBTMAN_LIMIT_H
#define LIBTMAN_LIMIT_H

/* TODO: delete these constrains */
#define IDSIZ           10
#define BRDSIZ          10
#define PRJSIZ          10

int limit_project_ok(char *name);
int limit_board_ok(char *name);
int limit_task_ok(char *name);

#endif
