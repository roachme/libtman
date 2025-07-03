#ifndef LIBTMAN_COLUMN_H
#define LIBTMAN_COLUMN_H

#include <limits.h>
#include "common.h"
#include "libtman.h"

#define COLSIZ      10
#define NCOLITEM    1
#define TOGGLEID_SIZ      10

#define COLUMN_FMT          "column : %s\n"
#define COLUMN_CURR         "curr"
#define COLUMN_PREV         "prev"
#define COLUMN_BLOG         "blog"
#define COLUMN_DONE         "done"
#define COLUMN_CURR_PRIO    0
#define COLUMN_PREV_PRIO    1
#define COLUMN_BLOG_PRIO    2
#define COLUMN_DONE_PRIO    INT_MAX

#define TOGGLESIZ       IDSIZ + PRJSIZ + 1

struct column {
    int prio;
    char mark;
    char prj[PRJSIZ + 1];
    char col[COLSIZ + 1];
};

typedef struct toggle {
    struct minitog {
        char id[TOGGLEID_SIZ + 1];
        char column[10];
        int update;
    } curr;
    struct {
        char id[TOGGLEID_SIZ + 1];
        char column[10];
        int update;
    } prev;
    struct {
        char id[TOGGLEID_SIZ + 1];
        char column[10];
        int update;
    } tmp;
} toggle_t;

//char *column_get_name(char *base, tman_arg_t * args);
//char *column_get_prio(char *base, tman_arg_t * args);

/* Project columns: BEGIN.  */
char *column_project_curr(char *base, tman_arg_t * args);
char *column_project_prev(char *base, tman_arg_t * args);
int column_project_del(char *base, tman_arg_t * args);
int column_project_move(char *base, tman_arg_t * args, char *colname);
int column_project_swap(char *base, tman_arg_t * args);
/* Project columns: END.  */

/* Board columns: BEGIN.  */
char *column_board_curr(char *base, tman_arg_t * args);
char *column_board_prev(char *base, tman_arg_t * args);
int column_board_del(char *base, tman_arg_t * args);
int column_board_move(char *base, tman_arg_t * args, char *colname);
int column_board_swap(char *base, tman_arg_t * args);
/* Board columns: END.  */

/* Task columns: BEGIN.  */
char *column_task_curr(char *base, tman_arg_t * args);
char *column_task_prev(char *base, tman_arg_t * args);
int column_task_del(char *base, tman_arg_t * args);
int column_task_move(char *base, tman_arg_t * args, char *colname);
int column_task_swap(char *base, tman_arg_t * args);
/* Task columns: END */

char *column_task_colname(char *base, tman_arg_t * args);

#endif
