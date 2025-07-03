/*
    Implement module column for tman project.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "path.h"
#include "column.h"
#include "common.h"

static char objcurr[10], objprev[10];
static char colname[10];

static void toggle_show(toggle_t * toggle)
{
    printf("curr: '%s' - '%s' - %d\n", toggle->curr.id, toggle->curr.column,
           toggle->curr.update);
    printf("prev: '%s' - '%s' - %d\n", toggle->prev.id, toggle->prev.column,
           toggle->prev.update);
    printf("tmp : '%s' - '%s' - %d\n", toggle->tmp.id, toggle->tmp.column,
           toggle->tmp.update);
}

static void toggle_init(toggle_t * toggle)
{
    toggle->curr.update = toggle->prev.update = toggle->tmp.update = 0;
    toggle->curr.id[0] = toggle->prev.id[0] = toggle->tmp.id[0] = '\0';
    toggle->curr.column[0] = toggle->prev.column[0] = toggle->tmp.column[0] =
        '\0';
}

// API level 1: BEGIN
static char *_column_get(const char *dirname, char *obj)
{
    FILE *fp;
    char buf[BUFSIZ + 1];
    char *pathname;

    if ((pathname = path_column(dirname, obj)) == NULL)
        return NULL;
    if ((fp = fopen(pathname, "r")) == NULL)
        return NULL;

    if (fscanf(fp, "column : %s\n", colname) != 1) {
        free(pathname);
        fclose(fp);
        return NULL;
    }
    free(pathname);
    fclose(fp);
    return colname;
}

static int _column_set(const char *dirname, char *obj, char *colname)
{
    FILE *fp;
    char buf[BUFSIZ + 1];
    char *pathname;

    fprintf(stderr, "_column_set: start, path: '%s' '%s'\n", dirname, obj);
    if ((pathname = path_column(dirname, obj)) == NULL)
        return 1;
    if ((fp = fopen(pathname, "w")) == NULL)
        return 1;
    fprintf(fp, "column : %s\n", colname);
    return fclose(fp);
}

static int _column_load(const char *dirname, toggle_t * toggle)
{
    DIR *dir;
    struct dirent *ent;
    char *_colname;

    if ((dir = opendir(dirname)) == NULL)
        return 1;

    toggle_init(toggle);
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_name[0] == '.' || ent->d_type != DT_DIR)
            continue;
        else if ((_colname = _column_get(dirname, ent->d_name)) == NULL)
            continue;

        if (strcmp(colname, COLCURR) == 0) {
            strcpy(toggle->curr.id, ent->d_name);
            strcpy(toggle->curr.column, COLCURR);
        } else if (strcmp(colname, COLPREV) == 0) {
            strcpy(toggle->prev.id, ent->d_name);
            strcpy(toggle->prev.column, COLPREV);
        }
    }
    return 0;
}

// static int _column_set(const char *dirname, char *obj, char *colname)
static int _column_save(const char *dirname, toggle_t * toggle)
{
    fprintf(stderr, "_column_save: start\n");
    if (toggle->curr.update == 1 && toggle->curr.id[0] != '\0')
        _column_set(dirname, toggle->curr.id, toggle->curr.column);
    if (toggle->prev.update == 1 && toggle->prev.id[0] != '\0')
        _column_set(dirname, toggle->prev.id, toggle->prev.column);
    if (toggle->tmp.update == 1 && toggle->tmp.id[0] != '\0')
        _column_set(dirname, toggle->tmp.id, toggle->tmp.column);
    return 0;
}

// NOTE: toggle is already load with current and previous objects
static int _column_move(toggle_t * toggle, char *obj, char *colname)
{
    if (strcmp(toggle->curr.id, obj) == 0) {
        if (strcmp(colname, COLCURR) == 0) {
            return 0;           // move current task to current column -> do nothing
        } else if (strcmp(colname, COLPREV) == 0) {
            /* Move current task to previous column -> do swapping.  */
            // Swapping
            toggle->curr.update = 1;
            strcpy(toggle->curr.column, COLPREV);
            toggle->prev.update = 1;
            strcpy(toggle->prev.column, COLCURR);
        } else {
            /* Move current task to another column -> mark previous as new current.  */
            toggle->curr.update = 1;
            strcpy(toggle->curr.column, colname);
            toggle->prev.update = 1;
            strcpy(toggle->prev.column, COLCURR);
        }
    } else if (strcmp(toggle->prev.id, obj) == 0) {
        if (strcmp(colname, COLPREV) == 0) {
            return 0;           // move previous task to previous column -> do nothing
        } else if (strcmp(colname, COLCURR) == 0) {
            /* Move previous task to current column -> do swapping */
            // Swapping
            toggle->prev.update = 1;
            strcpy(toggle->prev.column, COLCURR);
            toggle->curr.update = 1;
            strcpy(toggle->curr.column, COLPREV);
        } else {
            /* Move previous task to another task -> unmark previous task and move it.  */
            toggle->prev.update = 1;
            strcpy(toggle->prev.column, colname);
        }
    } else {
        if (strcmp(colname, COLPREV) == 0) {
            return 0;           /* Special case: cannot set previous directly.  */
        } else if (strcmp(colname, COLCURR) == 0) {
            /* Move non-toggle to curr column.  */
            toggle->curr.update = 1;
            strcpy(toggle->curr.column, COLPREV);
            toggle->prev.update = 1;
            strcpy(toggle->prev.column, COLBLOG);

            toggle->tmp.update = 1;
            strcpy(toggle->tmp.id, obj);
            strcpy(toggle->tmp.column, COLCURR);
        } else {
            /* Move non-toggle to non-toggle column */
            toggle->tmp.update = 1;
            strcpy(toggle->tmp.id, obj);
            strcpy(toggle->tmp.column, colname);
        }
    }
    return 0;
}

static int _column_add(toggle_t * toggle, char *obj, char *colname)
{
    return _column_move(toggle, obj, colname);
}

static int _column_del(toggle_t * toggle, char *obj)
{
    return _column_move(toggle, obj, COLBLOG);
}

static int _column_swap(toggle_t * toggle)
{
    return _column_move(toggle, toggle->curr.id, COLPREV);
}

// API level 1: END

// API level 2: BEGIN
static char *column_curr(char *dirname)
{
    toggle_t toggle;

    if (_column_load(dirname, &toggle))
        return NULL;
    else if (toggle.curr.id[0] == '\0')
        return NULL;
    return strcpy(objcurr, toggle.curr.id);
}

static char *column_prev(char *dirname)
{
    toggle_t toggle;

    if (_column_load(dirname, &toggle))
        return NULL;
    else if (toggle.prev.id[0] == '\0')
        return NULL;
    return strcpy(objprev, toggle.prev.id);
}

static int column_add(char *dirname, char *obj, char *colname)
{
    toggle_t toggle;

    if (_column_load(dirname, &toggle))
        return 1;
    if (_column_add(&toggle, obj, colname))
        return 1;
    return _column_save(dirname, &toggle);
}

static int column_del(char *dirname, char *obj)
{
    toggle_t toggle;

    if (_column_load(dirname, &toggle))
        return 1;
    if (_column_del(&toggle, obj))
        return 1;
    return _column_save(dirname, &toggle);
}

static int column_swap(char *dirname)
{
    toggle_t toggle;

    if (_column_load(dirname, &toggle))
        return 1;
    if (toggle.curr.id[0] == '\0' || toggle.prev.id[0] == '\0')
        return 1;
    if (_column_swap(&toggle))
        return 1;
    return _column_save(dirname, &toggle);
}

//int task_move(char *prj, char *id, char *col)
static int column_move(char *dirname, char *obj, char *colname)
{
    toggle_t toggle;

    if (_column_load(dirname, &toggle))
        return 1;
    if (_column_move(&toggle, obj, colname))
        return 1;
    return _column_save(dirname, &toggle);
}

// API level 2: END

/* Project columns: BEGIN.  */

char *column_project_curr(tman_arg_t * args)
{
    char *dirname = tmanfs.base;
    return column_curr(dirname);
}

char *column_project_prev(tman_arg_t * args)
{
    char *dirname = tmanfs.base;
    return column_prev(dirname);
}

int column_project_add(tman_arg_t * args, char *colname)
{
    char *dirname = tmanfs.base;
    return column_add(dirname, args->prj, colname);
}

int column_project_del(tman_arg_t * args)
{
    char *dirname = tmanfs.base;
    return column_del(dirname, args->prj);
}

int column_project_move(tman_arg_t * args, char *colname)
{
    char *dirname = tmanfs.base;
    return column_move(dirname, args->prj, colname);
}

int column_project_swap(tman_arg_t * args)
{
    char *dirname = tmanfs.base;
    return column_swap(dirname);
}

/* Project columns: END.  */

/* Board columns: BEGIN.  */
char *column_board_curr(tman_arg_t * args)
{
    char *dirname = path_prj_dir(tmanfs.base, args);
    return column_curr(dirname);
}

char *column_board_prev(tman_arg_t * args)
{
    char *dirname = path_prj_dir(tmanfs.base, args);
    return column_prev(dirname);
}

int column_board_add(tman_arg_t * args, char *colname)
{
    char *dirname = path_prj_dir(tmanfs.base, args);
    return column_add(dirname, args->brd, colname);
}

int column_board_del(tman_arg_t * args)
{
    char *dirname = path_prj_dir(tmanfs.base, args);
    return column_del(dirname, args->brd);
}

int column_board_move(tman_arg_t * args, char *colname)
{
    char *dirname = path_prj_dir(tmanfs.base, args);

    fprintf(stderr, "column_board_move: start, dirname: '%s'\n", dirname);
    return column_move(dirname, args->brd, colname);
}

int column_board_swap(tman_arg_t * args)
{
    char *dirname = path_prj_dir(tmanfs.base, args);
    return column_swap(dirname);
}

/* Board columns: END.  */

/* Task columns: BEGIN.  */
int column_task_add(tman_arg_t * args, char *colname)
{
    char *dirname = path_brd_dir(tmanfs.base, args);
    return column_add(dirname, args->id, colname);
}

int column_task_del(tman_arg_t * args)
{
    char *dirname = path_brd_dir(tmanfs.base, args);
    return column_del(dirname, args->id);
}

int column_task_move(tman_arg_t * args, char *colname)
{
    char *dirname = path_brd_dir(tmanfs.base, args);
    return column_move(dirname, args->id, colname);
}

int column_task_swap(tman_arg_t * args)
{
    char *dirname = path_brd_dir(tmanfs.base, args);
    return column_swap(dirname);
}

/* Task columns: END */
