/*
 * queue.h
 *
 *  Created on: Sep 6, 2018
 *      Author: z.j
 */

#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "list.h"

typedef List Queue;

#define queue_init list_init
#define queue_size list_size
#define queue_destroy(queue)

int queue_enqueue(Queue *queue, const void *data);
int queue_dequeue(Queue *queue, void **data);
#define queue_peek(queue)   ((queue)->head == 0 ? 0 : (queue)->head->data)

#ifdef __cplusplus
}
#endif

#endif