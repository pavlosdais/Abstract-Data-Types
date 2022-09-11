#pragma once  // include at most once

// needed typedefs
typedef int Vertex;
typedef struct _undir_graph* undir_graph;

// Pointer to function that visits the vertices
typedef void (*VisitFunc)(Vertex value);

// initializes undirected graph
void ug_init(undir_graph* G, unsigned int num_of_vertices, VisitFunc visit);

// inserts edge (A-B) at the graph
void ug_insert(undir_graph G, Vertex A, Vertex B);

// prints the graph
void ug_print(undir_graph A);

// prints a simple path, meaning a path where each vertex is visited at most once, between vertices start and goal, if such path exists
void ug_simplepathcheck(undir_graph G, Vertex start, Vertex goal);

// frees graph
void ug_destroy(undir_graph G);
