/*
 * list.c
 *
 *  Created on: Aug 27, 2018
 *      Author: z.j
 */

#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "logger.h"

// -------------------------------
void list_init(List *list, void (*destroy)(void *data))
{
    if (list) {
        list->size = 0;
        list->head = 0;
        list->tail = 0;
        list->destroy = destroy;
    }
}
// -------------------------------
int list_insert_next(List *list, ListElmt *element, const void *data)
{
    if (list == 0 || data == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "list = %p, data = %p", list, data);
        return -1;
    }

    ListElmt *new_element = (ListElmt*)malloc(sizeof(ListElmt));
    if (new_element == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "malloc failed for ListElmt");
        return -1;
    }

    new_element->data = (void*)data;

    if (element == 0) {
        // insert into head
        if (list_size(list) == 0) {
            list->tail = new_element;
        }

        new_element->next = list->head;
        list->head = new_element;
    } else {
        // insert somewhere other than at the head
        if (element->next == 0) {
            list->tail = new_element;
        }

        new_element->next = element->next;
        element->next = new_element;
    }

    list->size++;

    return 0;
}

// -------------------------------
int list_remove_next(List *list, ListElmt *element, void **data)
{
    if (list == 0 || data == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "list = %p, data = %p", list, data);
        return -1;
    }

    if (list_size(list) == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "empty list");
        return -1;
    }

    ListElmt* old_element;

    if (element == 0) {
        // remove from head
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if (list_size(list) == 1) {
            list->tail = 0;
        }
    } else {
        // return if this is the tail
        if (element->next == 0) {
            LOG_MSG(LOG_MODULE_ADS, ERROR, "empty next, element = %p", element);
            return -1;
        }

        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        if (element->next == 0) {
            list->tail = element;
        }
    }

    free(old_element);

    list->size--;

    return 0;
}
