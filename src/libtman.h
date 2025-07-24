#ifndef LIBTMAN_H
#define LIBTMAN_H

#include "tree.h"
#include "errmod.h"

typedef struct tman_arg tman_arg_t;
typedef struct tman_base tman_base_t;
typedef struct tman_unit tman_unit_t;
typedef struct tman_list tman_list_t;
typedef struct tman_context tman_ctx_t;
typedef struct tman_column tman_column_t;

struct tman_arg {
    char *prj;
    char *brd;
    char *task;
};

// NOTE: maybe use linked list as 'struct tman_unit' ?..
// This way cli part can add new column items, with no changes in lib part
struct tman_column {
    int prio;
    char mark;
    char *name;
};

struct tman_base {
    char *task;                 /* directory where tasks are stored */
};

struct tman_unit {
    char *key;
    char *val;
    struct tman_unit *next;
};

struct tman_list {
    char *name;
    unsigned char status;       /* Status code for invalid objects */
    struct tman_list *next;
};

struct tman_context {
    struct tman_unit *units;
    struct tman_unit *column;
    struct tman_list *list;
};

/* Core functions.  */
int tman_mkfs(void);
int tman_check_db(void);
const char *tman_strerror(void);
tman_ctx_t *tman_deinit(tman_ctx_t * ctx);
const char *tman_strerror_get(int status);
tman_ctx_t *tman_init(tman_base_t * structure);

/* Data structure.  */
tman_unit_t *tman_unit_add(tman_unit_t * head, char *key, char *val);
tman_unit_t *tman_unit_parse(struct tman_unit *head, const char *str);
char *tman_unit_key(tman_unit_t * head, char *key);
void *tman_unit_free(tman_unit_t * units);
int tman_unit_set(struct tman_unit *head, char *key, char *val);
int tman_unit_save(const char *filename, tman_unit_t * units);

void *tman_list_free(tman_list_t * list);

/* Input argument functions.  */
int tman_check_arg_task(tman_arg_t * args);
int tman_check_arg_brd(tman_arg_t * args);
int tman_check_arg_prj(tman_arg_t * args);
int tman_check_arg_brd_exist(tman_arg_t * args);
int tman_check_arg_task_exist(tman_arg_t * args);

/* Task functions.  */
int tman_task_add(tman_ctx_t * ctx, tman_arg_t * args);
int tman_task_del(tman_ctx_t * ctx, tman_arg_t * args);
int tman_task_get(tman_ctx_t * ctx, tman_arg_t * args);
int tman_task_list(tman_ctx_t * ctx, tman_arg_t * args);
int tman_task_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst);
int tman_task_set(tman_ctx_t * ctx, tman_arg_t * args);
int tman_task_column_get(tman_ctx_t * ctx, tman_arg_t * args);
int tman_task_column_set(tman_ctx_t * ctx, tman_arg_t * args);

/* Board functions.  */
int tman_brd_add(tman_ctx_t * ctx, tman_arg_t * args);
int tman_brd_del(tman_ctx_t * ctx, tman_arg_t * args);
int tman_brd_get(tman_ctx_t * ctx, tman_arg_t * args);
int tman_brd_list(tman_ctx_t * ctx, tman_arg_t * args);
int tman_brd_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst);
int tman_brd_set(tman_ctx_t * ctx, tman_arg_t * args);
int tman_brd_sync(tman_ctx_t * ctx, tman_arg_t * args);

/* Project functions.  */
int tman_prj_add(tman_ctx_t * ctx, tman_arg_t * args);
int tman_prj_del(tman_ctx_t * ctx, tman_arg_t * args);
int tman_prj_get(tman_ctx_t * ctx, tman_arg_t * args);
int tman_prj_list(tman_ctx_t * ctx);
int tman_prj_rename(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst);
int tman_prj_set(tman_ctx_t * ctx, tman_arg_t * args);
int tman_prj_sync(tman_ctx_t * ctx, tman_arg_t * args);

#endif
