#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "aux.h"
#include "dir.h"
#include "unit.h"
#include "list.h"
#include "path.h"
#include "valid.h"
#include "osdep.h"
#include "errmod.h"
#include "common.h"
#include "libtman.h"

struct tmanstruct {
    char db[PATH_MAX + 1];      /* directory for tman metadata */
    char base[PATH_MAX + 1];    /* directory for all stuff above */
};

static int fill_sysvars(const char *taskdir, struct tmanstruct *tmanfs)
{
    sprintf(tmanfs->base, "%s", taskdir);
    sprintf(tmanfs->db, "%s/%s", taskdir, ".tman");
    return LIBTMAN_OK;
}

int tman_project_exist(const char *taskdir, tman_arg_t * args)
{
    char *pathname = path_project_dir(taskdir, args);
    return ISDIR(pathname) == TRUE ? LIBTMAN_OK : LIBTMAN_PRJ_NOSUCH;
}

int tman_board_exist(const char *taskdir, tman_arg_t * args)
{
    char *pathname = path_board_dir(taskdir, args);
    return ISDIR(pathname) == TRUE ? LIBTMAN_OK : LIBTMAN_BRD_NOSUCH;
}

int tman_task_exist(const char *taskdir, tman_arg_t * args)
{
    char *pathname = path_task_dir(taskdir, args);
    return ISDIR(pathname) == TRUE ? LIBTMAN_OK : LIBTMAN_ID_NOSUCH;
}

int tman_project_valid(const char *taskdir, tman_arg_t * args)
{
    if (valid_project_name(args->project) == FALSE)
        return emod_set(LIBTMAN_PRJ_ILLEG);
    return LIBTMAN_OK;
}

int tman_board_valid(const char *taskdir, tman_arg_t * args)
{
    if (valid_board_name(args->board) == FALSE)
        return emod_set(LIBTMAN_BRD_ILLEG);
    return LIBTMAN_OK;
}

int tman_task_valid(const char *taskdir, tman_arg_t * args)
{
    if (valid_task_name(args->taskid) == FALSE)
        return emod_set(LIBTMAN_ID_ILLEG);
    return LIBTMAN_OK;
}

int tman_make_db(const char *taskdir)
{
    struct tmanstruct tmanfs;

    if (fill_sysvars(taskdir, &tmanfs))
        return emod_set(LIBTMAN_ESYSVAR);
    else if (MKDIR(tmanfs.base))
        return emod_set(LIBTMAN_SYS_FS);
    else if (MKDIR(tmanfs.db))
        return emod_set(LIBTMAN_SYS_FS);
    return LIBTMAN_OK;
}

int tman_check_db(const char *taskdir)
{
    struct tmanstruct tmanfs;

    if (fill_sysvars(taskdir, &tmanfs))
        return emod_set(LIBTMAN_ESYSVAR);
    else if (!ISDIR(tmanfs.db))
        return emod_set(LIBTMAN_DB);
    return LIBTMAN_OK;
}

int tman_task_add(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    if (dir_task_add(taskdir, args) != 0)
        return emod_set(LIBTMAN_DIR_ID_MAKE);
    else if (unit_save(path_task_unit(taskdir, args), ctx->units))
        return emod_set(LIBTMAN_UNIT_SAVE);
    else if (unit_save(path_task_column(taskdir, args), ctx->column))
        return emod_set(LIBTMAN_COLUMN_SAVE);
    return LIBTMAN_OK;
}

int tman_task_del(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    if (dir_task_del(taskdir, args))
        return emod_set(LIBTMAN_DIR_ID_DEL);
    return LIBTMAN_OK;
}

int tman_task_get(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    if ((ctx->units = unit_load(path_task_unit(taskdir, args))) == NULL)
        return emod_set(LIBTMAN_DIR_ID_DEL);
    return LIBTMAN_OK;
}

int tman_task_list(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    return aux_list_get(ctx, path_board_dir(taskdir, args));
}

int tman_task_move(const char *taskdir, tman_arg_t * src, tman_arg_t * dst,
                   tman_ctx_t * ctx)
{
    if (dir_task_move(taskdir, src, dst))
        return emod_set(LIBTMAN_DIR_ID_MOVE);
    return LIBTMAN_OK;
}

int tman_task_set(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    return aux_unit_set(ctx->units, path_task_unit(taskdir, args));
}

int tman_task_column_get(const char *taskdir, tman_arg_t * args,
                         tman_ctx_t * ctx)
{
    if ((ctx->column = unit_load(path_task_column(taskdir, args))) == NULL)
        return LIBTMAN_COLUMN_GET;
    return LIBTMAN_OK;
}

int tman_task_column_set(const char *taskdir, tman_arg_t * args,
                         tman_ctx_t * ctx)
{
    return aux_unit_set(ctx->column, path_task_column(taskdir, args));
}

int tman_board_add(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    if (dir_board_add(taskdir, args))
        return emod_set(LIBTMAN_DIR_BRD_MAKE);
    else if (unit_save(path_board_unit(taskdir, args), ctx->units))
        return emod_set(LIBTMAN_UNIT_SAVE);
    else if (unit_save(path_board_column(taskdir, args), ctx->column))
        return emod_set(LIBTMAN_COLUMN_SAVE);
    return LIBTMAN_OK;
}

int tman_board_del(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    if (dir_board_del(taskdir, args))
        return emod_set(LIBTMAN_DIR_BRD_DEL);
    return LIBTMAN_OK;
}

int tman_board_get(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    if ((ctx->units = unit_load(path_board_unit(taskdir, args))) == NULL)
        return emod_set(LIBTMAN_UNIT_LOAD);
    return LIBTMAN_OK;
}

int tman_board_list(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    return aux_list_get(ctx, path_project_dir(taskdir, args));
}

int tman_board_move(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    return 0;
}

int tman_board_set(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    return aux_unit_set(ctx->units, path_board_unit(taskdir, args));
}

int tman_board_column_set(const char *taskdir, tman_arg_t * args,
                          tman_ctx_t * ctx)
{
    if (unit_save(path_board_column(taskdir, args), ctx->column))
        return emod_set(LIBTMAN_UNIT_SAVE);
    return LIBTMAN_OK;
}

int tman_project_add(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    if (dir_project_add(taskdir, args))
        return emod_set(LIBTMAN_DIR_PRJ_MAKE);
    else if (unit_save(path_project_unit(taskdir, args), ctx->units))
        return emod_set(LIBTMAN_UNIT_SAVE);
    else if (unit_save(path_project_column(taskdir, args), ctx->column))
        return emod_set(LIBTMAN_UNIT_SAVE);
    return LIBTMAN_OK;
}

int tman_project_del(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    if (dir_project_del(taskdir, args))
        return emod_set(LIBTMAN_DIR_PRJ_DEL);
    return LIBTMAN_OK;
}

int tman_project_get(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    if ((ctx->units = unit_load(path_project_unit(taskdir, args))) == NULL)
        return emod_set(LIBTMAN_UNIT_LOAD);
    return LIBTMAN_OK;
}

int tman_project_list(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    return aux_list_get(ctx, taskdir);
}

int tman_project_rename(const char *taskdir, tman_arg_t * src, tman_arg_t * dst,
                        tman_ctx_t * ctx)
{
    if (dir_project_rename(taskdir, src, dst))
        return emod_set(LIBTMAN_DIR_PRJ_RENAME);
    return LIBTMAN_OK;
}

int tman_project_set(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx)
{
    return aux_unit_set(ctx->units, path_project_unit(taskdir, args));
}

int tman_project_column_set(const char *taskdir, tman_arg_t * args,
                            tman_ctx_t * ctx)
{
    if (unit_save(path_project_column(taskdir, args), ctx->column))
        return emod_set(LIBTMAN_UNIT_SAVE);
    return LIBTMAN_OK;
}

tman_unit_t *tman_unit_add(struct tman_unit *head, char *key, char *val)
{
    return unit_add(head, key, val);
}

int tman_unit_set(struct tman_unit *head, char *key, char *val)
{
    // TODO: this will never fail, cuz in case of error it returns head.
    return !unit_set(head, key, val) ? LIBTMAN_OK : LIBTMAN_UNIT_SET;
}

int tman_unit_save(const char *filename, tman_unit_t * units)
{
    return unit_save(filename, units);
}

tman_unit_t *tman_unit_parse(struct tman_unit *head, const char *str)
{
    return unit_parse(head, str);
}

char *tman_unit_key(tman_unit_t * head, char *key)
{
    return unit_get(head, key);
}

void *tman_unit_free(tman_unit_t * units)
{
    unit_free(units);
    return NULL;
}

void *tman_list_free(tman_list_t * list)
{
    list_free(list);
    return NULL;
}

char *tman_strerror(int errnum)
{
    return emod_strerror(errnum);
}
