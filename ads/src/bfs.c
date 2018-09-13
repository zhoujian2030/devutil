/*
 * bfs.c
 *
 *  Created on: Sep 6, 2018
 *      Author: z.j
 */

#include "bfs.h"
#include "queue.h"
#include "logger.h"

// ----------------------------
int bfs(Graph *graph, BfsVertex *start, List *hops)
{
    Queue       queue;
    AdjList     *adjlist, *clr_adjlist;
    BfsVertex   *clr_vertex, *adj_vertex;
    ListElmt    *element, *member;

    LOG_MSG(LOG_MODULE_ADS, DEBUG, "execute bfs, start = %p", start);

    for (element = list_head(&graph_adjlists(graph)); element != 0; element = list_next(element)) {
        clr_vertex = ((AdjList *)list_data(element))->vertex;
        if (graph->match(clr_vertex, start)) {
            clr_vertex->color = gray;
            clr_vertex->hops = 0;
        } else {
            clr_vertex->color = white;
            clr_vertex->hops = -1;
        }
    }

    // initialize the queue with the adjacency list of the start vertex
    queue_init(&queue, 0);
    if (graph_get_adjlist(graph, start, &clr_adjlist) != 0) {
        queue_destroy(&queue);
        LOG_MSG(LOG_MODULE_ADS, ERROR, "graph_get_adjlist failed, start = %p", start);
        
        return -1;
    }

    if (queue_enqueue(&queue, clr_adjlist) != 0) {
        queue_destroy(&queue);
        LOG_MSG(LOG_MODULE_ADS, ERROR, "queue_enqueue failed, start = %p", start);
        return -1;
    }

    // perform bfs
    while (queue_size(&queue) > 0) {
        adjlist = queue_peek(&queue);

        // traverse each vertex int the current adjacency list
        for (member = list_head(&adjlist->adjacent); member != 0; member = list_next(member)){
            adj_vertex = list_data(member);

            // determine the color of the next adjacent vertex
            if (graph_get_adjlist(graph, adj_vertex, &clr_adjlist) != 0) {
                queue_destroy(&queue);
                LOG_MSG(LOG_MODULE_ADS, ERROR, "graph_get_adjlist failed, adj_vertex = %p", adj_vertex);
                return -1;
            }
            clr_vertex = clr_adjlist->vertex;

            if (clr_vertex->color == white) {
                clr_vertex->color = gray;
                clr_vertex->hops = ((BfsVertex *)adjlist->vertex)->hops + 1;
                LOG_MSG(LOG_MODULE_ADS, DEBUG, "hops = %d, node data = %d", clr_vertex->hops, *(int*)clr_vertex->data);

                if (queue_enqueue(&queue, clr_adjlist) != 0) {
                    queue_destroy(&queue);
                    LOG_MSG(LOG_MODULE_ADS, ERROR, "queue_enqueue failed, adj_vertex = %p", adj_vertex);
                    return -1;
                }
            }
        }

        // dequeue the current adjacency list and color its vertex black
        if (queue_dequeue(&queue, (void **)&adjlist) == 0) {
            ((BfsVertex *)adjlist->vertex)->color = black;
        } else {
            queue_destroy(&queue);
            LOG_MSG(LOG_MODULE_ADS, ERROR, "queue_enqueue failed");
            return -1;
        }
    }

    queue_destroy(&queue);

    //
    list_init(hops, 0);

    for (element = list_head(&graph_adjlists(graph)); element != 0; element = list_next(element)){
        // skip vertices that were not visited
        clr_vertex = ((AdjList *)list_data(element))->vertex;
        if (clr_vertex->hops != -1) {
            if (list_insert_next(hops, list_tail(hops), clr_vertex) != 0) {
                // list_destroy todo
                LOG_MSG(LOG_MODULE_ADS, ERROR, "list_insert_next failed");
                return -1;
            }
        }
    }

    return 0;
}