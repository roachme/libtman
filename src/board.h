#ifndef LIBTMAN_BOARD_H
#define LIBTMAN_BOARD_H

#include "libtman.h"

int board_is_valid_length(char *brd);
int board_is_valid_name(char *brd);
int board_exist(char *base, tman_arg_t * args);

#endif
