#include <stdio.h>
#include <dirent.h>

#include "aux.h"
#include "unit.h"
#include "list.h"
#include "valid.h"

int aux_unit_set(tman_unit_t * newunits, const char *fname)
{
    struct tman_unit *item;
    struct tman_unit *units;

    if ((units = unit_load(fname)) == NULL)
        return emod_set(LIBTMAN_UNIT_LOAD);

    // TODO: check values in atomic way

    for (item = newunits; item; item = item->next)
        unit_set(units, item->key, item->val);
    if (unit_save(fname, units))
        return emod_set(LIBTMAN_UNIT_SAVE);

    unit_free(units);
    return LIBTMAN_OK;
}

int aux_list_get(tman_ctx_t * ctx, const char *fname)
{
    DIR *ids;
    int status;
    tman_arg_t args;
    struct dirent *ent;

    if ((ids = opendir(fname)) == NULL)
        return emod_set(LIBTMAN_DIR_PRJ_OPEN);

    while ((ent = readdir(ids)) != NULL) {
        status = LIBTMAN_OK;
        args.taskid = ent->d_name;

        if (ent->d_name[0] == '.' || ent->d_type != DT_DIR)
            continue;
        else if (valid_task_name(args.taskid) == FALSE)
            status = LIBTMAN_ID_ILLEG;

        ctx->list = list_add(ctx->list, args.taskid, status);
    }

    closedir(ids);
    return LIBTMAN_OK;
}
