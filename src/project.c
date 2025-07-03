/*
 * Module for 'special' project names - current and previous.
 * Save these project names to be able to switch between them.
 * They also can be used as defaults.
*/

#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "project.h"
#include "common.h"
#include "column.h"
#include "errmod.h"
#include "libtman.h"
#include "osdep.h"
#include "path.h"

#define CPRJ        0           /* index of current prj */
#define PPRJ        1           /* index of previous prj */
#define NPRJ        2           /* number of 'special' project */
#define PRJFMT      "%s\n"      /* project file format */
#define NPRJITEM    1           /* number of project items per line */

static char *prjfile;
static char prjs[NPRJ][PRJSIZ + 1];

static char project_curr[IDSIZ + 1], project_prev[IDSIZ + 1];

static int project_is_set(char *prj)
{
    if (prj == NULL || prj[0] == '\0')
        return FALSE;
    return TRUE;
}

static int save(void)
{
    int i;
    FILE *fp;

    if ((fp = fopen(prjfile, "w")) == NULL)
        return emod_set(LIBTMAN_PRJ_SAVE);

    for (i = 0; i < NPRJ && prjs[i][0] != '\0'; ++i)
        fprintf(fp, PRJFMT, prjs[i]);
    return fclose(fp);
}

// BUG: it does not return TRUE/FALSE, but number instead.
/* TODO: change function name, cuz it's confusing.  */
int project_is_valid_name(char *prj)
{
    if (!isalnum(*prj++))
        return 0;
    for (; *prj; ++prj)
        if (!(isalnum(*prj) || *prj == '_' || *prj == '-'))
            return 0;
    return isalnum(*--prj) != 0;
}

int project_exist(char *prj)
{
    return ISDIR(genpath_prj(prj));
}

char *project_getcurr(char *base, const tman_arg_t * args)
{
    return 0;
}

char *project_getprev(char *base, const tman_arg_t * args)
{
    return 0;
}

int project_addcurr(char *prj)
{
    return 0;
}

int project_delcurr(void)
{
    return 0;
}

int project_delprev(void)
{
    return 0;
}

int project_swap(void)
{
    return 0;
}

int project_is_curr(char *prj)
{
    return 0;
}

int project_is_prev(char *prj)
{
    return 0;
}

/* TODO: change function name, cuz it's confusing.  */
int project_is_valid_length(char *prj)
{
    return strlen(prj) <= PRJSIZ;
}

// roach

// TODO: maybe it's better to add new objects to column BLOG by default?
int project_add(char *base, const tman_arg_t * args, char *colname)
{
    return 0;
}

static int swap(const tman_arg_t * a, const tman_arg_t * b, toggle_t * toggle)
{
    return 0;
}

static int movecurr(char *base, const tman_arg_t * args, toggle_t * toggle,
                    char *colname)
{
    return 0;
}

int project_move(char *base, const tman_arg_t * args, char *colname)
{
    return 0;
}
