#ifndef LIBTMAN_TASK_H
#define LIBTMAN_TASK_H

#include "libtman.h"

// TODO: move it cli part (both of 'em)
int task_is_valid_name(char *id);
int task_is_valid_length(char *id);

// TODO: maybe move parameter `args` before `base`
// TODO: maybe rename base to `taskdir`

// TODO: maybe add parameter `char *colname` to specify where to add it
int task_add(char *base, const tman_arg_t * args, char *colname);
int task_exist(char *base, const tman_arg_t * args);
int task_del(char *base, const tman_arg_t * args);
int task_move(char *base, const tman_arg_t * args, char *col);
int task_swap(char *base, const tman_arg_t * a, const tman_arg_t * b);

int task_iscurr(char *base, const tman_arg_t * args);
int task_isprev(char *base, const tman_arg_t * args);

// TODO: maybe not use `const` and let the function to set default (current) task ID?
char *task_getcurr(char *base, const tman_arg_t * args);
char *task_getprev(char *base, const tman_arg_t * args);

#endif
