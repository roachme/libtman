#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"
#include "osdep.h"

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
    char *pathname = path_task_dir(base, args);
    return ISDIR(pathname);
}
