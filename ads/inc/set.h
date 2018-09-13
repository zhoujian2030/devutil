/*
 * set.h
 *
 *  Created on: Aug 28, 2018
 *      Author: z.j
 */

#ifndef SET_H
#define SET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "list.h"

typedef List Set;
typedef ListElmt SetElmt;

void set_init(Set *set, int (*match)(const void *key1, const void *key2), void (*destroy)(void * data));

int set_insert(Set *set, const void *data);
int set_remove(Set *set, void **data);

int set_is_member(const Set *set, const void *data);

#define set_size(set)   ((set)->size)

#ifdef __cplusplus
}
#endif

#endif
