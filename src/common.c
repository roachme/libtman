#include <stdarg.h>
#include <ctype.h>

#include "libtman.h"
#include "common.h"

static char *base = tmanfs.base;
static char pathname[PATHSIZ + 1];

char *genpath_prj(char *prj)
{
    sprintf(pathname, "%s/%s", base, prj);
    return pathname;
}

char *genpath_full(char *prj, char *id)
{
    sprintf(pathname, "%s/%s/%s", base, prj, id);
    return pathname;
}

char *genpath_col(char *prj, char *id)
{
    sprintf(pathname, "%s/%s/%s/.tman/col", base, prj, id);
    return pathname;
}

char *genpath_unit(char *prj, char *id)
{
    sprintf(pathname, "%s/%s/%s/.tman/unit", base, prj, id);
    return pathname;
}

char *genpath_unit_prj(char *prj)
{
    sprintf(pathname, "%s/%s/.tman/unit", base, prj);
    return pathname;
}
