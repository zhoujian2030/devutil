/*
 * set.c
 *
 *  Created on: Aug 28, 2018
 *      Author: z.j
 */

#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "set.h"

// ------------------------------
void set_init(Set *set, int (*match)(const void *key1, const void *key2), void (*destroy)(void * data))
{
    list_init(set, destroy);
    set->match = match;
}

// ------------------------------
int set_insert(Set *set, const void *data)
{
    if (set_is_member(set, data)) {
        return 1;
    }

    return list_insert_next(set, list_tail(set), data);
}

// ------------------------------
int set_remove(Set *set, void **data) 
{
    SetElmt *element, *prevElement = 0;

    for (element = list_head(set); element != 0; element = list_next(element)) {
        if (set->match(*data, list_data(element))) {
            break;
        }

        prevElement = element;
    }

    // set is empty
    if (element == 0) {
        return -1;
    }

    return list_remove_next(set, prevElement, data);
}

// ------------------------------
int set_is_member(const Set *set, const void *data)
{
    SetElmt *member;

    for (member = list_head(set); member != 0; member = list_next(member)) {
        if (set->match(data, list_data(member))) {
            return 1;
        }
    }

    return 0;
}
