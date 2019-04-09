/*
 * dlist.c
 *
 *  Created on: Sep 13, 2018
 *      Author: z.j
 */

#include <stdlib.h>
#include <string.h>

#include "dlist.h"
#include "logger.h"

// -------------------------------
void dlist_init(DList *list, void (*destroy)(void *data))
{
    if (list != 0) {
        list->size = 0;
        list->destroy = destroy;
        list->head = 0;
        list->tail  = 0;
    } else {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "list is null");
    }
}

// -------------------------------
void dlist_destroy(DList *list)
{
    // TODO
}

// -------------------------------
int dlist_insert_next(DList *list, DListElmt *element, const void *data)
{
    if (list == 0 || data == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "list = %p, data = %p", list, data);
        return -1;
    }

    if (element == 0 && dlist_size(list) != 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "dlist_size(list) = %d", dlist_size(list));
        return -1;
    }

    if (element != 0 && dlist_size(list) == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "list is empty, element = %p", element);
        return -1;
    }

    DListElmt *new_element;
    if ((new_element == (DListElmt *)malloc(sizeof(DListElmt))) == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "malloc failed for new_element");
        return -1;
    }

    new_element->data = (void *)data;

    LOG_MSG(LOG_MODULE_ADS, TRACE, "insert new element = %p after element = %p", new_element, element);

    if (dlist_size(list) == 0) {
        // handle insertion when the list is empty
        list->head = new_element;
        list->tail = new_element;
        list->head->next = 0;
        list->head->prev = 0;
    } else {
        // handle insertion when the list is not empty
        new_element->next = element->next;
        new_element->prev = element;
        
        if (element->next == 0) {
            list->tail = new_element;
        } else {
            element->next->prev = new_element;
        }

        element->next = new_element;
    }

    list->size++;
    return 0;
}

// -------------------------------
int dlist_insert_prev(DList *list, DListElmt *element, const void *data)
{
    if (list == 0 || data == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "list = %p, data = %p", list, data);
        return -1;
    }

    if (element == 0 && dlist_size(list) != 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "dlist_size(list) = %d", dlist_size(list));
        return -1;
    }

    if (element != 0 && dlist_size(list) == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "list is empty, element = %p", element);
        return -1;
    }

    DListElmt *new_element;
    if ((new_element == (DListElmt *)malloc(sizeof(DListElmt))) == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "malloc failed for new_element");
        return -1;
    }

    LOG_MSG(LOG_MODULE_ADS, TRACE, "insert new element = %p before element = %p", new_element, element);

    new_element->data = (void *) data;
    if (dlist_size(list) == 0) {
        // empty list
        list->head = new_element;
        list->head->prev = 0;
        list->head->next = 0;
        list->tail = new_element;
    } else {
        // not empty
        new_element->next = element;
        new_element->prev = element->prev;

        if (element->prev == 0) {
            list->head = new_element;
        } else {
            element->prev->next = new_element;
        }

        element->prev = new_element;
    }

    list->size++;
    return 0;
}

// -------------------------------
int dlist_remove(DList *list, DListElmt *element, void **data)
{
    if (list == 0 || element == 0 || data == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "list = %p, element = %p, data = %p", list, element, data);
        return -1;
    }

    if (dlist_size(list) == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "empty list = %p", list);
        return -1;
    }

    LOG_MSG(LOG_MODULE_ADS, TRACE, "remove element = %p", element);
    *data = element->data;

    if (element == list->head) {
        list->head = element->next;

        if (list->head == 0) {
            list->tail  = 0;
        } else {
            list->head->prev = 0;
        }
    } else {
        element->prev->next = element->next;
        if (element->next == 0) {
            list->tail = element->prev;
        } else {
            element->next->prev = element->prev;
        }
    }

    free(element);

    list->size--;
    return 0;
}