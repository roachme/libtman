#include <string.h>
#include <stdlib.h>

#include "list.h"

void list_free(tman_list_t * head)
{
    tman_list_t *p, *q;

    for (p = head; p != NULL; p = q) {
        q = p->next;
        free(p->name);
        free(p);
    }
}

tman_list_t *list_add(tman_list_t * head, char *id, int status)
{
    tman_list_t *node;

    if ((node = malloc(sizeof(tman_list_t))) == NULL)
        return NULL;
    else if ((node->name = strdup(id)) == NULL)
        return NULL;
    node->status = status;
    node->next = head;
    return node;
}
