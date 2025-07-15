#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "dir.h"
#include "column.h"
#include "limit.h"
#include "unit.h"
#include "libtman.h"
#include "tree.h"
#include "common.h"
#include "osdep.h"
#include "errmod.h"
#include "path.h"
#include "valid.h"

static struct tmanstruct tmanfs;

typedef struct mylist mylist_t;

static void tman_mylist_free(struct mylist *head)
{
    struct mylist *p, *q;

    for (p = head; p != NULL; p = q) {
        q = p->next;
        free(p->id);
        if (p->colname)
            free(p->colname);
        free(p);
    }
}

static mylist_t *add_list_node(mylist_t * head, char *id, char *colname,
                               int status)
{
    struct mylist *mylist;
    mylist = malloc(sizeof(struct mylist));
    mylist->id = strdup(id);
    if (colname != NULL)
        mylist->colname = strdup(colname);
    else
        mylist->colname = NULL;
    mylist->shown = FALSE;
    mylist->status = status;
    mylist->next = head;
    return mylist;
}

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
    sprintf(tmanfs.pgn, "%s", base->pgn);
    return LIBTMAN_OK;
}

int tman_check_arg_task_exist(tman_arg_t * args)
{
    if (task_exist(tmanfs.base, args) == FALSE)
        return FALSE;
    return TRUE;
}

int tman_check_arg_task(tman_arg_t * args)
{
    if (args->task == NULL
        && (args->task = column_task_curr(tmanfs.base, args)) == NULL)
        return emod_set(LIBTMAN_ID_NOCURR);
    else if (valid_task_name(args->task) == FALSE)
        return emod_set(LIBTMAN_ID_ILLEG);
    else if (limit_task_ok(args->task) == FALSE)
        return emod_set(LIBTMAN_ID_TOOLONG);
    else if (task_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_ID_NOSUCH);
    return LIBTMAN_OK;
}

int tman_check_arg_brd(tman_arg_t * args)
{
    if (args->brd == NULL
        && (args->brd = column_board_curr(tmanfs.base, args)) == NULL)
        return emod_set(LIBTMAN_BRD_NOCURR);
    else if (valid_board_name(args->brd) == FALSE)
        return emod_set(LIBTMAN_BRD_ILLEG);
    else if (limit_board_ok(args->brd) == FALSE)
        return emod_set(LIBTMAN_BRD_TOOLONG);
    else if (board_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_BRD_NOSUCH);
    return LIBTMAN_OK;
}

int tman_check_arg_prj(tman_arg_t * args)
{
    if (args->prj == NULL
        && (args->prj = column_project_curr(tmanfs.base, args)) == NULL)
        return emod_set(LIBTMAN_PRJ_NOCURR);
    else if (valid_project_name(args->prj) == FALSE)
        return emod_set(LIBTMAN_PRJ_ILLEG);
    else if (limit_project_ok(args->prj) == FALSE)
        return emod_set(LIBTMAN_PRJ_TOOLONG);
    else if (project_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_PRJ_NOSUCH);
    return LIBTMAN_OK;
}

int tman_mkfs(void)
{
    if (MKDIR(tmanfs.base))
        return emod_set(LIBTMAN_SYS_FS);
    else if (MKDIR(tmanfs.db))
        return emod_set(LIBTMAN_SYS_FS);
    else if (MKDIR(tmanfs.pgn))
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

int tman_setup(int setuplvl)
{
    int status = LIBTMAN_OK;

    if (setuplvl == LIBTMAN_SETUPSOFT)  /* no filesystem check.  */
        ;
    else if (setuplvl == LIBTMAN_SETUPCHECK) {  /* check filesystem.  */
        if (ISDIR(tmanfs.db) != TRUE)
            status = emod_set(LIBTMAN_EINIT);
    } else {                    /* LIBTMAN_SETUPHARD: create filesystem.  */
        if ((status = tman_mkfs()) != LIBTMAN_OK)
            status = emod_set(LIBTMAN_EFINIT);
    }
    return status;
}

int tman_task_add(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    /* Special case: board should not exists. If this's a case - let it go. */
    if ((status = check_args(args)) != LIBTMAN_ID_NOSUCH)
        return emod_set(LIBTMAN_ID_EXISTS);

    if (dir_task_add(tmanfs.base, args) != 0)
        return emod_set(LIBTMAN_DIR_ID_MAKE);
    else if (opts->task_switch
             && column_task_move(tmanfs.base, args, COLUMN_CURR))
        return emod_set(LIBTMAN_COL_ADD);
    else if (!opts->task_switch
             && column_task_move(tmanfs.base, args, COLUMN_BLOG))
        return emod_set(LIBTMAN_COL_MOVE);
    else if (unit_save(path_task_unit(tmanfs.base, args), ctx->unitbin))
        return emod_set(LIBTMAN_UNIT_SET);
    return LIBTMAN_OK;
}

int tman_task_del(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = check_args(args)))
        return status;

    if (column_task_del(tmanfs.base, args))
        return emod_set(LIBTMAN_COL_DEL);
    else if (dir_task_del(tmanfs.base, args))
        return emod_set(LIBTMAN_DIR_ID_DEL);
    return LIBTMAN_OK;
}

int tman_task_get(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = check_args(args)))
        return status;

    if ((ctx->unitbin = unit_load(path_task_unit(tmanfs.base, args))) == NULL)
        status = LIBTMAN_UNIT_GET;
    return status;
}

int tman_task_flow(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = check_args(args)))
        return status;
    else if (valid_column_name(ctx->colname) == FALSE)
        return emod_set(LIBTMAN_VALID_COLUMN);
    else if (column_task_move(tmanfs.base, args, ctx->colname))
        return emod_set(LIBTMAN_COL_MOVE);
    return LIBTMAN_OK;
}

int tman_task_list(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    DIR *ids;
    int status;
    char *colname, *pathname;
    struct dirent *ent;
    struct tman_unit *units;
    struct objlist *objs;

    if ((objs = calloc(1, sizeof(struct objlist))) == NULL)
        return emod_set(LIBTMAN_MALLOC);
    if ((status = tman_check_arg_prj(args)) != LIBTMAN_OK)
        return status;
    if ((status = tman_check_arg_brd(args)) != LIBTMAN_OK)
        return status;
    if ((pathname = path_brd_dir(tmanfs.base, args)) == NULL)
        return emod_set(LIBTMAN_NODEF_ERR);
    if ((ids = opendir(pathname)) == NULL)
        return emod_set(LIBTMAN_DIR_PRJ_OPEN);

    while ((ent = readdir(ids)) != NULL) {
        args->task = ent->d_name;

        if (ent->d_name[0] == '.' || ent->d_type != DT_DIR)
            continue;
        else if ((status = tman_check_arg_task(args))) {
            objs->inval = add_list_node(objs->inval, args->task, NULL, status);
            continue;
        } else if (!(units = unit_load(path_task_unit(tmanfs.base, args)))) {
            status = LIBTMAN_UNIT_LOAD;
            objs->inval = add_list_node(objs->inval, args->task, NULL, status);
            continue;
        } else if (!(colname = column_task_colname(tmanfs.base, args))) {
            status = LIBTMAN_COL_GET;
            objs->inval = add_list_node(objs->inval, args->task, NULL, status);
            continue;
        }

        if (strcmp(colname, COLUMN_CURR) == 0) {
            status = LIBTMAN_OK;
            objs->curr = add_list_node(objs->curr, args->task, colname, status);
        } else if (strcmp(colname, COLUMN_PREV) == 0) {
            status = LIBTMAN_OK;
            objs->prev = add_list_node(objs->prev, args->task, colname, status);
        } else if (strcmp(colname, COLUMN_BLOG) == 0) {
            status = LIBTMAN_OK;
            objs->blog = add_list_node(objs->blog, args->task, colname, status);
        } else if (strcmp(colname, COLUMN_DONE) == 0) {
            status = LIBTMAN_OK;
            objs->done = add_list_node(objs->done, args->task, colname, status);
        } else {
            status = LIBTMAN_OK;
            char *task = args->task;
            objs->custom = add_list_node(objs->custom, task, colname, status);
        }

        unit_free(units);
    }

    ctx->ids = objs;
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
    if (strcmp(src->prj, dst->prj) != 0 || strcmp(src->brd, dst->brd) != 0)
        column_task_del(tmanfs.base, src);

    if (dir_task_rename(tmanfs.base, src, dst))
        return emod_set(LIBTMAN_DIR_ID_REN);
    return LIBTMAN_OK;
}

// TODO: add support to switch to previous task in any project
int tman_task_prev(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = check_args(args)))
        return status;
    else if (column_task_prev(tmanfs.base, args) == NULL)
        return emod_set(LIBTMAN_ID_NOPREV);

    if (column_task_swap(tmanfs.base, args))
        return emod_set(LIBTMAN_ID_SWAP);

    /* Get new current task ID explicitly. check_args() won't work, cuz
     * there's args->id == old current task ID.  */
    args->task = column_task_curr(tmanfs.base, args);
    return LIBTMAN_OK;
}

int tman_task_set(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;
    char *pathname;
    struct tman_unit *units;

    if ((status = check_args(args)))
        return status;
    else if ((pathname = path_task_unit(tmanfs.base, args)) == NULL)
        return emod_set(LIBTMAN_NODEF_ERR);
    else if ((units = unit_load(pathname)) == NULL)
        return emod_set(LIBTMAN_NODEF_ERR);

    for (struct tman_unit * item = ctx->unitbin; item; item = item->next)
        unit_set(units, item->key, item->val);
    if (unit_save(pathname, units))
        status = LIBTMAN_UNIT_SET;
    unit_free(units);
    return status;
}

int tman_task_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = check_args(args)))
        return status;
    else if (opts->task_switch == FALSE)
        return LIBTMAN_OK;      /* here's nothing to do.  */

    if (column_project_move(tmanfs.base, args, COLUMN_CURR))
        return emod_set(LIBTMAN_PRJ_SWITCH);
    else if (column_board_move(tmanfs.base, args, COLUMN_CURR))
        return emod_set(LIBTMAN_PRJ_SWITCH);
    else if (column_task_move(tmanfs.base, args, COLUMN_CURR))
        return emod_set(LIBTMAN_ID_SWAP);
    return LIBTMAN_OK;
}

int tman_brd_add(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    /* Special case: board should not exists. If this's a case - let it go. */
    else if ((status = tman_check_arg_brd(args)) != LIBTMAN_BRD_NOSUCH)
        return emod_set(LIBTMAN_BRD_EXISTS);

    if (dir_brd_add(tmanfs.base, args))
        return emod_set(LIBTMAN_NODEF_ERR);
    else if (opts->brd_switch
             && column_board_move(tmanfs.base, args, COLUMN_CURR))
        return emod_set(LIBTMAN_NODEF_ERR);
    else if (!opts->brd_switch
             && column_board_move(tmanfs.base, args, COLUMN_BLOG))
        return emod_set(LIBTMAN_NODEF_ERR);
    else if (unit_save(path_brd_unit(tmanfs.base, args), ctx->unitbrd))
        return emod_set(LIBTMAN_NODEF_ERR);
    return LIBTMAN_OK;
}

int tman_brd_del(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    if ((status = tman_check_arg_brd(args)))
        return status;

    if (column_board_del(tmanfs.base, args))
        return emod_set(LIBTMAN_PRJ_DEL_PREV);
    else if (dir_brd_del(tmanfs.base, args))
        return emod_set(LIBTMAN_DIR_PRJ_DEL);
    return LIBTMAN_OK;
}

int tman_brd_get(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((status = tman_check_arg_brd(args)))
        return status;

    if ((ctx->unitbin = unit_load(path_brd_unit(tmanfs.base, args))) == NULL)
        return emod_set(LIBTMAN_NODEF_ERR);
    return LIBTMAN_OK;
}

/*
 * roachme: Refactor this shit
*/
int tman_brd_list(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    DIR *edir;
    char *dirname;
    struct tman_unit *units;
    struct dirent *ent;
    struct tree *node;
    int colprio, status;
    char *brd_curr, *brd_prev;

    if ((status = tman_check_arg_prj(args)))
        return status;
    if ((dirname = path_prj_dir(tmanfs.base, args)) == NULL)
        return emod_set(LIBTMAN_NODEF_ERR);
    if ((edir = opendir(dirname)) == NULL)
        return emod_set(LIBTMAN_DIR_PRJ_OPEN);

    /* Free task prj list because it might be called more than once.  */
    tree_free(ctx->prjs);
    ctx->prjs = NULL;

    brd_curr = column_board_curr(tmanfs.base, args);
    brd_prev = column_board_prev(tmanfs.base, args);
    while ((ent = readdir(edir)) != NULL) {
        args->brd = ent->d_name;

        if (ent->d_name[0] == '.' || ent->d_type != DT_DIR)
            continue;
        else if ((status = tman_check_arg_brd(args))) {
            node = tree_alloc(ent->d_name, 1, "");
            node->status = status;
            ctx->invbrds = tree_add(ctx->invbrds, node);
            continue;
        } else if (!(units = unit_load(path_brd_unit(tmanfs.base, args)))) {
            node = tree_alloc(ent->d_name, 1, "");
            node->status = LIBTMAN_UNIT_GET;
            ctx->invbrds = tree_add(ctx->invbrds, node);
            continue;
        }
        // TODO: unify this shit
        if (brd_curr != NULL && strncmp(brd_curr, ent->d_name, PRJSIZ) == 0)
            colprio = 1;
        else if (brd_prev != NULL
                 && strncmp(brd_prev, ent->d_name, PRJSIZ) == 0)
            colprio = 2;
        else
            colprio = 3;
        node = tree_alloc(ent->d_name, colprio, unit_get(units, "desc"));
        ctx->brds = tree_add(ctx->brds, node);
        unit_free(units);
    }

    closedir(edir);
    return LIBTMAN_OK;
}

int tman_brd_prev(tman_ctx_t * ctx, tman_opt_t * opts)
{
    int status;
    tman_arg_t args;

    args.prj = args.brd = args.task = NULL;
    if ((args.prj = column_project_curr(tmanfs.base, &args))
        && (status = tman_check_arg_prj(&args)))
        return status;
    if ((args.brd = column_board_prev(tmanfs.base, &args))
        && (status = tman_check_arg_brd(&args)))
        return status;
    if (column_board_swap(tmanfs.base, &args))
        return emod_set(LIBTMAN_BRD_SWAP);
    return LIBTMAN_OK;
}

int tman_brd_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst)
{
    return 0;
}

int tman_brd_set(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;
    struct tman_unit *item;
    struct tman_unit *units;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((status = tman_check_arg_brd(args)))
        return status;
    else if ((units = unit_load(path_brd_unit(tmanfs.base, args))) == NULL)
        return emod_set(LIBTMAN_NODEF_ERR);

    for (item = ctx->unitbin; item; item = item->next)
        unit_set(units, item->key, item->val);
    if (unit_save(path_brd_unit(tmanfs.base, args), units))
        return emod_set(LIBTMAN_UNIT_SET);
    // TODO: is it good idea to free it here, or in CLI part?
    unit_free(units);
    return LIBTMAN_OK;
}

int tman_brd_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((status = tman_check_arg_brd(args)))
        return status;
    else if (opts->brd_switch == FALSE)
        return LIBTMAN_OK;

    if (column_project_move(tmanfs.base, args, COLUMN_CURR))
        return emod_set(LIBTMAN_PRJ_SWITCH);
    else if (column_board_move(tmanfs.base, args, COLUMN_CURR))
        return emod_set(LIBTMAN_PRJ_SWITCH);
    return LIBTMAN_OK;
}

int tman_prj_add(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    /* Special case: board should not exists. If this's a case - let it go. */
    if ((status = tman_check_arg_prj(args)) != LIBTMAN_PRJ_NOSUCH)
        return emod_set(LIBTMAN_PRJ_EXISTS);

    if (dir_prj_add(tmanfs.base, args))
        return emod_set(LIBTMAN_DIR_PRJ_MAKE);
    else if (opts->prj_switch
             && column_project_move(tmanfs.base, args, COLUMN_CURR))
        return emod_set(LIBTMAN_PRJ_SWITCH);
    else if (!opts->prj_switch
             && column_project_move(tmanfs.base, args, COLUMN_BLOG))
        return emod_set(LIBTMAN_COL_ADD);
    else if (unit_save(path_prj_unit(tmanfs.base, args), ctx->unitbin))
        return emod_set(LIBTMAN_UNIT_SET);
    return LIBTMAN_OK;
}

int tman_prj_del(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;

    if (column_project_del(tmanfs.base, args))
        return emod_set(LIBTMAN_PRJ_DEL_PREV);
    else if (dir_prj_del(tmanfs.base, args))
        return emod_set(LIBTMAN_DIR_PRJ_DEL);
    return LIBTMAN_OK;
}

int tman_prj_get(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((ctx->unitbin =
              unit_load(path_prj_unit(tmanfs.base, args))) == NULL)
        return emod_set(LIBTMAN_NODEF_ERR);
    return LIBTMAN_OK;
}

/*
 * roachme: Refactor this shit
*/
int tman_prj_list(tman_ctx_t * ctx, tman_opt_t * opts)
{
    DIR *edir;
    int status;
    struct tman_unit *units;
    struct dirent *ent;
    struct tree *node;
    int colprio = 1;
    char *cprj, *pprj;
    tman_arg_t args;

    if ((edir = opendir(tmanfs.base)) == NULL)
        return emod_set(LIBTMAN_DIR_PRJ_OPEN);

    /* Free task prj list because it might be called more than once.  */
    tree_free(ctx->prjs);
    ctx->prjs = NULL;

    cprj = column_project_curr(tmanfs.base, &args);
    pprj = column_project_prev(tmanfs.base, &args);
    while ((ent = readdir(edir)) != NULL) {
        args.prj = ent->d_name;

        if (ent->d_name[0] == '.' || ent->d_type != DT_DIR)
            continue;
        else if ((status = tman_check_arg_prj(&args))) {
            node = tree_alloc(ent->d_name, 1, "");
            node->status = status;
            ctx->invprjs = tree_add(ctx->invprjs, node);
            continue;
        } else if (!(units = unit_load(path_prj_unit(tmanfs.base, &args)))) {
            node = tree_alloc(ent->d_name, 1, "");
            node->status = LIBTMAN_UNIT_GET;
            ctx->invprjs = tree_add(ctx->invprjs, node);
            continue;
        }
        // TODO: unify this shit
        if (cprj != NULL && strncmp(cprj, ent->d_name, PRJSIZ) == 0)
            colprio = 1;
        else if (pprj != NULL && strncmp(pprj, ent->d_name, PRJSIZ) == 0)
            colprio = 2;
        else
            colprio = 3;
        node = tree_alloc(ent->d_name, colprio, unit_get(units, "desc"));
        ctx->prjs = tree_add(ctx->prjs, node);
        unit_free(units);
    }

    closedir(edir);
    return LIBTMAN_OK;
}

int tman_prj_prev(tman_ctx_t * ctx, tman_opt_t * opts)
{
    int status;
    tman_arg_t args;

    args.prj = args.brd = args.task = NULL;
    if ((args.prj = column_project_curr(tmanfs.base, &args))
        && (status = tman_check_arg_prj(&args)))
        return status;
    if ((args.prj = column_project_prev(tmanfs.base, &args))
        && (status = tman_check_arg_prj(&args)))
        return status;
    if (column_project_swap(tmanfs.base, &args))
        return emod_set(LIBTMAN_PRJ_SWAP);
    return LIBTMAN_OK;
}

int tman_prj_rename(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst)
{
    int status;

    /* TODO: Use tman_check_arg_prj() to remove code duplication.  */
    if (src->prj == NULL)
        return emod_set(LIBTMAN_PRJ_MISSING);
    else if (dst->prj == NULL)
        return emod_set(LIBTMAN_PRJ_MISSING);
    else if ((status = tman_check_arg_prj(src)))
        return emod_set(status);
    else if ((status = tman_check_arg_prj(dst))
             && status != LIBTMAN_PRJ_NOSUCH)
        return emod_set(status);
    else if (project_exist(tmanfs.base, dst))
        return emod_set(LIBTMAN_PRJ_EXISTS);
    else if (valid_project_name(dst->prj) == FALSE)
        return emod_set(LIBTMAN_PRJ_ILLEG);
    else if (limit_project_ok(dst->prj) == FALSE)
        return emod_set(LIBTMAN_PRJ_TOOLONG);

    return dir_prj_rename(tmanfs.base, src, dst);
}

int tman_prj_set(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;
    struct tman_unit *item;
    struct tman_unit *units;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((units = unit_load(path_prj_unit(tmanfs.base, args))) == NULL)
        return emod_set(LIBTMAN_NODEF_ERR);

    for (item = ctx->unitbin; item; item = item->next)
        unit_set(units, item->key, item->val);
    if (unit_save(path_prj_unit(tmanfs.base, args), units))
        return emod_set(LIBTMAN_UNIT_SET);
    // TODO: is it good idea to free it here, or in CLI part?
    unit_free(units);
    return LIBTMAN_OK;
}

int tman_prj_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    if (opts->prj_switch == FALSE)
        return LIBTMAN_OK;

    if (column_project_move(tmanfs.base, args, COLUMN_CURR))
        return emod_set(LIBTMAN_PRJ_SWITCH);
    return LIBTMAN_OK;
}

struct tman_unit *tman_unit_add(struct tman_unit *head, char *key, char *val)
{
    return unit_add(head, key, val);
}

char *tman_unit_get_key(tman_unit_t * head, char *key)
{
    return unit_get(head, key);
}

void tman_unit_free(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    unit_free(ctx->unitbin);
    ctx->unitbin = NULL;
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
    unit_free(ctx->unitbin);
    unit_free(ctx->unitpgn);
    tree_free(ctx->brds);
    tree_free(ctx->prjs);

    if (ctx->ids) {
        tman_mylist_free(ctx->ids->inval);
        tman_mylist_free(ctx->ids->curr);
        tman_mylist_free(ctx->ids->prev);
        tman_mylist_free(ctx->ids->blog);
        tman_mylist_free(ctx->ids->done);
        tman_mylist_free(ctx->ids->custom);
        free(ctx->ids);
    }

    free(ctx);
    return NULL;
}
