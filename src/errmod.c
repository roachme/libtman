#include <string.h>
#include "errmod.h"

static int errcode = LIBTMAN_OK;

const char *errcodes[__LIBTMAN_STATUS_LAST] = {
    [LIBTMAN_OK] = "OK",

    [LIBTMAN_DB] = "main database directory not found",
    [LIBTMAN_ESYSVAR] = "could not fill system variables",
    [LIBTMAN_EINIT] = "not inited",
    [LIBTMAN_SYS_FS] = "could not create directory",
    [LIBTMAN_MALLOC] = "could not malloc memory",

    [LIBTMAN_PRJ_LOAD] = "could not load project names",
    [LIBTMAN_PRJ_SAVE] = "could not save project names",
    [LIBTMAN_PRJ_ILLEG] = "illegal project name",
    [LIBTMAN_PRJ_NOSUCH] = "no such project",
    [LIBTMAN_PRJ_EXISTS] = "project already exists",
    [LIBTMAN_PRJ_BOARD_CHECK] = "board support is disabled in project",
    [LIBTMAN_PRJ_BOARD_ADD] = "could not add board support in project",

    [LIBTMAN_BRD_LOAD] = "could not load board names",
    [LIBTMAN_BRD_SAVE] = "could not save board names",
    [LIBTMAN_BRD_ILLEG] = "illegal board name",
    [LIBTMAN_BRD_NOSUCH] = "no such board",
    [LIBTMAN_BRD_EXISTS] = "board already exists",

    [LIBTMAN_ID_ILLEG] = "illegal task ID",
    [LIBTMAN_ID_NOSUCH] = "no such task ID",
    [LIBTMAN_ID_EXISTS] = "task ID already exists",

    [LIBTMAN_COL_ADD] = "could not add task to default column",
    [LIBTMAN_COL_GET] = "could not get column name",
    [LIBTMAN_COL_MOVE] = "could not move task to column",
    [LIBTMAN_COL_DEL] = "could not delete task from column",
    [LIBTMAN_COL_EXISTS] = "column does not exist",

    [LIBTMAN_DIR_PRJ_DEL] = "could not delete project directory",
    [LIBTMAN_DIR_PRJ_MAKE] = "could not create project directory",
    [LIBTMAN_DIR_PRJ_OPEN] = "could not open project directory",
    [LIBTMAN_DIR_PRJ_RENAME] = "could not rename project",

    [LIBTMAN_DIR_BRD_DEL] = "could not delete prj directory",
    [LIBTMAN_DIR_BRD_MAKE] = "could not create prj directory",
    [LIBTMAN_DIR_BRD_MOVE] = "could not move board directory",
    [LIBTMAN_DIR_BRD_OPEN] = "could not open board directory",

    [LIBTMAN_DIR_ID_DEL] = "could not delete task directory",
    [LIBTMAN_DIR_ID_MAKE] = "could not create task directory",
    [LIBTMAN_DIR_ID_MOVE] = "could not rename task directory",
    [LIBTMAN_DIR_ID_OPEN] = "could not open task directory",

    [LIBTMAN_UNIT_DEL] = "could not delete task unit",
    [LIBTMAN_UNIT_GET] = "could not get unit values",
    [LIBTMAN_UNIT_ILLEG] = "illegal unit value",
    [LIBTMAN_UNIT_KEY] = "unit key does not exist",
    [LIBTMAN_UNIT_LOAD] = "could not load units",
    [LIBTMAN_UNIT_SAVE] = "could not save unit values",

    [LIBTMAN_COLUMN_DEL] = "could not delete task column",
    [LIBTMAN_COLUMN_GET] = "could not get column values",
    [LIBTMAN_COLUMN_ILLEG] = "illegal column value",
    [LIBTMAN_COLUMN_KEY] = "column key does not exist",
    [LIBTMAN_COLUMN_LOAD] = "could not load columns",
    [LIBTMAN_COLUMN_SAVE] = "could not save column values",

    /* HOTFIX: delete it once error code table is filled.  */
    /* Not yet defined error messages.  */
    [LIBTMAN_NODEF_ERR] = "not yet defined error messages",
};

int emod_set(int err)
{
    errcode = err;
    if (errcode < 0 || errcode > __LIBTMAN_STATUS_LAST)
        errcode = -1;
    return errcode;
}

char *emod_strerror(int errnum)
{
    static char errmsg[ERRMSGSIZ + 1];

    if (errnum < 0 || errnum > __LIBTMAN_STATUS_LAST)
        return strncpy(errmsg, "internal unknown error", ERRMSGSIZ);
    return strncpy(errmsg, errcodes[errnum], ERRMSGSIZ);
}
