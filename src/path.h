#ifndef LIBTMAN_PATH_H
#define LIBTMAN_PATH_H

#include "libtman.h"

char *path_prj_dir(char *base, const tman_arg_t * args);
char *path_brd_dir(char *base, const tman_arg_t * args);
char *path_task_dir(char *base, const tman_arg_t * args);
char *path_prj_board(const char *base, const tman_arg_t * args);

char *path_prj_unit(char *base, const tman_arg_t * args);
char *path_brd_unit(char *base, const tman_arg_t * args);
char *path_task_unit(char *base, const tman_arg_t * args);

char *path_prj_toggle(char *base, const tman_arg_t * args);
char *path_brd_toggle(char *base, const tman_arg_t * args);
char *path_task_toggle(char *base, const tman_arg_t * args);

char *path_prj_column(char *base, const tman_arg_t * args);
char *path_brd_column(char *base, const tman_arg_t * args);
char *path_task_column(char *base, const tman_arg_t * args);

char *path_column(const char *dirname, const char *obj);
char *path_column_prio(const char *dirname, const char *obj);

#endif
