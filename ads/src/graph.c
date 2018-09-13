/*
 * graph.c
 *
 *  Created on: Aug 28, 2018
 *      Author: z.j
 */

#include "graph.h"
#include "list.h"
#include "set.h"
#include "logger.h"

// ------------------------------
void graph_init(Graph *graph, 
    int (*match)(const void *key1, const void *key2), 
    void (*destroy)(void *data))
{
    if (graph) {
        graph->vcount = 0;
        graph->ecount = 0;
        graph->match = match;
        graph->destroy = destroy;

        list_init(&graph->adjlists, 0);
    }
}

// ------------------------------
int graph_insert_vertex(Graph *graph, const void *data)
{
    ListElmt *element;
    AdjList *adjlist;
    int retval;

    // check if exists
    for (element = list_head(&graph->adjlists); element != 0; element = list_next(element)) {
        if (graph->match(data, ((AdjList *)list_data(element))->vertex)) {
            LOG_MSG(LOG_MODULE_ADS, WARNING, "data = %p exists!", data);
            return 1;
        }
    }

    if ((adjlist = (AdjList *)malloc(sizeof(AdjList))) == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "failed to malloc memory for AdjList");
        return -1;
    }

    adjlist->vertex = (void *)data;
    set_init(&adjlist->adjacent, graph->match, 0);

    if ((retval = list_insert_next(&graph->adjlists, list_tail(&graph->adjlists), adjlist)) != 0) {
        return retval;
    }

    graph->vcount++;
    return 0;
}

int graph_insert_edge(Graph *graph, const void *data1, const void *data2)
{
    ListElmt *element;
    int retval;

    for (element = list_head(&graph->adjlists); element != 0; element = list_next(element)) {
        if (graph->match(data2, ((AdjList *)list_data(element))->vertex)) {
            break;
        }
    }

    if (element == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "data2 = %p not exists", data2);
        return -1;
    }

    for (element = list_head(&graph->adjlists); element != 0; element = list_next(element)) {
        if (graph->match(data1, ((AdjList *)list_data(element))->vertex)) {
            break;
        }
    }

    if (element == 0) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "data1 = %p not exists", data1);
        return -1;
    }

    if ((retval = set_insert(&((AdjList *)list_data(element))->adjacent, data2)) != 0) {
        return retval;
    }

    graph->ecount++;
    return 0;
}

// ------------------------------
int graph_remove_vertex(Graph *graph, void **data)
{
    ListElmt *element, *temp, *prev;
    AdjList *adjlist;
    int found;

    prev = 0;
    found = 0;

    for (element = list_head(&graph->adjlists); element != 0; element = list_next(element)) {
        // Do not remove a vertex if it is in an adjacency list
        if (set_is_member(&((AdjList *)list_data(element))->adjacent, *data)) {
            return -1;
        }

        if (graph->match(*data, ((AdjList *)list_data(element))->vertex)) {
            temp = element;
            found = 1;
        }

        if (!found) {
            prev = element;
        }
    }

    // not found the vertex
    if (!found) {
        return -1;
    }

    // Do not allow to remove the vertex if its adjacency list is not empty
    if (set_size(&((AdjList *)list_data(element))->adjacent) > 0) {
        return -1;
    }

    if (list_remove_next(&graph->adjlists, prev, (void **)&adjlist) != 0) {
        return -1;
    }

    *data = adjlist->vertex;
    free(adjlist);

    graph->vcount--;
    return 0;
}

// ------------------------------
int graph_remove_edge(Graph *graph, void *data1, void **data2)
{
    ListElmt *element;

    // locate the adjacency list for the first vertex
    for (element = list_head(&graph->adjlists); element != 0; element = list_next(element)) {
        if (graph->match(data1, ((AdjList *)list_data(element))->vertex)) {
            break;
        }
    }

    if (element == 0) {
        return -1;
    }

    // remove the second vertex from the adjacency list of the first vertex
    if (set_remove(&((AdjList *)list_data(element))->adjacent, data2) != 0) {
        return -1;
    }

    graph->ecount--;
    return 0;
}

// -----------------------------
int graph_get_adjlist(const Graph *graph, const void *data, AdjList **adjlist)
{
    ListElmt *element, *prev = 0;

    for (element = list_head(&graph->adjlists); element != 0; element = list_next(element)) {
        if (graph->match(data, ((AdjList *)list_data(element))->vertex)) {
            break;
        }

        prev = element;
    }

    if (element == 0) {
        printf("data = %p not exists!\n", data);
        return -1;
    } 

    *adjlist = list_data(element);
    return 0;
}

// -----------------------------
int graph_is_adjacent(const Graph *graph, const void *data1, const void *data2)
{
    ListElmt *element, *prev = 0;

    for (element = list_head(&graph->adjlists); element != 0; element = list_next(element)) {
        if (graph->match(data1, ((AdjList *)list_data(element))->vertex)) {
            break;
        }

        prev = element;
    }

    if (element == 0) {
        return 0;
    }

    // return whether the 2nd vertex is in the adjacency list of the first
    return set_is_member(&((AdjList *)list_data(element))->adjacent, data2);
}
