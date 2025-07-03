#ifndef LIBTMAN_DIR_H
#define LIBTMAN_DIR_H

#include "libtman.h"

int dir_id_add(char *base, tman_arg_t * args);
int dir_id_del(char *base, char *prj, char *id);
int dir_id_rename(char *base, char *srcprj, char *dstprj, char *srcid,
                  char *dstid);
int dir_id_move(char *base, char *id, char *dstprj, char *srcprj);

int dir_brd_add(char *base, char *prj, char *brd);

int dir_prj_add(char *base, char *prj);
int dir_prj_del(char *base, char *prj);
int dir_prj_rename(char *base, char *src, char *dst);

#endif
