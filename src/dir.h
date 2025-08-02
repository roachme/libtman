#ifndef LIBTMAN_DIR_H
#define LIBTMAN_DIR_H

#include "libtman.h"

int dir_prj_add(char *base, tman_arg_t * args);
int dir_prj_del(char *base, tman_arg_t * args);
int dir_prj_rename(char *base, tman_arg_t * src, tman_arg_t * dst);

int dir_brd_add(char *base, tman_arg_t * args);
int dir_brd_del(char *base, tman_arg_t * args);
int dir_brd_move(char *base, tman_arg_t * src, tman_arg_t * dst);

int dir_task_add(char *base, tman_arg_t * args);
int dir_task_del(char *base, tman_arg_t * args);
int dir_task_move(char *base, tman_arg_t * src, tman_arg_t * dst);

#endif
