#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <getopt.h>

#include "project.h"
#include "limit.h"

/* use stdbool or add prefix or move it to cli part.  */
#define TRUE        1
#define FALSE       0

#define DESCSIZ         80
#define PGNOUTSIZ       80

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define xstr(s) str(s)
#define str(s) #s

#define BOOL            int
#define TRUE            1
#define FALSE           0
#define IDSIZ           10
#define PATHSIZ         4096
#define TMANPATHSIZE    1024
#define FINITSIZ TMANPATHSIZE + 6 + 10  /* filename + padding */
#define FSTATESIZ TMANPATHSIZE + 6 + 10 /* filename + padding */

/* TODO: make it not global.  */
struct tmanstruct {
    char db[TMANPATHSIZE + 1];  /* directory for tman metadata */
    char cfg[TMANPATHSIZE + 1]; /* directory for config files */
    char pgn[TMANPATHSIZE + 1]; /* directory for plugin data */
    char base[TMANPATHSIZE + 1];        /* directory for all stuff above */
    char pgnins[TMANPATHSIZE + 1];      /* directory for installed plugins */

    char fcfg[TMANPATHSIZE + 1];        /* system config file */
    char finit[FINITSIZ + 1];   /* file that tells that tman is inited */
    char fstate[FSTATESIZ + 1]; /* file to store tman task state */
};

extern struct tmanstruct tmanfs;

char *genpath_prj(char *prj);
char *genpath_unit_prj(char *prj);
char *genpath_col(char *prj, char *id);
char *genpath_full(char *prj, char *id);
char *genpath_unit(char *prj, char *id);
char *genpath_pgn(char *prj, char *id, char *name, char *cmd);

#endif
