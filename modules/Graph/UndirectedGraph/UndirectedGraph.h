#pragma once  // include at most once

#include <stdint.h>


typedef int Vertex;
typedef struct _undir_graph* undir_graph;

// Pointer to function that visits the vertices
typedef void (*VisitFunc)(Vertex value);


// creates undirected graph
// -requires a visit function
undir_graph ug_create(const uint32_t, const VisitFunc);

// inserts edge (A-B) at the graph
void ug_insert(undir_graph, Vertex A, Vertex B);

// prints the graph
void ug_print(const undir_graph);

// prints a simple path, meaning a path where each vertex is visited at most once, between vertices start and goal, if such path exists
void ug_simplepathcheck(const undir_graph, const Vertex start, const Vertex goal);

// destroys the memory used by the undirected graph
void ug_destroy(const undir_graph);
