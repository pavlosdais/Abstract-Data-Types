#include <stdio.h>
#include <stdlib.h>
#include "../lib/ADT.h"

// function prototype
void visit(Vertex x);

int main(void)
{
    // create graph
    dir_graph A = dg_create(6, visit);

    // the directed graph:
    // https://cgi.di.uoa.gr/~k08/manolis/2021-2022/lectures/Graphs.pdf , page 139
    dg_insert(A, 1, 4);
    dg_insert(A, 1, 2);
    dg_insert(A, 2, 3);
    dg_insert(A, 5, 2);
    dg_insert(A, 3, 4);
    dg_insert(A, 3, 1);

    // print graph
    printf("\nGraph:\n");
    dg_print(A); printf("\n");
    
    // dfs at graph
    printf("DFS:\n");
    dg_dfs(A); printf("\n");

    // print topological ordering of graph
    printf("Topological ordering of the graph:\n");
    dg_bts(A); printf("\n");

    // print strongly connected components
    printf("Strongly-Connected Components:\n");
    dg_scc(A);

    // free the graph
    dg_destroy(A);

    return 0;
}

void visit(Vertex x)
{
    printf("%d ", x);
}