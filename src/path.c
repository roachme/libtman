#include <stdio.h>
#include <limits.h>
#include <stdarg.h>

#include "path.h"

char *path_generic(char *buf, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    return buf;
}

char *path_prj_board(const char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/.tman/useboard";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, base, args->project);
}

char *path_prj_dir(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, base, args->project);
}

char *path_brd_dir(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, base, args->project, args->board);
}

char *path_task_dir(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s/%s";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, base, args->project, args->board,
                        args->taskid);
}

/* Generate path for unit files.  */
char *path_prj_unit(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/.tman/units";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, base, args->project);
}

char *path_brd_unit(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s/.tman/units";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, base, args->project, args->board);
}

char *path_task_unit(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s/%s/.tman/units";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, base, args->project, args->board,
                        args->taskid);
}

char *path_prj_column(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/.tman/column";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, base, args->project);
}

char *path_brd_column(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s/.tman/column";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, base, args->project, args->board);
}

char *path_task_column(char *base, const tman_arg_t * args)
{
    const char *fmt = "%s/%s/%s/%s/.tman/column";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, base, args->project, args->board,
                        args->taskid);
}
