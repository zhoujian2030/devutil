/*
 * bfs.h
 *
 *  Created on: Sep 6, 2018
 *      Author: z.j
 */

#ifndef BFS_H
#define BFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "graph.h"
#include "list.h"

typedef struct BfsVertex_
{
    void *data;
    VertexColor color;
    int hops;
} BfsVertex;

int bfs(Graph *graph, BfsVertex *start, List *hops);

#ifdef __cplusplus
}
#endif

#endif
