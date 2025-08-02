#ifndef LIBTMAN_H
#define LIBTMAN_H

#include "common.h"
#include "errmod.h"

typedef struct tman_arg tman_arg_t;
typedef struct tman_base tman_base_t;
typedef struct tman_unit tman_unit_t;
typedef struct tman_list tman_list_t;
typedef struct tman_context tman_ctx_t;

struct tman_arg {
    char *project;
    char *board;
    char *taskid;
};

struct tman_base {
    char *task;                 /* Directory where tasks are stored */
};

struct tman_unit {
    char *key;
    char *val;
    struct tman_unit *next;
};

struct tman_list {
    char *name;
    int status;                 /* Status code of the object */
    struct tman_list *next;
};

struct tman_context {
    struct tman_unit *units;
    struct tman_unit *column;
    struct tman_list *list;
};

/* Core functions.  */
int tman_make_db(void);
int tman_check_db(void);
char *tman_strerror(int errnum);
int tman_init(tman_base_t * base);
void tman_deinit(tman_ctx_t * ctx);

/* Data structure for unit values.  */
tman_unit_t *tman_unit_add(tman_unit_t * head, char *key, char *val);
tman_unit_t *tman_unit_parse(struct tman_unit *head, const char *str);
char *tman_unit_key(tman_unit_t * head, char *key);
void *tman_unit_free(tman_unit_t * units);
int tman_unit_set(struct tman_unit *head, char *key, char *val);
int tman_unit_save(const char *filename, tman_unit_t * units);

/* Data structure for list of objects.  */
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
int tman_board_add(tman_ctx_t * ctx, tman_arg_t * args);
int tman_board_del(tman_ctx_t * ctx, tman_arg_t * args);
int tman_board_get(tman_ctx_t * ctx, tman_arg_t * args);
int tman_board_list(tman_ctx_t * ctx, tman_arg_t * args);
int tman_board_move(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst);
int tman_board_set(tman_ctx_t * ctx, tman_arg_t * args);
int tman_board_column_set(tman_ctx_t * ctx, tman_arg_t * args);
int tman_board_column_get(tman_ctx_t * ctx, tman_arg_t * args);

/* Project functions.  */
int tman_project_add(tman_ctx_t * ctx, tman_arg_t * args);
int tman_project_del(tman_ctx_t * ctx, tman_arg_t * args);
int tman_project_get(tman_ctx_t * ctx, tman_arg_t * args);
int tman_project_list(tman_ctx_t * ctx);
int tman_project_rename(tman_ctx_t * ctx, tman_arg_t * src, tman_arg_t * dst);
int tman_project_set(tman_ctx_t * ctx, tman_arg_t * args);
int tman_project_column_set(tman_ctx_t * ctx, tman_arg_t * args);
int tman_project_column_get(tman_ctx_t * ctx, tman_arg_t * args);

#endif
