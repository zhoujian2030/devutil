/*
 * dfs.h
 *
 *  Created on: Sep 12, 2018
 *      Author: z.j
 */

#ifndef DFS_H
#define DFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "graph.h"
#include "list.h"

typedef struct DfsVertex_
{
    void *data;
    VertexColor color;
} DfsVertex;

int dfs(Graph *graph, List *ordered);

#ifdef __cplusplus
}
#endif

#endif
