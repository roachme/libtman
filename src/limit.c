#include <string.h>

#include "limit.h"

int limit_project_ok(char *name)
{
    return strlen(name) <= PRJSIZ;
}

int limit_board_ok(char *name)
{
    return strlen(name) <= BRDSIZ;
}

int limit_task_ok(char *name)
{
    return strlen(name) <= IDSIZ;
}
