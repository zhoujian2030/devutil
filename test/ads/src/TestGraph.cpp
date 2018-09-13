/*
 * TestGraph.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: J.ZH
 */

#include "TestGraph.h"
#include "bfs.h"
#include "dfs.h"
#include <iostream>
#include "logger.h"
#include <string>

// using namespace ads;
using namespace std;

int vertexMatch(const void *data1, const void *data2)
{
    if(data1 == data2) {
        BfsVertex *v1 = (BfsVertex *)data1;
        BfsVertex *v2 = (BfsVertex *)data2;

        if (v1 && v2) {
            return (v1->data == v2->data);
        }

        return 1;
    } else {
        return 0;
    }
}

void vertexDestroy(void *data) 
{
    if (data) {
        // int *p = (int*)data;
        delete (int*)data;
    }
}

TEST_F(TestGraph, bfs) 
{
    LoggerSetlevel(DEBUG);

    Graph *graph = new Graph;
    graph_init(graph, vertexMatch, vertexDestroy);
    int id = 1;

    BfsVertex *bv1 = new BfsVertex;
    int *vid1 = new int;
    *vid1 = id++;
    bv1->data = (void*)vid1;
    ASSERT_EQ(0, graph_insert_vertex(graph, (void*)bv1));

    BfsVertex *bv2 = new BfsVertex;
    int *vid2 = new int;
    *vid2 = id++;
    bv2->data = (void*)vid2;
    graph_insert_vertex(graph, (void*)bv2);
    graph_insert_edge(graph, (void*)bv1, (void*)bv2);
    graph_insert_edge(graph, (void*)bv2, (void*)bv1);

    BfsVertex *bv3 = new BfsVertex;
    int *vid3 = new int;
    *vid3 = id++;
    bv3->data = (void*)vid3;
    graph_insert_vertex(graph, (void*)bv3);
    graph_insert_edge(graph, (void*)bv1, (void*)bv3);
    graph_insert_edge(graph, (void*)bv2, (void*)bv3);
    graph_insert_edge(graph, (void*)bv3, (void*)bv1);
    graph_insert_edge(graph, (void*)bv3, (void*)bv2);

    BfsVertex *bv4 = new BfsVertex;
    int *vid4 = new int;
    *vid4 = id++;
    bv4->data = (void*)vid4;
    graph_insert_vertex(graph, (void*)bv4);
    graph_insert_edge(graph, (void*)bv2, (void*)bv4);
    graph_insert_edge(graph, (void*)bv4, (void*)bv2);

    BfsVertex *bv5 = new BfsVertex;
    int *vid5 = new int;
    *vid5 = id++;
    bv5->data = (void*)vid5;
    graph_insert_vertex(graph, (void*)bv5);
    graph_insert_edge(graph, (void*)bv3, (void*)bv5);
    graph_insert_edge(graph, (void*)bv4, (void*)bv5);
    graph_insert_edge(graph, (void*)bv5, (void*)bv3);
    graph_insert_edge(graph, (void*)bv5, (void*)bv4);

    BfsVertex *bv6 = new BfsVertex;
    int *vid6 = new int;
    *vid6 = id++;
    bv6->data = (void*)vid6;
    graph_insert_vertex(graph, (void*)bv6);
    graph_insert_edge(graph, (void*)bv5, (void*)bv6);
    graph_insert_edge(graph, (void*)bv6, (void*)bv5);

    /*************************    
             1
           /  \
          /    \
         2------3 
         \      /
          4----5 
              /
             6      
    **************************/
    BfsVertex *start = bv1;
    List hopList;
    ASSERT_EQ(0, bfs(graph, start, &hopList)); 
    ASSERT_EQ(6, list_size(&hopList));
    ASSERT_EQ(0, bv1->hops);
    ASSERT_EQ(1, bv2->hops);
    ASSERT_EQ(1, bv3->hops);
    ASSERT_EQ(2, bv4->hops);
    ASSERT_EQ(2, bv5->hops);
    ASSERT_EQ(3, bv6->hops);

    start = bv2;
    ASSERT_EQ(0, bfs(graph, start, &hopList)); 
    ASSERT_EQ(1, bv1->hops);
    ASSERT_EQ(0, bv2->hops);
    ASSERT_EQ(1, bv3->hops);
    ASSERT_EQ(1, bv4->hops);
    ASSERT_EQ(2, bv5->hops);
    ASSERT_EQ(3, bv6->hops);

    start = bv3;
    ASSERT_EQ(0, bfs(graph, start, &hopList)); 
    ASSERT_EQ(1, bv1->hops);
    ASSERT_EQ(1, bv2->hops);
    ASSERT_EQ(0, bv3->hops);
    ASSERT_EQ(2, bv4->hops);
    ASSERT_EQ(1, bv5->hops);
    ASSERT_EQ(2, bv6->hops);

    start = bv4;
    ASSERT_EQ(0, bfs(graph, start, &hopList)); 
    ASSERT_EQ(2, bv1->hops);
    ASSERT_EQ(1, bv2->hops);
    ASSERT_EQ(2, bv3->hops);
    ASSERT_EQ(0, bv4->hops);
    ASSERT_EQ(1, bv5->hops);
    ASSERT_EQ(2, bv6->hops);

    start = bv5;
    ASSERT_EQ(0, bfs(graph, start, &hopList)); 
    ASSERT_EQ(2, bv1->hops);
    ASSERT_EQ(2, bv2->hops);
    ASSERT_EQ(1, bv3->hops);
    ASSERT_EQ(1, bv4->hops);
    ASSERT_EQ(0, bv5->hops);
    ASSERT_EQ(1, bv6->hops);

    start = bv6;
    ASSERT_EQ(0, bfs(graph, start, &hopList)); 
    ASSERT_EQ(3, bv1->hops);
    ASSERT_EQ(3, bv2->hops);
    ASSERT_EQ(2, bv3->hops);
    ASSERT_EQ(2, bv4->hops);
    ASSERT_EQ(1, bv5->hops);
    ASSERT_EQ(0, bv6->hops);
}

// -------------------------------
TEST_F(TestGraph, dfs)
{
    LoggerSetlevel(DEBUG);

    Graph *graph = new Graph;
    graph_init(graph, vertexMatch, vertexDestroy);

    string courses[] = {"CS100", "CS200", "CS300", "MA100", "MA200", "MA300", "CS150"}; 
    int id = 0;

    DfsVertex *bv0 = new DfsVertex;
    int *vid0 = new int;
    *vid0 = id++;
    bv0->data = (void*)vid0;
    ASSERT_EQ(0, graph_insert_vertex(graph, (void*)bv0));

    DfsVertex *bv1 = new DfsVertex;
    int *vid1 = new int;
    *vid1 = id++;
    bv1->data = (void*)vid1;
    ASSERT_EQ(0, graph_insert_vertex(graph, (void*)bv1));
    ASSERT_EQ(0, graph_insert_edge(graph, (void*)bv0, (void*)bv1));

    DfsVertex *bv2 = new DfsVertex;
    int *vid2 = new int;
    *vid2 = id++;
    bv2->data = (void*)vid2;
    ASSERT_EQ(0, graph_insert_vertex(graph, (void*)bv2));
    ASSERT_EQ(0, graph_insert_edge(graph, (void*)bv1, (void*)bv2));

    DfsVertex *bv3 = new DfsVertex;
    int *vid3 = new int;
    *vid3 = id++;
    bv3->data = (void*)vid3;
    ASSERT_EQ(0, graph_insert_vertex(graph, (void*)bv3));
    ASSERT_EQ(0, graph_insert_edge(graph, (void*)bv3, (void*)bv2));

    DfsVertex *bv4 = new DfsVertex;
    int *vid4 = new int;
    *vid4 = id++;
    bv4->data = (void*)vid4;
    ASSERT_EQ(0, graph_insert_vertex(graph, (void*)bv4));
    ASSERT_EQ(0, graph_insert_edge(graph, (void*)bv3, (void*)bv4));
    ASSERT_EQ(0, graph_insert_edge(graph, (void*)bv0, (void*)bv4));

    DfsVertex *bv5 = new DfsVertex;
    int *vid5 = new int;
    *vid5 = id++;
    bv5->data = (void*)vid5;
    ASSERT_EQ(0, graph_insert_vertex(graph, (void*)bv5));
    ASSERT_EQ(0, graph_insert_edge(graph, (void*)bv4, (void*)bv5));
    ASSERT_EQ(0, graph_insert_edge(graph, (void*)bv2, (void*)bv5));

    DfsVertex *bv6 = new DfsVertex;
    int *vid6 = new int;
    *vid6 = id++;
    bv6->data = (void*)vid6;
    ASSERT_EQ(0, graph_insert_vertex(graph, (void*)bv6));

    /*
                CS100      MA100      CS150
                |        /  |
               \ /      /  \ /
                CS200  /   MA200 
                |     /     |
               \ /  \/_    \ /
                CS300 ---->MA300
    */

    List orderedList;
    ASSERT_EQ(0, dfs(graph, &orderedList));

    ASSERT_EQ(7, list_size(&orderedList));

    ListElmt *element;
    for (element = list_head(&orderedList); element != 0; element = list_next(element)) {
        id = *(int *)((DfsVertex *)list_data(element))->data;
        printf("%s(%d)  ", courses[id].c_str(), id);
    }
    printf("\n");
}
