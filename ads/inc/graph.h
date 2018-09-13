/*
 * graph.h
 *
 *  Created on: Aug 28, 2018
 *      Author: z.j
 */

#ifndef GRAPH_H
#define GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "set.h"

typedef struct AdjList_
{
    void *vertex;
    Set adjacent;
} AdjList;

typedef struct Graph_ 
{
    int vcount;
    int ecount;

    int  (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);

    List adjlists;
} Graph;

typedef enum VertexColor_ {
    white,
    gray,
    black
} VertexColor;

void graph_init(Graph *graph, int (*match)(const void *key1, const void *key2), void (*destroy)(void *data));
int graph_insert_vertex(Graph *graph, const void *data);
int graph_insert_edge(Graph *graph, const void *data1, const void *data2);
int graph_remove_vertex(Graph *graph, void **data);
int graph_remove_edge(Graph *graph, void *data1, void **data2);
int graph_get_adjlist(const Graph *graph, const void *data, AdjList **adjlist);
int graph_is_adjacent(const Graph *graph, const void *data1, const void *data2);

#define graph_adjlists(graph)   ((graph)->adjlists)
#define graph_vcount(graph)     ((graph)->vcount)
#define graph_ecount(graph)     ((graph)->ecount)


#ifdef __cplusplus
}
#endif

#endif

