#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "path.h"

// TODO: add generic function using varg to generate path

/* Generate path object directories.  */
char *path_prj_dir(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s";
    static char pathname[PATH_MAX + 1];
    sprintf(pathname, fmt, base, args->prj);
    //printf("path.c: %s:pathname: '%s'\n", __func__, mypath);
    return pathname;
}

char *path_brd_dir(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s";
    static char pathname[PATH_MAX + 1];
    sprintf(pathname, fmt, base, args->prj, args->brd);
    //printf("path.c: %s:pathname: '%s'\n", __func__, mypath);
    return pathname;
}

char *path_task_dir(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s/%s";
    static char pathname[PATH_MAX + 1];
    sprintf(pathname, fmt, base, args->prj, args->brd, args->task);
    //printf("path.c: %s:pathname: '%s'\n", __func__, mypath);
    return pathname;
}

/* Generate path for unit files.  */
char *path_prj_unit(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/.tman/units";
    static char pathname[PATH_MAX + 1];
    sprintf(pathname, fmt, base, args->prj);
    //printf("path.c: %s:pathname: '%s'\n", __func__, mypath);
    return pathname;
}

char *path_brd_unit(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s/.tman/units";
    static char pathname[PATH_MAX + 1];
    sprintf(pathname, fmt, base, args->prj, args->brd);
    //printf("path.c: %s:pathname: '%s'\n", __func__, mypath);
    return pathname;
}

char *path_task_unit(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s/%s/.tman/units";
    static char pathname[PATH_MAX + 1];
    sprintf(pathname, fmt, base, args->prj, args->brd, args->task);
    //printf("path.c: %s:pathname: '%s'\n", __func__, mypath);
    return pathname;
}

char *path_column(const char *dirname, const char *obj)
{
    const char *fmt = "%s/%s/.tman/column";
    static char pathname[PATH_MAX + 1];
    sprintf(pathname, fmt, dirname, obj);
    //printf("path.c: %s:pathname: '%s'\n", __func__, mypath);
    return pathname;
}
