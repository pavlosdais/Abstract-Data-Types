#include <stdio.h>
#include <stdlib.h>
#include "../lib/ADT.h"

int main(void)
{
    // create graph
    wu_graph A = wug_create(7);

    // the weighted undirected graph:
    // https://www.researchgate.net/profile/Soran-Saeed/publication/330778836/figure/fig2/AS:721420615168005@1549011486980/Example-of-Minimum-spanning-tree-11.jpg
    wug_insert(A, 1, 2, 1);
    wug_insert(A, 2, 3, 10);
    wug_insert(A, 2, 4, 15);
    wug_insert(A, 3, 4, 11);
    wug_insert(A, 4, 5, 6);
    wug_insert(A, 5, 6, 9);
    wug_insert(A, 6, 3, 2);
    wug_insert(A, 1, 6, 14);
    wug_insert(A, 1, 3, 9);

    // print the graph
    wug_print(A);
    
    // print minimum spanning tree
    printf("\nMinimum spanning tree edges:\n");
    wug_minspantree(A);

    // free graph
    wug_destroy(A);

    return 0;
}
