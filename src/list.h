#ifndef LIBTMAN_LIST_H
#define LIBTMAN_LIST_H

#include "libtman.h"

void list_free(tman_list_t * head);
tman_list_t *list_add(tman_list_t * head, char *id, int status);

#endif
