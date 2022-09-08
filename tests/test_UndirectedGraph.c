#include <stdio.h>
#include <stdlib.h>
#include "../lib/ADT.h"

// function prototype
void visit(Vertex x);

int main(void)
{
    // create graph
    undir_graph A;
    ug_init(&A, 5, visit);

    // random graph
    ug_insert(A, 4, 0);
    ug_insert(A, 2, 1);
    ug_insert(A, 3, 2);
    ug_insert(A, 2, 0);

    // print graph
    ug_print(A); printf("\n");

    // perfrom simple path check between vertices (3-1)
    ug_simplepathcheck(A, 3, 1);
    
    // free graph
    ug_destroy(A);

    return 0;
}

void visit(Vertex x)
{
    printf("%d ", x);
}