#ifndef LIBTMAN_BOARD_H
#define LIBTMAN_BOARD_H

#include "libtman.h"

int board_add(char *base, tman_arg_t * args, const char *colname);
int board_exist(char *base, tman_arg_t * args);
int board_move(char *base, tman_arg_t * args, const char *colname);
char *board_getcurr(char *base, const tman_arg_t * args);

int board_is_valid_length(char *brd);
int board_is_valid_name(char *brd);

#endif
