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

static char colname[10];

/*
static void toggle_show(toggle_t * toggle)
{
    printf("curr: '%s' - '%s' - %d\n", toggle->curr.id, toggle->curr.column,
           toggle->curr.update);
    printf("prev: '%s' - '%s' - %d\n", toggle->prev.id, toggle->prev.column,
           toggle->prev.update);
    printf("tmp : '%s' - '%s' - %d\n", toggle->tmp.id, toggle->tmp.column,
           toggle->tmp.update);
}
*/

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
    char *pathname;

    if ((pathname = path_column(dirname, obj)) == NULL)
        return NULL;
    if ((fp = fopen(pathname, "r")) == NULL) {
        return NULL;
    }

    if (fscanf(fp, "column : %s\n", colname) != 1) {
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    return colname;
}

static int _column_set(const char *dirname, char *obj, char *colname)
{
    FILE *fp;
    char *pathname;

    if ((pathname = path_column(dirname, obj)) == NULL)
        return 1;
    //fprintf(stderr, "%s: %s\n", __func__, pathname);
    if ((fp = fopen(pathname, "w")) == NULL) {
        return 1;
    }
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

        if (strcmp(colname, COLUMN_CURR) == 0) {
            memset(toggle->curr.id, 0, TOGGLESIZ);
            strcpy(toggle->curr.id, ent->d_name);
            toggle->curr.id[TOGGLEID_SIZ] = '\0';
            strcpy(toggle->curr.column, COLUMN_CURR);
            //fprintf(stderr, "-> column_load: '%s'\n", toggle->curr.id);
        } else if (strcmp(colname, COLUMN_PREV) == 0) {
            memset(toggle->prev.id, 0, TOGGLESIZ);
            strcpy(toggle->prev.id, ent->d_name);
            toggle->prev.id[TOGGLEID_SIZ] = '\0';
            strcpy(toggle->prev.column, COLUMN_PREV);
        }

    }
    return closedir(dir);
}

static int _column_save(const char *dirname, toggle_t * toggle)
{
    //fprintf(stderr, "%s: '%s' - '%s' start\n", __func__, toggle->curr.id, toggle->prev.id);

    if (toggle->curr.update == 1 && toggle->curr.id[0] != '\0')
        if (_column_set(dirname, toggle->curr.id, toggle->curr.column))
            return 1;
    if (toggle->prev.update == 1 && toggle->prev.id[0] != '\0')
        if (_column_set(dirname, toggle->prev.id, toggle->prev.column))
            return 1;
    if (toggle->tmp.update == 1 && toggle->tmp.id[0] != '\0')
        if (_column_set(dirname, toggle->tmp.id, toggle->tmp.column))
            return 1;
    return 0;
}

// NOTE: toggle is already load with current and previous objects
static int _column_move(toggle_t * toggle, char *obj, char *colname)
{
    if (strcmp(toggle->curr.id, obj) == 0) {
        if (strcmp(colname, COLUMN_CURR) == 0) {
            return 0;           // move current task to current column -> do nothing
        } else if (strcmp(colname, COLUMN_PREV) == 0) {
            /* Move current task to previous column -> do swapping.  */
            // Swapping
            toggle->curr.update = 1;
            strcpy(toggle->curr.column, COLUMN_PREV);
            toggle->prev.update = 1;
            strcpy(toggle->prev.column, COLUMN_CURR);
        } else {
            /* Move current task to another column -> mark previous as new current.  */
            toggle->curr.update = 1;
            strcpy(toggle->curr.column, colname);
            toggle->prev.update = 1;
            strcpy(toggle->prev.column, COLUMN_CURR);
        }
    } else if (strcmp(toggle->prev.id, obj) == 0) {
        if (strcmp(colname, COLUMN_PREV) == 0) {
            return 0;           // move previous task to previous column -> do nothing
        } else if (strcmp(colname, COLUMN_CURR) == 0) {
            /* Move previous task to current column -> do swapping */
            // Swapping
            toggle->prev.update = 1;
            strcpy(toggle->prev.column, COLUMN_CURR);
            toggle->curr.update = 1;
            strcpy(toggle->curr.column, COLUMN_PREV);
        } else {
            /* Move previous task to another task -> unmark previous task and move it.  */
            toggle->prev.update = 1;
            strcpy(toggle->prev.column, colname);
        }
    } else {
        if (strcmp(colname, COLUMN_PREV) == 0) {
            return 0;           /* Special case: cannot set previous directly.  */
        } else if (strcmp(colname, COLUMN_CURR) == 0) {
            /* Move non-toggle to curr column.  */
            toggle->curr.update = 1;
            strcpy(toggle->curr.column, COLUMN_PREV);
            toggle->prev.update = 1;
            strcpy(toggle->prev.column, COLUMN_BLOG);

            toggle->tmp.update = 1;
            strcpy(toggle->tmp.id, obj);
            strcpy(toggle->tmp.column, COLUMN_CURR);
        } else {
            /* Move non-toggle to non-toggle column */
            toggle->tmp.update = 1;
            strcpy(toggle->tmp.id, obj);
            strcpy(toggle->tmp.column, colname);
        }
    }
    return 0;
}

static int _column_del(toggle_t * toggle, char *obj)
{
    return _column_move(toggle, obj, COLUMN_BLOG);
}

static int _column_swap(toggle_t * toggle)
{
    return _column_move(toggle, toggle->curr.id, COLUMN_PREV);
}

// API level 1: END

// API level 2: BEGIN
static char *column_curr(char *dirname, char *buf)
{
    toggle_t toggle;

    if (_column_load(dirname, &toggle))
        return NULL;
    if (toggle.curr.id[0] == '\0')
        return NULL;
    return strcpy(buf, toggle.curr.id);
}

static char *column_prev(char *dirname, char *buf)
{
    toggle_t toggle;

    if (_column_load(dirname, &toggle))
        return NULL;
    else if (toggle.prev.id[0] == '\0')
        return NULL;
    return strcpy(buf, toggle.prev.id);
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

    if (_column_load(dirname, &toggle)) {
        //fprintf(stderr, "--- ERORRO load toggles\n");
        return 1;
    }
    //fprintf(stderr, "--> %s:%s '%s' - '%s' start me\n", __func__, dirname, toggle.curr.id, toggle.prev.id);
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

char *column_project_curr(char *base, tman_arg_t * args)
{
    static char curr[PRJSIZ + 1] = { 0 };
    char *dirname = base;

    if (column_curr(dirname, curr) != NULL) {
        curr[PRJSIZ] = '\0';
        return curr;
    }
    return NULL;
}

char *column_project_prev(char *base, tman_arg_t * args)
{
    static char prev[PRJSIZ + 1] = { 0 };
    char *dirname = base;

    if (column_prev(dirname, prev) != NULL) {
        prev[PRJSIZ] = '\0';
        return prev;
    }
    return NULL;
}

int column_project_del(char *base, tman_arg_t * args)
{
    char *dirname = base;
    return column_del(dirname, args->prj);
}

int column_project_move(char *base, tman_arg_t * args, char *colname)
{
    char *dirname = base;
    return column_move(dirname, args->prj, colname);
}

int column_project_swap(char *base, tman_arg_t * args)
{
    char *dirname = base;
    return column_swap(dirname);
}

/* Project columns: END.  */

/* Board columns: BEGIN.  */
char *column_board_curr(char *base, tman_arg_t * args)
{
    static char curr[BRDSIZ + 1] = { 0 };
    char *dirname = path_prj_dir(base, args);

    if (column_curr(dirname, curr) != NULL) {
        curr[BRDSIZ] = '\0';
        return curr;
    }
    return NULL;
}

char *column_board_prev(char *base, tman_arg_t * args)
{
    static char prev[BRDSIZ + 1] = { 0 };
    char *dirname = path_prj_dir(base, args);

    if (column_curr(dirname, prev) != NULL) {
        prev[BRDSIZ] = '\0';
        return prev;
    }
    return NULL;
}

int column_board_del(char *base, tman_arg_t * args)
{
    char *dirname = path_prj_dir(base, args);
    return column_del(dirname, args->brd);

}

int column_board_move(char *base, tman_arg_t * args, char *colname)
{
    char *dirname = path_prj_dir(base, args);
    return column_move(dirname, args->brd, colname);
}

int column_board_swap(char *base, tman_arg_t * args)
{
    char *dirname = path_prj_dir(base, args);
    return column_swap(dirname);
}

/* Board columns: END.  */

/* Task columns: BEGIN.  */
char *column_task_curr(char *base, tman_arg_t * args)
{
    static char curr[IDSIZ + 1] = { 0 };
    char *dirname = path_brd_dir(base, args);

    if (column_curr(dirname, curr) != NULL) {
        curr[IDSIZ] = '\0';
        return curr;
    }
    return NULL;
}

char *column_task_prev(char *base, tman_arg_t * args)
{
    static char prev[IDSIZ + 1] = { 0 };
    char *dirname = path_brd_dir(base, args);

    if (column_prev(dirname, prev) != NULL) {
        prev[IDSIZ] = '\0';
        return prev;
    }
    return NULL;
}

int column_task_del(char *base, tman_arg_t * args)
{
    char *dirname = path_brd_dir(base, args);
    return column_del(dirname, args->task);
}

int column_task_move(char *base, tman_arg_t * args, char *colname)
{
    char *dirname = path_brd_dir(base, args);
    return column_move(dirname, args->task, colname);
}

int column_task_swap(char *base, tman_arg_t * args)
{
    char *dirname = path_brd_dir(base, args);
    return column_swap(dirname);
}

/* Task columns: END */

char *column_task_colname(char *base, tman_arg_t * args)
{
    char *dirname = path_brd_dir(base, args);
    return _column_get(dirname, args->task);
}
