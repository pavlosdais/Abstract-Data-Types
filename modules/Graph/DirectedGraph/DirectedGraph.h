#pragma once  // include at most once

#include <stdint.h>

// needed typedefs
typedef int Vertex;
typedef struct _dir_graph* dir_graph;

// Pointer to function that visits the vertices
typedef void (*VisitFunc)(Vertex value);


// creates directed graph
dir_graph dg_create(const uint32_t, const VisitFunc);

// inserts edge (A-B) at the graph
void dg_insert(const dir_graph, const Vertex A, const Vertex B);

// prints the graph
void dg_print(const dir_graph);

// depth first traversal of the graph
// prints tree, forward, back and cross edges (colored)
void dg_dfs(const dir_graph);

// returns reversed graph
dir_graph dg_reverse(const dir_graph);

// prints a topological ordering of the graph
void dg_bts(const dir_graph);

// prints strongly-connected components using Kosaraju's algorithm
void dg_scc(const dir_graph);

// frees graph
void dg_destroy(const dir_graph);
