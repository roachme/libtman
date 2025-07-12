#ifndef LIBTMAN_COMMON_H
#define LIBTMAN_COMMON_H

#include "limit.h"

/* use stdbool or add prefix or move it to cli part.  */
#define TRUE        1
#define FALSE       0

#define DESCSIZ         180

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define xstr(s) str(s)
#define str(s) #s

#define BOOL            int
#define TRUE            1
#define FALSE           0
#define PATHSIZ         4096
#define TMANPATHSIZE    1024
#define FINITSIZ TMANPATHSIZE + 6 + 10  /* filename + padding */
#define FSTATESIZ TMANPATHSIZE + 6 + 10 /* filename + padding */

/* TODO: make it not global.  */
struct tmanstruct {
    char db[TMANPATHSIZE + 1];  /* directory for tman metadata */
    char base[TMANPATHSIZE + 1];        /* directory for all stuff above */
    char pgn[TMANPATHSIZE + 1]; /* directory for installed plugins */
    char fcfg[TMANPATHSIZE + 1];        /* system config file */
};

extern struct tmanstruct tmanfs;

#endif
