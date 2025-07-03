#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "dir.h"
#include "board.h"
#include "column.h"
#include "project.h"
#include "unit.h"
#include "libtman.h"
#include "tree.h"
#include "task.h"
#include "common.h"
#include "osdep.h"
#include "errmod.h"
#include "path.h"

// TODO: Make NOT global.
struct tmanstruct tmanfs;

static int check_args(tman_arg_t * args)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((status = tman_check_arg_brd(args)))
        return status;
    else if ((status = tman_check_arg_id(args)))
        return status;
    return status;
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

int tman_check_arg_id_exist(tman_arg_t * args)
{
    if (task_exist(tmanfs.base, args) == FALSE)
        return FALSE;
    return TRUE;
}

int tman_check_arg_id(tman_arg_t * args)
{
    if (args->id == NULL
        && (args->id = task_getcurr(tmanfs.base, args)) == NULL) {
        fprintf(stderr, "tman_check_arg_id  ------ 1\n");
        return emod_set(LIBTMAN_ID_NOCURR);
    } else if (task_is_valid_name(args->id) == FALSE)
        return emod_set(LIBTMAN_ID_ILLEG);
    else if (task_is_valid_length(args->id) == FALSE)
        return emod_set(LIBTMAN_ID_TOOLONG);
    else if (task_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_ID_NOSUCH);
    return LIBTMAN_OK;
}

int tman_check_arg_brd(tman_arg_t * args)
{
    if (args->brd == NULL && (args->brd = column_board_curr(args)) == NULL)
        return emod_set(LIBTMAN_BRD_NOCURR);
    /*
       else if (board_is_valid_name(args->id) == FALSE)
       return emod_set(LIBTMAN_ID_ILLEG);
       else if (board_is_valid_length(args->id) == FALSE)
       return emod_set(LIBTMAN_ID_TOOLONG);
     */

    fprintf(stderr, "--------------- %s: %s\n", __FUNCTION__, args->brd);
    if (board_exist(tmanfs.base, args) == FALSE)
        return emod_set(LIBTMAN_BRD_NOSUCH);
    fprintf(stderr, "--------------- %s\n", __FUNCTION__);
    return LIBTMAN_OK;
}

int tman_check_arg_prj(tman_arg_t * args)
{
    if (args->prj == NULL && (args->prj = column_project_curr(args)) == NULL)
        return emod_set(LIBTMAN_PRJ_NOCURR);
    else if (project_is_valid_name(args->prj) == FALSE)
        return emod_set(LIBTMAN_PRJ_ILLEG);
    else if (project_is_valid_length(args->prj) == FALSE)
        return emod_set(LIBTMAN_PRJ_TOOLONG);
    else if (project_exist(args->prj) == FALSE)
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

    /* Special case: task ID should not exists. If this's a case - let it go. */
    if ((status = check_args(args)) && status != LIBTMAN_ID_NOSUCH)
        return status;
    else if (task_exist(tmanfs.base, args) == TRUE)
        return emod_set(LIBTMAN_ID_EXISTS);

    if (dir_id_add(tmanfs.base, args) != 0)
        return emod_set(LIBTMAN_DIR_ID_MAKE);
    else if (task_add(tmanfs.base, args, COLBLOG))
        return emod_set(LIBTMAN_COL_ADD);
    else if (opts->id_switch && task_move(tmanfs.base, args, COLCURR))
        return emod_set(LIBTMAN_COL_MOVE);
    else if (unit_save(path_task_unit(tmanfs.base, args), ctx->unitbin))
        return emod_set(LIBTMAN_UNIT_SET);
    return LIBTMAN_OK;
}

int tman_task_show(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
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

    /* TODO: make task_move() failure more descriptive.  */

    if ((status = check_args(args)))
        return status;
    else if (task_move(tmanfs.base, args, ctx->colname))
        return emod_set(LIBTMAN_COL_MOVE);
    return LIBTMAN_OK;
}

int tman_task_del(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = check_args(args)))
        return status;

    if (task_del(tmanfs.base, args))
        return emod_set(LIBTMAN_COL_DEL);
    else if (dir_id_del(tmanfs.base, args->prj, args->id))
        return emod_set(LIBTMAN_DIR_ID_DEL);
    return LIBTMAN_OK;
}

// TODO: for task IDs that are invalid, use separate memeber in ctx.
// if invalid then ctx->invIDS = add_id(id);
/*
 * roachme: refactor this shit.
 @param prj char * | NULL (then list the current project)
 @return struct item * | NULL (if error happened)
*/
int tman_task_list(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    DIR *ids;
    int status;
    struct dirent *ent;
    struct tree *node;
    struct tman_unit *units;
    tman_arg_t tmp;

    // TODO: move to to cli part
    /* Free task ID list because it might be called more than once.  */

    node = NULL;
    units = NULL;

    tmp.id = args->id;
    tmp.brd = args->brd;
    tmp.prj = args->prj;

    if ((status = tman_check_arg_prj(&tmp)) != LIBTMAN_OK)
        return status;
    else if ((status = tman_check_arg_brd(&tmp)) != LIBTMAN_OK)
        return status;
    else if ((ids = opendir(path_brd_dir(tmanfs.base, &tmp))) == NULL)
        return emod_set(LIBTMAN_DIR_PRJ_OPEN);

    while ((ent = readdir(ids)) != NULL) {
        tmp.id = ent->d_name;

        if (ent->d_name[0] == '.' || ent->d_type != DT_DIR)
            continue;
        else if (tman_check_arg_id(&tmp)) {
            // TODO: roach: sholud we leave it here? If not then what?..
            fprintf(stderr, "tman_task_list: failed: %s\n", tmp.id);
            continue;
        } else if ((units = unit_load(path_task_unit(tmanfs.base, &tmp))) ==
                   NULL) {
            // TODO: roach: sholud we leave it here? If not then what?..
            // IF builtin units could not get
            fprintf(stderr, "tman_task_list: unit_load: failed\n");
            continue;
        }
        //struct column column = col_getmark(tmp.prj, ent->d_name);
        node = tree_alloc(ent->d_name, 2, unit_get(units, "desc"), "");
        ctx->ids = tree_add(ctx->ids, node);
        unit_free(units);
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
    if (strcmp(src->prj, dst->prj) != 0) {
        if (task_iscurr(tmanfs.base, src))
            task_del(tmanfs.base, src);
        else if (task_isprev(tmanfs.base, src))
            task_del(tmanfs.base, src);
    }

    if (dir_id_rename(tmanfs.base, src->prj, dst->prj, src->id, dst->id))
        return emod_set(LIBTMAN_DIR_ID_REN);
    return LIBTMAN_OK;
}

// TODO: add support to switch to previous task in any project
int tman_task_prev(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;
    return 0;

    if ((status = check_args(args)))
        return status;
    else if (task_getprev(tmanfs.base, args) == NULL)
        return emod_set(LIBTMAN_ID_NOPREV);

    /*
       if (task_swap(tmanfs.base, args))
       return emod_set(LIBTMAN_ID_SWAP);
     */

    /* Get new current task ID explicitly. check_args() won't work, cuz
     * there's args->id == old current task ID.  */
    args->id = task_getcurr(tmanfs.base, args);
    return check_args(args);
}

struct tman_unit *tman_unit_add(struct tman_unit *head, char *key, char *val)
{
    return unit_add(head, key, val);
}

void *tman_unit_free(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    unit_free(ctx->unitbin);
    return ctx->unitbin = NULL;
}

int tman_task_set(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;
    struct tman_unit *item;
    struct tman_unit *units;

    // TODO: move genpath_unit to a variable to not call it twice
    if ((status = check_args(args)))
        return status;
    else if ((units = unit_load(genpath_unit(args->prj, args->id))) == NULL)
        return emod_set(LIBTMAN_NODEF_ERR);

    for (item = ctx->unitbin; item; item = item->next) {
        unit_set(units, item->key, item->val);
    }
    if (unit_save(genpath_unit(args->prj, args->id), units))
        return emod_set(LIBTMAN_UNIT_SET);
    unit_free(units);
    return LIBTMAN_OK;
}

int tman_task_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = check_args(args)))
        return status;
    else if (opts->id_switch == FALSE)
        return LIBTMAN_OK;      /* here's nothing to do.  */

    if (project_addcurr(args->prj))
        return emod_set(LIBTMAN_PRJ_SWITCH);
    else if (task_move(tmanfs.base, args, COLCURR))
        return emod_set(LIBTMAN_ID_SWAP);
    return LIBTMAN_OK;
}

int tman_prj_add(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    /* Special case: project should not exists. If this's a case - let it go. */
    if ((status = tman_check_arg_prj(args)) && status != LIBTMAN_PRJ_NOSUCH)
        return status;
    else if (project_exist(args->prj) == TRUE)
        return emod_set(LIBTMAN_PRJ_EXISTS);

    if (dir_prj_add(tmanfs.base, args->prj))
        return emod_set(LIBTMAN_DIR_PRJ_MAKE);
    else if (opts->prj_switch && column_project_move(args, COLCURR))
        return emod_set(LIBTMAN_PRJ_SWITCH);
    else if (!opts->prj_switch && column_project_add(args, COLBLOG))
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

    if (column_project_del(args))
        return emod_set(LIBTMAN_PRJ_DEL_PREV);
    else if (dir_prj_del(tmanfs.base, args->prj))
        return emod_set(LIBTMAN_DIR_PRJ_DEL);
    return LIBTMAN_OK;
}

/*
 * roachme: Refactor this shit
*/
int tman_prj_list(tman_ctx_t * ctx, tman_opt_t * opts)
{
    DIR *edir;
    struct tman_unit *units;
    struct dirent *ent;
    struct tree *node;
    int colprio = 1;
    char *cprj, *pprj;
    tman_arg_t args;
    char pgnout[PGNOUTSIZ + 1] = { 0 };

    if ((edir = opendir(tmanfs.base)) == NULL)
        return emod_set(LIBTMAN_DIR_PRJ_OPEN);

    /* Free task prj list because it might be called more than once.  */
    tree_free(ctx->prjs);
    ctx->prjs = NULL;

    cprj = project_getcurr(tmanfs.base, &args);
    pprj = project_getprev(tmanfs.base, &args);
    while ((ent = readdir(edir)) != NULL) {
        args.prj = ent->d_name;

        if (ent->d_name[0] == '.' || ent->d_type != DT_DIR)
            continue;
        else if (tman_check_arg_prj(&args)) {
            // TODO: roach: sholud we leave it here? If not then what?..
            continue;
        } else if ((units = unit_load(path_prj_unit(tmanfs.base, &args))) ==
                   NULL) {
            fprintf(stderr, "tman_prj_list:unit: error\n");
            // TODO: roach: sholud we leave it here? If not then what?..
            // IF builtin units could not get
            continue;
        }
        // TODO: unify this shit
        if (cprj != NULL && strncmp(cprj, ent->d_name, PRJSIZ) == 0)
            colprio = 1;
        else if (pprj != NULL && strncmp(pprj, ent->d_name, PRJSIZ) == 0)
            colprio = 2;
        else
            colprio = 3;
        node =
            tree_alloc(ent->d_name, colprio, unit_get(units, "desc"), pgnout);
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

    args.brd = args.id = NULL;
    if ((args.prj = column_project_curr(&args))
        && (status = tman_check_arg_prj(&args)))
        return status;
    if ((args.prj = column_project_prev(&args))
        && (status = tman_check_arg_prj(&args)))
        return status;
    if (column_project_swap(&args))
        return emod_set(LIBTMAN_PRJ_SWAP);
    return LIBTMAN_OK;
}

int tman_prj_rename(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst)
{
    int status;
    // roach
    fprintf(stderr, "tman_prj_rename: ADD function column_project_is_curr()\n");
    return 0;

    /* TODO: Use tman_check_arg_prj() to remove code duplication.  */
    if (src->prj == NULL)
        return emod_set(LIBTMAN_PRJ_MISSING);
    else if (dst->prj == NULL)
        return emod_set(LIBTMAN_PRJ_MISSING);
    else if ((status = tman_check_arg_prj(src)))
        return emod_set(status);
    else if ((status = tman_check_arg_prj(dst)) && status != LIBTMAN_PRJ_NOSUCH)
        return emod_set(status);
    else if (project_exist(dst->prj))
        return emod_set(LIBTMAN_PRJ_EXISTS);
    else if (project_is_valid_name(dst->prj) != TRUE)
        return emod_set(LIBTMAN_PRJ_ILLEG);
    else if (project_is_valid_length(dst->prj) == FALSE)
        return emod_set(LIBTMAN_PRJ_TOOLONG);

    if (project_is_curr(src->prj) == TRUE) {
        project_delcurr();
        project_addcurr(dst->prj);
    } else if (project_is_prev(src->prj) == TRUE) {
        project_delprev();
        project_addcurr(dst->prj);
        project_swap();
    }
    return dir_prj_rename(tmanfs.base, src->prj, dst->prj);
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

int tman_prj_show(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if ((ctx->unitbin =
              unit_load(path_prj_unit(tmanfs.base, args))) == NULL)
        return emod_set(LIBTMAN_NODEF_ERR);
    return LIBTMAN_OK;
}

int tman_prj_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    if ((status = tman_check_arg_prj(args)))
        return status;
    else if (opts->prj_switch == FALSE)
        return LIBTMAN_OK;

    if (column_project_add(args, COLCURR))
        return emod_set(LIBTMAN_PRJ_SWITCH);
    return LIBTMAN_OK;
}

int tman_brd_add(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    int status;

    /* Special case: board should not exists. If this's a case - let it go. */
    if ((status = tman_check_arg_prj(args)))
        return status;
    if ((status = tman_check_arg_brd(args)) && status != LIBTMAN_BRD_NOSUCH)
        return status;
    if (board_exist(tmanfs.base, args) == TRUE)
        return emod_set(LIBTMAN_BRD_EXISTS);

    if (dir_brd_add(tmanfs.base, args->prj, args->brd))
        return emod_set(LIBTMAN_NODEF_ERR);
    else if (opts->brd_switch && column_board_move(args, COLCURR))
        return emod_set(LIBTMAN_NODEF_ERR);
    else if (!opts->brd_switch && column_board_move(args, COLBLOG))
        return emod_set(LIBTMAN_NODEF_ERR);
    else if (unit_save(path_brd_unit(tmanfs.base, args), ctx->unitbrd))
        return emod_set(LIBTMAN_NODEF_ERR);
    return LIBTMAN_OK;
}

int tman_brd_del(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    return 0;
}

int tman_brd_list(tman_ctx_t * ctx, tman_opt_t * opts)
{
    return 0;
}

int tman_brd_prev(tman_ctx_t * ctx, tman_opt_t * opts)
{
    return 0;
}

int tman_brd_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst)
{
    return 0;
}

int tman_brd_set(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    return 0;
}

int tman_brd_show(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    return 0;
}

int tman_brd_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts)
{
    return 0;
}

const char *tman_strerror(void)
{
    return emod_strerror();
}

tman_ctx_t *tman_deinit(tman_ctx_t * ctx)
{
    unit_free(ctx->unitbin);
    unit_free(ctx->unitpgn);
    tree_free(ctx->ids);
    tree_free(ctx->prjs);
    free(ctx);
    return NULL;
}
