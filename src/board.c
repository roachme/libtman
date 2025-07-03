#include <string.h>

#include "path.h"
#include "osdep.h"
#include "column.h"
#include "libtman.h"

// TODO: It add new board to BLOG only, add support for any column
int board_add(char *base, tman_arg_t * args, const char *colname)
{
    return 0;
}

int board_del(char *base, tman_arg_t * args)
{
    return 0;
}

int board_move(char *base, tman_arg_t * args, const char *colname)
{
    return 0;
}

int board_swap(char *base, tman_arg_t * args)
{
    return 0;
}

char *board_getcurr(char *base, const tman_arg_t * args)
{
    return 0;
}

int board_exist(char *base, tman_arg_t * args)
{
    return ISDIR(path_brd_dir(base, args));
}
