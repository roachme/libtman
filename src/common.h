#ifndef LIBTMAN_COMMON_H
#define LIBTMAN_COMMON_H

/* use stdbool or add prefix or move it to cli part.  */
#define TRUE        1
#define FALSE       0

#define BOOL            int
#define TRUE            1
#define FALSE           0
#define PATHSIZ         4096
#define TMANPATHSIZE    1024

/* TODO: make it not global.  */
struct tmanstruct {
    char db[TMANPATHSIZE + 1];  /* directory for tman metadata */
    char base[TMANPATHSIZE + 1];        /* directory for all stuff above */
};

#endif
