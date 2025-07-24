#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

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

static tman_ctx_t *make_context(void)
{
    tman_ctx_t *ctx;

    if ((ctx = malloc(sizeof(tman_ctx_t))) == NULL)
        return NULL;

    memset(ctx, 0, sizeof(tman_ctx_t));
    return ctx;
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
    if (valid_project_name(args->prj) == FALSE)
        return emod_set(LIBTMAN_PRJ_ILLEG);
    else if (project_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_PRJ_NOSUCH);
    else if (project_check_board(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_PRJ_BOARD_CHECK);
    return LIBTMAN_OK;
}

int tman_check_arg_brd(tman_arg_t * args)
{
    if (valid_board_name(args->brd) == FALSE)
        return emod_set(LIBTMAN_BRD_ILLEG);
    else if (board_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_BRD_NOSUCH);
    return LIBTMAN_OK;
}

int tman_check_arg_task(tman_arg_t * args)
{
    if (valid_task_name(args->task) == FALSE)
        return emod_set(LIBTMAN_ID_ILLEG);
    else if (task_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_ID_NOSUCH);
    return LIBTMAN_OK;
}

int tman_mkfs(void)
{
    if (MKDIR(tmanfs.base))
        return emod_set(LIBTMAN_SYS_FS);
    else if (MKDIR(tmanfs.db))
        return emod_set(LIBTMAN_SYS_FS);
    return LIBTMAN_OK;
}

tman_ctx_t *tman_init(tman_base_t * base)
{
    if (fill_sysvars(base)) {
        emod_set(LIBTMAN_ESYSVAR);
        return NULL;
    }
    return make_context();
}

int tman_check_db(void)
{
    if (!ISDIR(tmanfs.db))
        return emod_set(LIBTMAN_DB);
    return LIBTMAN_OK;
}

int tman_task_add(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return emod_set(status);
    else if ((status = tman_check_arg_brd(args)))
        return emod_set(status);
    else if (valid_task_name(args->task) == FALSE)
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
    DIR *ids;
    int status;
    struct dirent *ent;

    if ((status = tman_check_arg_prj(args)) != LIBTMAN_OK)
        return status;
    else if ((status = tman_check_arg_brd(args)) != LIBTMAN_OK)
        return status;
    else if ((ids = opendir(path_brd_dir(tmanfs.base, args))) == NULL)
        return emod_set(LIBTMAN_DIR_PRJ_OPEN);

    while ((ent = readdir(ids)) != NULL) {
        status = LIBTMAN_OK;
        args->task = ent->d_name;

        if (ent->d_name[0] == '.' || ent->d_type != DT_DIR)
            continue;
        else if (valid_task_name(args->task) == FALSE)
            status = LIBTMAN_ID_ILLEG;

        ctx->list = list_add(ctx->list, args->task, status);
    }

    closedir(ids);
    return LIBTMAN_OK;
}

int tman_task_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst)
{
    int status;

    if ((status = check_args(src)))
        return emod_set(status);
    else if ((status = check_args(dst)) && status != LIBTMAN_ID_NOSUCH)
        return emod_set(status);
    else if (task_exist(tmanfs.base, dst))
        return emod_set(LIBTMAN_ID_EXISTS);

    /*
     * If move task from project to project, update current and previous tasks
     * in source project. Do it before moving task, cuz there might be
     * multiple current or previous tasks later.
     */
    // roachme: thoroughly test it, cuz moving toggles might cause problems
    /*
       if (strcmp(src->prj, dst->prj) != 0 || strcmp(src->brd, dst->brd) != 0)
       column_task_del(tmanfs.base, src);
     */

    if (dir_task_rename(tmanfs.base, src, dst))
        return emod_set(LIBTMAN_DIR_ID_MOVE);
    return LIBTMAN_OK;
}

int tman_task_set(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;
    char *pathname;
    struct tman_unit *units;

    if ((status = check_args(args)))
        return status;
    else if ((pathname = path_task_unit(tmanfs.base, args)) == NULL)
        return emod_set(LIBTMAN_MALLOC);
    else if ((units = unit_load(pathname)) == NULL)
        return emod_set(LIBTMAN_UNIT_LOAD);

    for (struct tman_unit * item = ctx->units; item; item = item->next)
        unit_set(units, item->key, item->val);
    if (unit_save(pathname, units))
        status = LIBTMAN_UNIT_SAVE;
    unit_free(units);
    return status;
}

int tman_brd_add(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return emod_set(status);
    else if (valid_board_name(args->prj) == FALSE)
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

int tman_brd_del(tman_ctx_t * ctx, tman_arg_t * args)
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

int tman_brd_get(tman_ctx_t * ctx, tman_arg_t * args)
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

/*
 * roachme: Refactor this shit
*/
int tman_brd_list(tman_ctx_t * ctx, tman_arg_t * args)
{
    fprintf(stderr, "under development\n");
    return 1;
}

int tman_brd_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst)
{
    return 0;
}

int tman_brd_set(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;
    struct tman_unit *item;
    struct tman_unit *units;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((status = tman_check_arg_brd(args)))
        return status;
    else if ((units = unit_load(path_brd_unit(tmanfs.base, args))) == NULL)
        return emod_set(LIBTMAN_UNIT_LOAD);

    for (item = ctx->units; item; item = item->next)
        unit_set(units, item->key, item->val);
    if (unit_save(path_brd_unit(tmanfs.base, args), units))
        return emod_set(LIBTMAN_UNIT_SAVE);
    return LIBTMAN_OK;
}

int tman_brd_sync(tman_ctx_t * ctx, tman_arg_t * args)
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

int tman_prj_add(tman_ctx_t * ctx, tman_arg_t * args)
{
    if (valid_project_name(args->prj) == FALSE)
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

int tman_prj_del(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;

    if (dir_prj_del(tmanfs.base, args))
        return emod_set(LIBTMAN_DIR_PRJ_DEL);
    return LIBTMAN_OK;
}

int tman_prj_get(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;

    if ((ctx->units = unit_load(path_prj_unit(tmanfs.base, args))) == NULL)
        return emod_set(LIBTMAN_UNIT_LOAD);
    return LIBTMAN_OK;
}

int tman_prj_list(tman_ctx_t * ctx)
{
    fprintf(stderr, "under development\n");
    return 1;
}

int tman_prj_rename(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst)
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
    else if (valid_project_name(dst->prj) == FALSE)
        return emod_set(LIBTMAN_PRJ_ILLEG);

    return dir_prj_rename(tmanfs.base, src, dst);
}

int tman_prj_set(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;
    struct tman_unit *item;
    struct tman_unit *units;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((units = unit_load(path_prj_unit(tmanfs.base, args))) == NULL)
        return emod_set(LIBTMAN_UNIT_LOAD);

    for (item = ctx->units; item; item = item->next)
        unit_set(units, item->key, item->val);
    if (unit_save(path_prj_unit(tmanfs.base, args), units))
        return emod_set(LIBTMAN_UNIT_SAVE);
    return LIBTMAN_OK;
}

int tman_prj_sync(tman_ctx_t * ctx, tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;

    if (unit_save(path_prj_column(tmanfs.base, args), ctx->column))
        return emod_set(LIBTMAN_UNIT_SAVE);
    return LIBTMAN_OK;
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
    char *pathname;
    struct tman_unit *item;
    struct tman_unit *column;

    if ((status = check_args(args)))
        return status;
    else if ((pathname = path_task_column(tmanfs.base, args)) == NULL)
        return emod_set(LIBTMAN_MALLOC);
    else if ((column = unit_load(pathname)) == NULL)
        return emod_set(LIBTMAN_UNIT_LOAD);
    for (item = ctx->column; item; item = item->next)
        if (valid_column_name(item->key) == FALSE)
            return emod_set(LIBTMAN_COLUMN_KEY);

    for (item = ctx->column; item; item = item->next)
        unit_set(column, item->key, item->val);
    if (unit_save(pathname, column))
        status = LIBTMAN_COLUMN_SAVE;
    return status;
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

const char *tman_strerror(void)
{
    return emod_strerror();
}

const char *tman_strerror_get(int status)
{
    return emod_strerror_get(status);
}

tman_ctx_t *tman_deinit(tman_ctx_t * ctx)
{
    unit_free(ctx->units);
    unit_free(ctx->column);
    list_free(ctx->list);
    free(ctx);
    return NULL;
}
