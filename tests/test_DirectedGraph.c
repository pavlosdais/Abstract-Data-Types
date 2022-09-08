#include <stdio.h>
#include <stdlib.h>
#include "../lib/ADT.h"

// function prototype
void visit(Vertex x);

int main(void)
{
    dir_graph A;
    dg_init(&A, 6, visit);
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