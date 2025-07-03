#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "path.h"

static char pathname[PATH_MAX + 1];

// TODO: add generic function using varg to generate path

/* Generate path object directories.  */
char *path_prj_dir(char *base, const tman_arg_t * args)
{
    memset(pathname, 0, PATH_MAX);
    sprintf(pathname, "%s/%s", base, args->prj);
    //printf("path.c: %s: %s\n", __FUNCTION__, pathname);
    return pathname;
}

char *path_brd_dir(char *base, const tman_arg_t * args)
{
    memset(pathname, 0, PATH_MAX);
    sprintf(pathname, "%s/%s/%s", base, args->prj, args->brd);
    //printf("path.c: %s: %s\n", __FUNCTION__, pathname);
    return pathname;
}

char *path_task_dir(char *base, const tman_arg_t * args)
{
    memset(pathname, 0, PATH_MAX);
    sprintf(pathname, "%s/%s/%s/%s", base, args->prj, args->brd, args->id);
    //printf("path.c: %s:%s %s\n", __FUNCTION__, __func__, pathname);
    return pathname;
}

/* Generate path for unit files.  */
char *path_prj_unit(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/.tman/units";
    memset(pathname, 0, PATH_MAX);
    sprintf(pathname, fmt, base, args->prj);
    //printf("path.c: %s: %s\n", __FUNCTION__, pathname);
    return pathname;
}

char *path_brd_unit(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s/.tman/units";
    memset(pathname, 0, PATH_MAX);
    sprintf(pathname, fmt, base, args->prj, args->brd);
    //printf("path.c: %s: %s\n", __FUNCTION__, pathname);
    return pathname;
}

char *path_task_unit(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s/%s/.tman/units";
    memset(pathname, 0, PATH_MAX);
    sprintf(pathname, fmt, base, args->prj, args->brd, args->id);
    //printf("path.c: %s: %s\n", __FUNCTION__, pathname);
    return pathname;
}

char *path_column(const char *dirname, const char *obj)
{
    char *_pathname;

    if ((_pathname = malloc(PATH_MAX + 1)) == NULL)
        return NULL;
    memset(_pathname, 0, PATH_MAX);
    sprintf(_pathname, "%s/%s/.tman/column", dirname, obj);
    return _pathname;
}
