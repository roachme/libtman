#ifndef LIBTMAN_PROJECT_H
#define LIBTMAN_PROJECT_H

#include "libtman.h"

//char *project_getcurr(void);
//char *project_getprev(void);
int project_delcurr(void);
int project_delprev(void);
int project_swap(void);
int project_addcurr(char *project);
int project_is_valid_name(char *project);
int project_is_curr(char *project);
int project_is_prev(char *project);
int project_is_valid_length(char *project);

int project_exist(char *base, tman_arg_t * args);
int project_add(char *base, const tman_arg_t * args, char *colname);
int project_move(char *base, const tman_arg_t * args, char *colname);
char *project_getcurr(char *base, const tman_arg_t * args);
char *project_getprev(char *base, const tman_arg_t * args);

#endif
