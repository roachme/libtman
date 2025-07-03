#include <stdio.h>

#include "dir.h"
#include "common.h"
#include "osdep.h"

static int _task_dbmkdir(char *base, tman_arg_t * args)
{
    char path[PATHSIZ + 1];
    const char *fmt = "%s/%s/%s/%s/.tman";
    sprintf(path, fmt, base, args->prj, args->brd, args->task);
    return MKDIR(path);
}

static int _brd_dbmkdir(char *base, tman_arg_t * args)
{
    char path[PATHSIZ + 1];
    sprintf(path, "%s/%s/%s/.tman", base, args->prj, args->brd);
    return MKDIR(path);
}

static int _prj_dbmkdir(char *base, tman_arg_t * args)
{
    char path[PATHSIZ + 1];
    sprintf(path, "%s/%s/.tman", base, args->prj);
    return MKDIR(path);
}

int dir_task_add(char *base, tman_arg_t * args)
{
    char path[PATHSIZ + 1];
    sprintf(path, "%s/%s/%s/%s", base, args->prj, args->brd, args->task);
    return !(MKDIR(path) == 0 && _task_dbmkdir(base, args) == 0);
}

int dir_task_del(char *base, tman_arg_t * args)
{
    char path[BUFSIZ + 1];
    sprintf(path, "%s/%s/%s/%s", base, args->prj, args->brd, args->task);
    return RMDIR(path);
}

int dir_task_rename(char *base, tman_arg_t * src, tman_arg_t * dst)
{
    char new_path[PATHSIZ + 1];
    char old_path[PATHSIZ + 1];
    sprintf(old_path, "%s/%s/%s/%s", base, src->prj, src->brd, src->task);
    sprintf(new_path, "%s/%s/%s/%s", base, dst->prj, dst->brd, dst->task);
    return rename(old_path, new_path);
}

int dir_brd_add(char *base, tman_arg_t * args)
{
    char path[PATHSIZ + 1];
    sprintf(path, "%s/%s/%s", base, args->prj, args->brd);
    return !(MKDIR(path) == 0 && _brd_dbmkdir(base, args) == 0);
}

int dir_brd_del(char *base, tman_arg_t * args)
{
    char path[PATHSIZ + 1];
    sprintf(path, "%s/%s/%s", base, args->prj, args->brd);
    return RMDIR(path);
}

int dir_prj_add(char *base, tman_arg_t * args)
{
    char path[PATHSIZ + 1];
    sprintf(path, "%s/%s/", base, args->prj);
    return !(MKDIR(path) == 0 && _prj_dbmkdir(base, args) == 0);
}

int dir_prj_del(char *base, tman_arg_t * args)
{
    char path[PATHSIZ + 1];
    sprintf(path, "%s/%s", base, args->prj);
    return RMDIR(path);
}

int dir_prj_rename(char *base, tman_arg_t * src, tman_arg_t * dst)
{
    char srcpath[PATHSIZ + 1];
    char dstpath[PATHSIZ + 1];

    sprintf(srcpath, "%s/%s", base, src->prj);
    sprintf(dstpath, "%s/%s", base, dst->prj);
    return MOVE(srcpath, dstpath);
}
