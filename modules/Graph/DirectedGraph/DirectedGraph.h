#pragma once  // include at most once

// needed typedefs
typedef int Vertex;
typedef struct _dir_graph* dir_graph;

// Pointer to function that visits the vertices
typedef void (*VisitFunc)(Vertex value);

// initializes directed graph
void dg_init(dir_graph*, unsigned int num_of_vertices, VisitFunc visit);

// inserts edge (A-B) at the graph
void dg_insert(dir_graph, Vertex A, Vertex B);

// prints the graph
void dg_print(dir_graph);

// depth first traversal of the graph
// prints tree, forward, back and cross edges (colored)
void dg_dfs(dir_graph);

// returns reversed graph
dir_graph dg_reverse(dir_graph);

// prints a topological ordering of the graph
void dg_bts(dir_graph);

// prints strongly-connected components using Kosaraju's algorithm
void dg_scc(dir_graph);

// frees graph
void dg_destroy(dir_graph);
