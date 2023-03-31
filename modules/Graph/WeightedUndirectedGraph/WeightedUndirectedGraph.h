#pragma once  // include at most once

#include <stdint.h>


// needed typedefs
typedef int Vertex;
typedef struct _wu_graph* wu_graph;
typedef uint32_t cost;  // the cost of the edge


// creates weighted undirected graph
wu_graph wug_create(const uint32_t);

// inserts edge (A-B) with its weight at the graph
void wug_insert(const wu_graph, const Vertex A, const Vertex B, const cost);

// prints the graph
void wug_print(const wu_graph);

// prints minimum spanning tree, as well as its total weight using the prim-jarnik algorithm
void wug_minspantree(const wu_graph);

// destroys the memory used by the weighted undirected graph
void wug_destroy(const wu_graph);
