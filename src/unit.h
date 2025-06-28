#ifndef LIBTMAN_UNIT_H
#define LIBTMAN_UNIT_H

#include "libtman.h"

#define KEYSIZ 20
#define VALSIZ 80

void unit_free(struct tman_unit *units);
struct tman_unit *unit_load(char *filename);
char *unit_get(struct tman_unit *head, char *key);
int unit_set(struct tman_unit *head, char *key, char *val);
int unit_save(char *filename, struct tman_unit *units);
struct tman_unit *unit_add(struct tman_unit *head, char *key, char *val);

#endif
