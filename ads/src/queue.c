/*
 * queue.c
 *
 *  Created on: Sep 6, 2018
 *      Author: z.j
 */

#include "queue.h"

// -------------------------
int queue_enqueue(Queue *queue, const void *data)
{
    return list_insert_next(queue, list_tail(queue), data);
}

// -------------------------
int queue_dequeue(Queue *queue, void **data)
{
    return list_remove_next(queue, 0, data);
}
