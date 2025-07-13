#ifndef LIBTMAN_TASK_H
#define LIBTMAN_TASK_H

#include "libtman.h"

// TODO: move it cli part (both of 'em)
int task_is_valid_name(char *id);
int task_is_valid_length(char *id);
int task_exist(char *base, const tman_arg_t * args);

#endif
