#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "unit.h"

int unit_save(char *filename, struct tman_unit *units)
{
    FILE *fp;

    if ((fp = fopen(filename, "w")) == NULL)
        return 1;

    for (; units; units = units->next)
        fprintf(fp, "%s : %s\n", units->key, units->val);
    return fclose(fp);
}

/*
 * Get units all units.
*/
struct tman_unit *unit_load(char *filename)
{
    FILE *fp;
    struct tman_unit *units;
    char buf[BUFSIZ + 1];
    char key[KEYSIZ + 1];
    char val[VALSIZ + 1];

    if ((fp = fopen(filename, "r")) == NULL)
        return NULL;

    // TODO: write a better parser
    units = NULL;
    while (fgets(buf, BUFSIZ, fp) != NULL) {
        char *token;
        if ((token = strtok(buf, " :\n")) != NULL)
            strcpy(key, token);
        if ((token = strtok(NULL, ":\n")) != NULL)
            strcpy(val, token);
        units = unit_add(units, key, &val[1]);
    }
    fclose(fp);
    return units;
}

/*
 * Add units one at a time.
*/
struct tman_unit *unit_add(struct tman_unit *head, char *key, char *val)
{
    struct tman_unit *unit, *tmp;

    if ((unit = malloc(sizeof(struct tman_unit))) == NULL)
        return head;

    unit->next = NULL;
    unit->key = strdup(key);
    unit->val = strdup(val);

    if ((tmp = head) == NULL)
        return unit;

    for (tmp = head; tmp && tmp->next; tmp = tmp->next) ;
    tmp->next = unit;
    return head;
}

/*
 * Set units one at a time.
*/
int unit_set(struct tman_unit *head, char *key, char *val)
{
    for (; head != NULL; head = head->next) {
        if (strcmp(head->key, key) == 0) {
            free(head->val);
            head->val = strdup(val);
            return 0;
        }
    }
    return 1;
}

char *unit_get(struct tman_unit *head, char *key)
{
    while (head) {
        if (strcmp(head->key, key) == 0)
            return head->val;
        head = head->next;
    }
    return NULL;
}

/*
 * Delete all units.
*/
void unit_free(struct tman_unit *units)
{
    struct tman_unit *tmp;

    while (units) {
        tmp = units->next;
        free(units->key);
        free(units->val);
        free(units);
        units = tmp;
    }
}
