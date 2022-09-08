This is an implementations of [directed graph](https://en.wikipedia.org/wiki/Directed_graph) using adjecency list. A directed graph, also called a digraph, is a graph in which the edges have a direction. This is usually indicated with an arrow on the edge; more formally, if v and w are vertices, an edge is an unordered pair {v,w}.

# Performance
<img align="right" width=330 alt="directed graph picture" src="https://upload.wikimedia.org/wikipedia/commons/thumb/2/23/Directed_graph_no_background.svg/1200px-Directed_graph_no_background.svg.png">

If V is the number vertices and E is the number of edges in the graph:

Algorithm                      | Average case | Worst case
-----------------------------  | ---------    | ----------
Space	                       | Θ(V+E)       | O(V+E)
Insert edge                    | Θ(1)	      | O(V)
Print	                       | Θ(V+E)       | O(V+E)
DFS 	                       | Θ(V+E)	      | O(V+E)
Reverse                        | Θ(V+E)	      | O(V+E)
Topological Ordering           | Θ(V+E)	      | O(V+E)
Strongly-Connected Components  | Θ(V+E)	      | O(V+E)

The algorithm used to find the strongly-connected components of the graph is [Kosaraju's algorithm](https://en.wikipedia.org/wiki/Kosaraju%27s_algorithm).
