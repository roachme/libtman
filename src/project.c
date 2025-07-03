#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"
#include "osdep.h"

// BUG: it does not return TRUE/FALSE, but number instead.
/* TODO: change function name, cuz it's confusing.  */
int project_is_valid_name(char *prj)
{
    if (!isalnum(*prj++))
        return 0;
    for (; *prj; ++prj)
        if (!(isalnum(*prj) || *prj == '_' || *prj == '-'))
            return 0;
    return isalnum(*--prj) != 0;
}

int project_exist(char *base, tman_arg_t * args)
{
    char *pathname = path_prj_dir(base, args);
    return ISDIR(pathname);
}

/* TODO: change function name, cuz it's confusing.  */
int project_is_valid_length(char *prj)
{
    return strlen(prj) <= PRJSIZ;
}
