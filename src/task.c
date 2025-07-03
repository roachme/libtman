/*  Module to move task from column to column.
    It also supports toggles, i.e. current and previous task IDs.  */

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include "task.h"
#include "column.h"
#include "common.h"
#include "common.h"
#include "osdep.h"
#include "libtman.h"
#include "path.h"

/*
Default columns:
    curr
    prev
    blog

User defined columns (go to cli part):
    TBD
*/

/* For external use. Used only by getters so no harm to module.  */
static char taskcurr[IDSIZ + 1], taskprev[IDSIZ + 1];

/*
 * Check that task ID is valid.
 * @param id task ID
 * @return on success - 1, otherwise 0
*/
int task_is_valid_name(char *id)
{
    if (!isalnum(*id++))
        return 0;
    for (; *id; ++id)
        if (!(isalnum(*id) || *id == '_' || *id == '-'))
            return 0;
    return isalnum(*--id) != 0;
}

/*
 * Return true if ID length's less or equal LIMIT, otherwise false.
 */
int task_is_valid_length(char *id)
{
    return strlen(id) <= IDSIZ;
}

/*
 * Check that task ID exists in project.
 * @param prj project name
 * @param id task ID
*/
int task_exist(char *base, const tman_arg_t * args)
{
    return ISDIR(path_task_dir(base, args));
}

/*
 * Notes:
 * 1. col_del() should delete a file or what? Move it to column blog or done??
*/

static int addcurr(char *base, const tman_arg_t * args, toggle_t * toggle)
{
    return 0;
}

static int delcurr(char *base, const tman_arg_t * args, toggle_t * toggle,
                   char *colname)
{
    return 0;
}

static int delprev(char *base, const tman_arg_t * args, toggle_t * toggle,
                   char *colname)
{
    return 0;
}

static int addprev(char *base, const tman_arg_t * args, toggle_t * toggle)
{
    return 0;
}

static int swap(const tman_arg_t * a, const tman_arg_t * b, toggle_t * toggle)
{
    return 0;
}

/* Move current task to another column.  */
static int movecurr(char *base, const tman_arg_t * args, toggle_t * toggle,
                    char *colname)
{
    return 0;
}

/* Move previous task to another column.  */
static int moveprev(char *base, const tman_arg_t * args, toggle_t * toggle,
                    char *colname)
{
    return 0;
}

/* Move random task to another column. */
static int movetask(char *base, const tman_arg_t * args, toggle_t * toggle,
                    char *colname)
{
    return 0;
}

/*
 * Get current task ID in project.
 * @param prj project name
 * @return current task ID
*/
char *task_getcurr(char *base, const tman_arg_t * args)
{
    return 0;
}

/*
 * Get previous task ID in project.
 * @param prj project name
 * @return previous task ID
*/
char *task_getprev(char *base, const tman_arg_t * args)
{
    return 0;
}

/*
 * Add a task to column.
 * @param base task directory
 * @param args input arguments
 * @param colname column name
*/
int task_add(char *base, const tman_arg_t * args, char *colname)
{
    return 0;
}

/**
 * Delete task from column.
 * @param prj project name
 * @param id task ID
 * @return on success: 0
 * @return on failure: 1
*/
int task_del(char *base, const tman_arg_t * args)
{
    return 0;
}

int task_iscurr(char *base, const tman_arg_t * args)
{
    return 0;
}

int task_isprev(char *base, const tman_arg_t * args)
{
    return 0;
}

/*
 * Move task to another column.
 * @param prj project name
 * @param id task ID
 * @param col column name
*/
int task_move(char *base, const tman_arg_t * args, char *colname)
{
    return 0;
}

/**
 * Swap current and previous task IDs in project.
 * @param prj project
*/
int task_swap(char *base, const tman_arg_t * a, const tman_arg_t * b)
{
    return 0;
}
