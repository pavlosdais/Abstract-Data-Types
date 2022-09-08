#pragma once  // include at most once

// needed typedefs
typedef int Vertex;
typedef struct _wu_graph* wu_graph;
typedef unsigned int cost;

// initializes weighted undirected graph
void wug_init(wu_graph*, unsigned int num_of_vertices);

// inserts edge (A-B) with its weight at the graph
void wug_insert(wu_graph, Vertex A, Vertex B, cost);

// prints the graph
void wug_print(wu_graph);

// prints minimum spanning tree, as well as its total weight using the prim-jarnik algorithm
void wug_minspantree(wu_graph);

// frees graph
void DestroyGraph(wu_graph);
