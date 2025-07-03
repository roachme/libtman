#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"
#include "osdep.h"

int board_exist(char *base, tman_arg_t * args)
{
    char *pathname = path_brd_dir(base, args);
    return ISDIR(pathname);
}

int board_is_valid_name(char *brd)
{
    if (!isalnum(*brd++))
        return 0;
    for (; *brd; ++brd)
        if (!(isalnum(*brd) || *brd == '_' || *brd == '-'))
            return 0;
    return isalnum(*--brd) != 0;
}

int board_is_valid_length(char *brd)
{
    return strlen(brd) <= PRJSIZ;
}
