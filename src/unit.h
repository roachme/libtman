#ifndef LIBTMAN_UNIT_H
#define LIBTMAN_UNIT_H

#include "libtman.h"

tman_unit_t *unit_load(const char *filename);
tman_unit_t *unit_parse(tman_unit_t * head, const char *str);
tman_unit_t *unit_add(tman_unit_t * head, const char *key, const char *val);
tman_unit_t *unit_set(tman_unit_t * head, const char *key, const char *val);
int unit_save(const char *filename, tman_unit_t * head);
char *unit_get(tman_unit_t * head, const char *key);
void unit_free(tman_unit_t * head);

#endif
