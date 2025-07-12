#include <stdio.h>
#include <string.h>
#include "errmod.h"
#include "common.h"

static int errcode = LIBTMAN_OK;

const char *errcodes[__LIBTMAN_STATUS_LAST] = {
    [LIBTMAN_OK] = "OK",

    [LIBTMAN_ECORE] = "error happened in core",
    [LIBTMAN_ESYSVAR] = "could not fill system variables",
    [LIBTMAN_EINIT] = "not inited",
    [LIBTMAN_EFINIT] = "could not init",
    [LIBTMAN_EMKDIR] = "could not make directory",
    [LIBTMAN_ECONF] = "error to parse config",
    [LIBTMAN_EPGN] = "failed to execute plugin command",
    [LIBTMAN_EOPENPRJDIR] = "could not open project directory",
    [LIBTMAN_EINIT_PRJMOD] = "could not init module prj",
    [LIBTMAN_SYS_FS] = "could not create directory",
    [LIBTMAN_MALLOC] = "could not malloc memory",

    [LIBTMAN_PRJ_LOAD] = "could not load project names",
    [LIBTMAN_PRJ_SAVE] = "could not save project names",
    [LIBTMAN_PRJ_SWAP] = "could not swap projects",
    [LIBTMAN_PRJ_TOOLONG] = "project name is too long, limit is " xstr(PRJSIZ),
    [LIBTMAN_PRJ_ILLEG] = "illegal project name",
    [LIBTMAN_PRJ_NOCURR] = "no current project",
    [LIBTMAN_PRJ_NOPREV] = "no previous project",
    [LIBTMAN_PRJ_NOSUCH] = "no such project",
    [LIBTMAN_PRJ_EXISTS] = "project already exists",
    [LIBTMAN_PRJ_SWITCH] = "could not switch to project",
    [LIBTMAN_PRJ_DEL_CURR] = "could not unmark current project",
    [LIBTMAN_PRJ_DEL_PREV] = "could not unmark previous project",
    [LIBTMAN_PRJ_MISSING] = "project name missing",

    [LIBTMAN_BRD_LOAD] = "could not load board names",
    [LIBTMAN_BRD_SAVE] = "could not save board names",
    [LIBTMAN_BRD_SWAP] = "could not swap boards",
    [LIBTMAN_BRD_TOOLONG] = "board name is too long, limit is " xstr(PRJSIZ),
    [LIBTMAN_BRD_ILLEG] = "illegal board name",
    [LIBTMAN_BRD_NOCURR] = "no current board",
    [LIBTMAN_BRD_NOPREV] = "no previous board",
    [LIBTMAN_BRD_NOSUCH] = "no such board",
    [LIBTMAN_BRD_EXISTS] = "board already exists",
    [LIBTMAN_BRD_SWITCH] = "could not switch to board",
    [LIBTMAN_BRD_DEL_CURR] = "could not unmark current board",
    [LIBTMAN_BRD_DEL_PREV] = "could not unmark previous board",
    [LIBTMAN_BRD_MISSING] = "board name missing",

    [LIBTMAN_ID_ILLEG] = "illegal task ID",
    [LIBTMAN_ID_TOOLONG] = "task ID is too long, limit is " xstr(IDSIZ),
    [LIBTMAN_ID_NOSUCH] = "no such task ID",
    [LIBTMAN_ID_NOCURR] = "no current task ID",
    [LIBTMAN_ID_NOPREV] = "no previous task ID",
    [LIBTMAN_ID_EXISTS] = "task ID already exists",
    [LIBTMAN_ID_SWAP] = "could not swap task IDs",

    [LIBTMAN_COL_ADD] = "could not add task to default column",
    [LIBTMAN_COL_GET] = "could not get column name",
    [LIBTMAN_COL_MOVE] = "could not move task to column",
    [LIBTMAN_COL_DEL] = "could not delete task from column",
    [LIBTMAN_COL_EXISTS] = "column does not exist",

    [LIBTMAN_DIR_ID_DEL] = "could not delete task directory",
    [LIBTMAN_DIR_ID_MAKE] = "could not create task directory",
    [LIBTMAN_DIR_ID_REN] = "could not rename task directory",
    [LIBTMAN_DIR_ID_OPEN] = "could not open task directory",

    [LIBTMAN_DIR_PRJ_DEL] = "could not delete prj directory",
    [LIBTMAN_DIR_PRJ_MAKE] = "could not create prj directory",
    [LIBTMAN_DIR_PRJ_OPEN] = "could not open prj directory",

    [LIBTMAN_UNIT_DEL] = "could not delete task unit",
    [LIBTMAN_UNIT_GET] = "could not get unit values",
    [LIBTMAN_UNIT_ILLEG] = "illegal unit value",
    [LIBTMAN_UNIT_KEY] = "unit key does not exist",
    [LIBTMAN_UNIT_LOAD] = "could not load units",
    [LIBTMAN_UNIT_MAKE] = "could not create task unit",
    [LIBTMAN_UNIT_SET] = "could not set unit values",

    [LIBTMAN_CMD_BIN] = "no such command",
    [LIBTMAN_CMD_PGN] = "no such plugin",
    [LIBTMAN_CMD_GEN] = "no such command or plugin",

    /* HOTFIX: delete it once error code table is filled.  */
    /* Not yet defined error messages.  */
    [LIBTMAN_NODEF_ERR] = "not yet defined error messages",
};

int emod_set(int err)
{
    errcode = err;
    if (errcode < 0 || errcode >= __LIBTMAN_STATUS_LAST)
        errcode = -1;
    return errcode;
}

/*
int emod_get(void)
{
    return errcode;
}
*/

const char *emod_strerror(void)
{
    static char errmsg[ERRMSGSIZ + 1];

    if (errcode < 0 || errcode >= __LIBTMAN_STATUS_LAST)
        return strncpy(errmsg, "internal unknown error", ERRMSGSIZ);
    return strncpy(errmsg, errcodes[errcode], ERRMSGSIZ);
}

const char *emod_strerror_get(int status)
{
    static char errmsg[ERRMSGSIZ + 1];

    if (status < 0 || status >= __LIBTMAN_STATUS_LAST)
        return strncpy(errmsg, "internal unknown error", ERRMSGSIZ);
    return strncpy(errmsg, errcodes[status], ERRMSGSIZ);
}
