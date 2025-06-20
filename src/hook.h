#ifndef LIBTMAN_PGN_H
#define LIBTMAN_PGN_H

#include "libtman.h"
#include "unit.h"
#include "limit.h"

#define HOOKSIZ     1000

int ispgn(char *pgndir, const char *pgname);
int hookact(struct tman_hook *hooks, char *cmd, char *prj, char *id);
struct unit *hookshow(struct tman_hook *hooks, char *prj, char *id, char *cmd);
char *hookls(struct tman_hook *hooks, char *pgnout, char *prj, char *id);

#endif
