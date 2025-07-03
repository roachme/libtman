#ifndef LIBTMAN_COLUMN_H
#define LIBTMAN_COLUMN_H

#include "common.h"
#include "libtman.h"

#define COLSIZ      10
#define NCOLITEM    1

#define COLFMT     "column : %s\n"
#define COLCURR    "curr"
#define COLPREV    "prev"
#define COLBLOG    "blog"
#define COLDONE    "done"

// TODO: DUPlicate, cate rid of it
#define PRJSIZ          10

#define TOGGLESIZ       IDSIZ + PRJSIZ + 1

struct column {
    int prio;
    char mark;
    char prj[PRJSIZ + 1];
    char col[COLSIZ + 1];
};

typedef struct toggle {
    struct minitog {
        char id[10];
        char column[10];
        int update;
    } curr;
    struct {
        char id[10];
        char column[10];
        int update;
    } prev;
    struct {
        char id[10];
        char column[10];
        int update;
    } tmp;
} toggle_t;

/* Project columns: BEGIN.  */
char *column_project_curr(tman_arg_t * args);
char *column_project_prev(tman_arg_t * args);
int column_project_add(tman_arg_t * args, char *colname);
int column_project_del(tman_arg_t * args);
int column_project_move(tman_arg_t * args, char *colname);
int column_project_swap(tman_arg_t * args);
/* Project columns: END.  */

/* Board columns: BEGIN.  */
char *column_board_curr(tman_arg_t * args);
char *column_board_prev(tman_arg_t * args);
int column_board_add(tman_arg_t * args, char *colname);
int column_board_del(tman_arg_t * args);
int column_board_move(tman_arg_t * args, char *colname);
int column_board_swap(tman_arg_t * args);
/* Board columns: END.  */

/* Task columns: BEGIN.  */
int column_task_add(tman_arg_t * args, char *colname);
int column_task_del(tman_arg_t * args);
int column_task_move(tman_arg_t * args, char *colname);
int column_task_swap(tman_arg_t * args);
/* Task columns: END */

#endif
