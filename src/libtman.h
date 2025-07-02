#ifndef LIBTMAN_H
#define LIBTMAN_H

#include "errmod.h"
#include "tree.h"

typedef struct tman_arg tman_arg_t;
typedef struct tman_base tman_base_t;
typedef struct tman_unit tman_unit_t;
typedef struct tman_option tman_opt_t;
typedef struct tman_context tman_ctx_t;

struct tman_arg {
    char *id;
    char *brd;
    char *prj;
};

struct tman_base {
    char *pgn;                  /* directory where plugins are installed */
    char *task;                 /* directory where tasks are stored */
};

struct tman_unit {
    char *key;
    char *val;
    struct tman_unit *next;
};

struct tman_option {
    int id_switch;
    int brd_switch;
    int prj_switch;
};

/* TODO: maybe it's better to move it cli part?  */
enum tman_setuplvl {
    LIBTMAN_SETUPSOFT,
    LIBTMAN_SETUPCHECK,
    LIBTMAN_SETUPHARD,
};

/* Generic tman structure used by all(?) API functions.  */
struct tman_context {
    char *colname;
    struct tman_unit *unitbin;
    struct tman_unit *unitpgn;
    struct tree *ids;
    struct tree *prjs;
};

/* Core functions.  */
int tman_setup(int setuplvl);
tman_ctx_t *tman_init(tman_base_t * structure);
tman_ctx_t *tman_deinit(tman_ctx_t * ctx);

/* Core util functions.  */
int tman_mkfs(void);
const char *tman_strerror(void);

/* Date structure.  */
tman_unit_t *tman_unit_add(tman_unit_t * head, char *key, char *val);
void *tman_unit_free(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);

/* Input irguments functions.  */
int tman_check_arg_id(tman_arg_t * args);
int tman_check_arg_id_exist(tman_arg_t * args);
int tman_check_arg_brd(tman_arg_t * args);
int tman_check_arg_prj(tman_arg_t * args);

/* Task ID functions.  */
int tman_task_add(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_task_col(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_task_del(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_task_list(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_task_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst);
int tman_task_prev(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_task_set(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_task_show(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_task_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);

/* Board functions.  */
int tman_prj_add(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_prj_del(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_prj_list(tman_ctx_t * ctx, tman_opt_t * options);
int tman_prj_prev(tman_ctx_t * ctx, tman_opt_t * options);
int tman_prj_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst);
int tman_prj_set(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_prj_show(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_prj_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);

/* Project functions.  */
int tman_prj_add(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_prj_del(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_prj_list(tman_ctx_t * ctx, tman_opt_t * options);
int tman_prj_prev(tman_ctx_t * ctx, tman_opt_t * options);
int tman_prj_rename(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst);
int tman_prj_set(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_prj_show(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);
int tman_prj_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * options);

#endif
