#ifndef LIBTMAN_H
#define LIBTMAN_H

#include "common.h"
#include "errmod.h"

typedef struct tman_arg tman_arg_t;
typedef struct tman_unit tman_unit_t;
typedef struct tman_list tman_list_t;
typedef struct tman_context tman_ctx_t;

struct tman_arg {
    char *project;
    char *board;
    char *taskid;
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
int tman_make_db(const char *taskdir);
int tman_check_db(const char *taskdir);
char *tman_strerror(int errnum);

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
int tman_project_exist(const char *taskdir, tman_arg_t * args);
int tman_project_valid(const char *taskdir, tman_arg_t * args);
int tman_board_exist(const char *taskdir, tman_arg_t * args);
int tman_board_valid(const char *taskdir, tman_arg_t * args);
int tman_task_exist(const char *taskdir, tman_arg_t * args);
int tman_task_valid(const char *taskdir, tman_arg_t * args);

/* Task functions.  */
int tman_task_add(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_task_del(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_task_get(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_task_list(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_task_move(const char *taskdir, tman_arg_t * src, tman_arg_t * dst,
                   tman_ctx_t * ctx);
int tman_task_set(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_task_column_get(const char *taskdir, tman_arg_t * args,
                         tman_ctx_t * ctx);
int tman_task_column_set(const char *taskdir, tman_arg_t * args,
                         tman_ctx_t * ctx);

/* Board functions.  */
int tman_board_add(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_board_del(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_board_get(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_board_list(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_board_move(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_board_set(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_board_column_set(const char *taskdir, tman_arg_t * args,
                          tman_ctx_t * ctx);
int tman_board_column_get(const char *taskdir, tman_arg_t * args,
                          tman_ctx_t * ctx);

/* Project functions.  */
int tman_project_add(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_project_del(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_project_get(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_project_list(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_project_rename(const char *taskdir, tman_arg_t * src, tman_arg_t * dst,
                        tman_ctx_t * ctx);
int tman_project_set(const char *taskdir, tman_arg_t * args, tman_ctx_t * ctx);
int tman_project_column_set(const char *taskdir, tman_arg_t * args,
                            tman_ctx_t * ctx);
int tman_project_column_get(const char *taskdir, tman_arg_t * args,
                            tman_ctx_t * ctx);

#endif
