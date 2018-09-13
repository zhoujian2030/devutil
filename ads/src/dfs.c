/*
 * dfs.c
 *
 *  Created on: Sep 12, 2018
 *      Author: z.j
 */

#include "dfs.h"
#include <stdlib.h>
#include "logger.h"

// ----------------------------
static int dfs_main(Graph *graph, AdjList *adjlist, List *ordered)
{
    AdjList     *clr_adjlist;
    DfsVertex   *clr_vertex, *adj_vertex;
    ListElmt    *member;

    LOG_MSG(LOG_MODULE_ADS, DEBUG, "vertex = %p, color = %d, id = %d", 
        adjlist->vertex, ((DfsVertex *)adjlist->vertex)->color, *(int *)((DfsVertex *)adjlist->vertex)->data);

    // color the verex gray and traverse its adjacency list
    ((DfsVertex *)adjlist->vertex)->color = gray;

    for (member = list_head(&adjlist->adjacent); member != 0; member = list_next(member)) {
        // determine the color of the next adjacent vertext
        adj_vertex = list_data(member);

        if (graph_get_adjlist(graph, adj_vertex, &clr_adjlist) != 0) {
            LOG_MSG(LOG_MODULE_ADS, ERROR, "graph_adjlists failed, adj_vertex = %p", adj_vertex);
            return -1;
        }

        clr_vertex = clr_adjlist->vertex;
        
        // move one vertex deeper when the next adjacent vertex is white
        if (clr_vertex->color == white) {
             if (dfs_main(graph, clr_adjlist, ordered) != 0) {
                 LOG_MSG(LOG_MODULE_ADS, ERROR, "dfs_main failed, clr_adjlist = %p", clr_adjlist);
                 return -1;
             }
        }
    }

    // color the current vertex black and make it the first in the list
    ((DfsVertex *)adjlist->vertex)->color = black;
    LOG_MSG(LOG_MODULE_ADS, DEBUG, "BLACK: vertex = %p, color = %d, id = %d", 
        adjlist->vertex, ((DfsVertex *)adjlist->vertex)->color, *(int *)((DfsVertex *)adjlist->vertex)->data);

    if (list_insert_next(ordered, 0, (DfsVertex *)adjlist->vertex) != 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "list_insert_next failed");
        return -1;
    }

    return 0;
}

// ----------------------------
int dfs(Graph *graph, List *ordered)
{
    DfsVertex   *vertex;
    ListElmt    *element;

    // initialize all of the vertices in the graph
    for (element = list_head(&graph_adjlists(graph)); element != 0; element = list_next(element)) {
        vertex = ((AdjList *)list_data(element))->vertex;
        vertex->color = white;
    }

    // perform dfs
    list_init(ordered, 0);

    for (element = list_head(&graph_adjlists(graph)); element != 0; element = list_next(element)) {
        // ensure that every component of unconnected graphs is searched
        vertex = ((AdjList *)list_data(element))->vertex;

        LOG_MSG(LOG_MODULE_ADS, DEBUG, "vertex = %p, id = %d", vertex, *(int *)vertex->data);

        if (vertex->color == white) {
            if (dfs_main(graph, (AdjList *)list_data(element), ordered) != 0) {
                LOG_MSG(LOG_MODULE_ADS, ERROR, "dfs_main failed");
                // list_destroy(ordered); todo
                return -1;
            }
        }
    }

    return 0;
}