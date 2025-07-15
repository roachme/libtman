#ifndef LIBTMAN_H
#define LIBTMAN_H

#include "errmod.h"
#include "tree.h"

struct mylist {
    char *id;
    int status;                 // status code for invalid objects
    int colprio;                // maybe there's no need, cuz we are gonna use linked list
    char *colname;              // maybe?...  even for custom column, just read value from file
    int shown;
    struct mylist *next;
};

struct objlist {
    struct mylist *curr;
    struct mylist *prev;
    struct mylist *blog;
    struct mylist *done;
    struct mylist *custom;
    struct mylist *inval;
};

typedef struct tman_arg tman_arg_t;
typedef struct tman_base tman_base_t;
typedef struct tman_unit tman_unit_t;
typedef struct tman_option tman_opt_t;
typedef struct tman_context tman_ctx_t;

struct tman_arg {
    char *prj;
    char *brd;
    char *task;
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
    int brd_switch;
    int prj_switch;
    int task_switch;
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
    struct tman_unit *unitbrd;
    struct tree *prjs;
    struct tree *brds;
    struct objlist *ids;

    /* TODO: use 'struct objlist' and get rid of struct's below.  */
    struct tree *invprjs;
    struct tree *invbrds;
};

/* Core functions.  */
int tman_setup(int setuplvl);
tman_ctx_t *tman_init(tman_base_t * structure);
tman_ctx_t *tman_deinit(tman_ctx_t * ctx);

/* Core util functions.  */
int tman_mkfs(void);
const char *tman_strerror(void);
const char *tman_strerror_get(int status);

/* Data structure.  */
tman_unit_t *tman_unit_add(tman_unit_t * head, char *key, char *val);
char *tman_unit_get_key(tman_unit_t * head, char *key);
void tman_unit_free(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);

/* Input argument functions.  */
int tman_check_arg_task(tman_arg_t * args);
int tman_check_arg_task_exist(tman_arg_t * args);
int tman_check_arg_brd(tman_arg_t * args);
int tman_check_arg_prj(tman_arg_t * args);

/* Task functions.  */
int tman_task_add(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_task_del(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_task_get(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_task_flow(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_task_list(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_task_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst);
int tman_task_prev(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_task_set(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_task_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);

/* Board functions.  */
int tman_brd_add(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_brd_del(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_brd_get(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_brd_list(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_brd_prev(tman_ctx_t * ctx, tman_opt_t * opts);
int tman_brd_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst);
int tman_brd_set(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_brd_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);

/* Project functions.  */
int tman_prj_add(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_prj_del(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_prj_get(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_prj_list(tman_ctx_t * ctx, tman_opt_t * opts);
int tman_prj_prev(tman_ctx_t * ctx, tman_opt_t * opts);
int tman_prj_rename(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst);
int tman_prj_set(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);
int tman_prj_sync(tman_ctx_t * ctx, tman_arg_t * args, tman_opt_t * opts);

#endif
