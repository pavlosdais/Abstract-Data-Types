This is an implementations of [undirected graph](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)#Undirected_graph) using adjecency list. An undirected graph is graph, i.e., a set of objects (called vertices or nodes) that are connected together, where all the edges are bidirectional. An undirected graph is sometimes called an undirected network.

# Performance
<img align="right" width=310 alt="undirected graph picture" src="https://upload.wikimedia.org/wikipedia/commons/thumb/b/bc/Undirected_graph_no_background.svg/1200px-Undirected_graph_no_background.svg.png">

If V is the number vertices and E is the number of edges in the graph:

Algorithm         | Average case | Worst case
---------------   | ---------    | ----------
Space	          | Θ(V+E)       | O(V+E)
Insert edge       | Θ(1)	     | O(1)
Print	          | Θ(V+E)	     | O(V+E)
Simple Path Check | Θ(V+E)	     | O(V+E)

The algorithm used to perform the simple path check (meaning a path where each vertex is visited at most once) between two vertices is [Breadth-first search](https://en.wikipedia.org/wiki/Breadth-first_search).
