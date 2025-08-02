#include <stdio.h>
#include <string.h>
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

static struct tmanstruct tmanfs;

static int check_args(tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((status = tman_check_arg_brd(args)))
        return status;
    else if ((status = tman_check_arg_task(args)))
        return status;
    return status;
}

static int project_check_board(char *base, tman_arg_t * args)
{
    char *pathname = path_prj_board(base, args);
    return ISFILE(pathname);
}

static int project_add_board(char *base, tman_arg_t * args)
{
    char *pathname = path_prj_board(base, args);
    return TOUCH(pathname);
}

static int project_exist(char *base, tman_arg_t * args)
{
    char *pathname = path_prj_dir(base, args);
    return ISDIR(pathname);
}

static int board_exist(char *base, tman_arg_t * args)
{
    char *pathname = path_brd_dir(base, args);
    return ISDIR(pathname);
}

static int task_exist(char *base, const tman_arg_t * args)
{
    char *pathname = path_task_dir(base, args);
    return ISDIR(pathname);
}

static int fill_sysvars(tman_base_t * base)
{
    sprintf(tmanfs.base, "%s", base->task);
    sprintf(tmanfs.db, "%s/%s", base->task, ".tman");
    return LIBTMAN_OK;
}

int tman_check_arg_brd_exist(tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((status = tman_check_arg_brd(args)))
        return status;
    else if (board_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_BRD_NOSUCH);
    return LIBTMAN_OK;
}

int tman_check_arg_task_exist(tman_arg_t * args)
{
    int status;

    if ((status = check_args(args)))
        return emod_set(status);
    if (task_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_ID_NOSUCH);
    return LIBTMAN_OK;
}

int tman_check_arg_prj(tman_arg_t * args)
{
    if (valid_project_name(args->project) == FALSE)
        return emod_set(LIBTMAN_PRJ_ILLEG);
    else if (project_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_PRJ_NOSUCH);
    else if (project_check_board(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_PRJ_BOARD_CHECK);
    return LIBTMAN_OK;
}

int tman_check_arg_brd(tman_arg_t * args)
{
    if (valid_board_name(args->board) == FALSE)
        return emod_set(LIBTMAN_BRD_ILLEG);
    else if (board_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_BRD_NOSUCH);
    return LIBTMAN_OK;
}

int tman_check_arg_task(tman_arg_t * args)
{
    if (valid_task_name(args->taskid) == FALSE)
        return emod_set(LIBTMAN_ID_ILLEG);
    else if (task_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_ID_NOSUCH);
    return LIBTMAN_OK;
}

int tman_make_db(void)
{
    if (MKDIR(tmanfs.base))
        return emod_set(LIBTMAN_SYS_FS);
    else if (MKDIR(tmanfs.db))
        return emod_set(LIBTMAN_SYS_FS);
    return LIBTMAN_OK;
}

int tman_check_db(void)
{
    if (!ISDIR(tmanfs.db))
        return emod_set(LIBTMAN_DB);
    return LIBTMAN_OK;
}

int tman_init(tman_base_t * base)
{
    if (fill_sysvars(base))
        return emod_set(LIBTMAN_ESYSVAR);
    return LIBTMAN_OK;
}

int tman_task_add(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return emod_set(status);
    else if ((status = tman_check_arg_brd(args)))
        return emod_set(status);
    else if (valid_task_name(args->taskid) == FALSE)
        return emod_set(LIBTMAN_ID_ILLEG);
    else if (task_exist(tmanfs.base, args) == TRUE)
        return emod_set(LIBTMAN_ID_EXISTS);

    if (dir_task_add(tmanfs.base, args) != 0)
        return emod_set(LIBTMAN_DIR_ID_MAKE);
    else if (unit_save(path_task_unit(tmanfs.base, args), ctx->units))
        return emod_set(LIBTMAN_UNIT_SAVE);
    else if (unit_save(path_task_column(tmanfs.base, args), ctx->column))
        return emod_set(LIBTMAN_COLUMN_SAVE);
    return LIBTMAN_OK;
}

int tman_task_del(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = check_args(args)))
        return status;

    if (dir_task_del(tmanfs.base, args))
        return emod_set(LIBTMAN_DIR_ID_DEL);
    return LIBTMAN_OK;
}

int tman_task_get(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = check_args(args)))
        return status;

    if ((ctx->units = unit_load(path_task_unit(tmanfs.base, args))) == NULL)
        status = LIBTMAN_UNIT_GET;
    return status;
}

int tman_task_list(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)) != LIBTMAN_OK)
        return status;
    else if ((status = tman_check_arg_brd(args)) != LIBTMAN_OK)
        return status;
    return aux_list_get(ctx, path_brd_dir(tmanfs.base, args));
}

int tman_task_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst)
{
    int status;

    if ((status = check_args(src)))
        return emod_set(status);
    else if (valid_task_name(dst->taskid) == FALSE)
        return emod_set(LIBTMAN_ID_ILLEG);
    else if (task_exist(tmanfs.base, dst) == TRUE)
        return emod_set(LIBTMAN_ID_EXISTS);

    if (dir_task_move(tmanfs.base, src, dst))
        return emod_set(LIBTMAN_DIR_ID_MOVE);
    return LIBTMAN_OK;
}

int tman_task_set(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = check_args(args)))
        return status;
    return aux_unit_set(ctx, path_task_unit(tmanfs.base, args));
}

int tman_task_column_get(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = check_args(args)))
        return status;

    if ((ctx->column = unit_load(path_task_column(tmanfs.base, args))) == NULL)
        status = LIBTMAN_COLUMN_GET;

    return status;
}

int tman_task_column_set(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = check_args(args)))
        return status;
    return aux_column_set(ctx, path_task_column(tmanfs.base, args));
}

int tman_board_add(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return emod_set(status);
    else if (valid_board_name(args->project) == FALSE)
        return emod_set(LIBTMAN_BRD_ILLEG);
    else if (board_exist(tmanfs.base, args) == TRUE)
        return emod_set(LIBTMAN_BRD_EXISTS);

    if (dir_brd_add(tmanfs.base, args))
        return emod_set(LIBTMAN_DIR_BRD_MAKE);
    else if (unit_save(path_brd_unit(tmanfs.base, args), ctx->units))
        return emod_set(LIBTMAN_UNIT_SAVE);
    else if (unit_save(path_brd_column(tmanfs.base, args), ctx->column))
        return emod_set(LIBTMAN_COLUMN_SAVE);
    return LIBTMAN_OK;
}

int tman_board_del(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((status = tman_check_arg_brd(args)))
        return status;

    if (dir_brd_del(tmanfs.base, args))
        return emod_set(LIBTMAN_DIR_BRD_DEL);
    return LIBTMAN_OK;
}

int tman_board_get(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((status = tman_check_arg_brd(args)))
        return status;

    if ((ctx->units = unit_load(path_brd_unit(tmanfs.base, args))) == NULL)
        return emod_set(LIBTMAN_UNIT_LOAD);
    return LIBTMAN_OK;
}

int tman_board_list(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)) != LIBTMAN_OK)
        return status;
    else if ((status = tman_check_arg_brd(args)) != LIBTMAN_OK)
        return status;
    return aux_list_get(ctx, path_prj_dir(tmanfs.base, args));
}

int tman_board_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst)
{
    return 0;
}

int tman_board_set(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = check_args(args)))
        return status;
    return aux_unit_set(ctx, path_brd_unit(tmanfs.base, args));
}

int tman_board_column_set(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((status = tman_check_arg_brd(args)))
        return status;

    if (unit_save(path_brd_column(tmanfs.base, args), ctx->column))
        return emod_set(LIBTMAN_UNIT_SAVE);
    return LIBTMAN_OK;
}

int tman_project_add(tman_ctx_t * ctx, tman_arg_t * args)
{
    if (valid_project_name(args->project) == FALSE)
        return emod_set(LIBTMAN_PRJ_ILLEG);
    else if (project_exist(tmanfs.base, args) == TRUE)
        return emod_set(LIBTMAN_PRJ_EXISTS);
    // TODO: check board support

    if (dir_prj_add(tmanfs.base, args))
        return emod_set(LIBTMAN_DIR_PRJ_MAKE);
    else if (project_add_board(tmanfs.base, args))
        return emod_set(LIBTMAN_PRJ_BOARD_ADD);
    else if (unit_save(path_prj_unit(tmanfs.base, args), ctx->units))
        return emod_set(LIBTMAN_UNIT_SAVE);
    else if (unit_save(path_prj_column(tmanfs.base, args), ctx->column))
        return emod_set(LIBTMAN_UNIT_SAVE);
    return LIBTMAN_OK;
}

int tman_project_del(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;

    if (dir_prj_del(tmanfs.base, args))
        return emod_set(LIBTMAN_DIR_PRJ_DEL);
    return LIBTMAN_OK;
}

int tman_project_get(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;

    if ((ctx->units = unit_load(path_prj_unit(tmanfs.base, args))) == NULL)
        return emod_set(LIBTMAN_UNIT_LOAD);
    return LIBTMAN_OK;
}

int tman_project_list(tman_ctx_t * ctx)
{
    return aux_list_get(ctx, tmanfs.base);
}

int tman_project_rename(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst)
{
    int status;

    /* TODO: Use tman_check_arg_prj() to remove code duplication.  */
    if ((status = tman_check_arg_prj(src)))
        return emod_set(status);
    else if ((status = tman_check_arg_prj(dst))
             && status != LIBTMAN_PRJ_NOSUCH)
        return emod_set(status);
    else if (project_exist(tmanfs.base, dst))
        return emod_set(LIBTMAN_PRJ_EXISTS);
    else if (valid_project_name(dst->project) == FALSE)
        return emod_set(LIBTMAN_PRJ_ILLEG);

    if (dir_prj_rename(tmanfs.base, src, dst))
        return emod_set(LIBTMAN_DIR_PRJ_RENAME);
    return LIBTMAN_OK;
}

int tman_project_set(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = check_args(args)))
        return status;
    return aux_unit_set(ctx, path_prj_unit(tmanfs.base, args));
}

int tman_project_column_set(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;

    if (unit_save(path_prj_column(tmanfs.base, args), ctx->column))
        return emod_set(LIBTMAN_UNIT_SAVE);
    return LIBTMAN_OK;
}

tman_unit_t *tman_unit_add(struct tman_unit *head, char *key, char *val)
{
    return unit_add(head, key, val);
}

int tman_unit_set(struct tman_unit *head, char *key, char *val)
{
    return unit_set(head, key, val);
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

void tman_deinit(tman_ctx_t * ctx)
{
    unit_free(ctx->units);
    unit_free(ctx->column);
    list_free(ctx->list);
}
