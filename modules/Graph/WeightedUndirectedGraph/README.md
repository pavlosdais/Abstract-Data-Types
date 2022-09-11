This is an implementations of [weighted undirected graph](https://www.codewars.com/kata/5aaea7a25084d71006000082) using adjecency list. A weighted undirected graph is the same as a undirected graph except that each edge has a weight, or cost, associated with it.

# Performance
<img align="right" width=360 alt="weighted undirected graph picture" src="https://study.com/cimages/multimages/16/weighted_graph4176817323068517144.png">

If V is the number vertices and E is the number of edges in the graph:

Algorithm             | Average case | Worst case
---------------       | ---------    | ----------
Space	              | Θ(V+E)       | O(V+E)
Insert edge           | Θ(1)	     | O(1)
Print	              | Θ(V+E)	     | O(V+E)
Minimum spanning tree | O(Elog V)    | O(Elog V)

[Prim-Jarnik's algorithm](https://en.wikipedia.org/wiki/Prim%27s_algorithm) is used to find the minimum spanning tree of the graph.
