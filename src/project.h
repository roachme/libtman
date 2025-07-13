#ifndef LIBTMAN_PROJECT_H
#define LIBTMAN_PROJECT_H

#include "libtman.h"

int project_is_valid_name(char *project);
int project_is_valid_length(char *project);
int project_exist(char *base, tman_arg_t * args);

#endif
