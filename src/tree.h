#ifndef LIBTMAN_TREE_H
#define LIBTMAN_TREE_H

#include "common.h"

struct tree {
    int mark;
    int status;                 /* error code in case of invalid value */
    int colprio;
    char id[IDSIZ + 1];
    char desc[DESCSIZ + 1];
    struct tree *left;
    struct tree *right;
};

struct tree *tree_alloc(char *id, int col, char *desc);
struct tree *tree_add(struct tree *parent, struct tree *node);
struct tree *tree_free(struct tree *parent);
void tree_print(struct tree *parent);

#endif
