[Priority queue](https://en.wikipedia.org/wiki/Priority_queue) is an abstract data-type similar to a regular queue or stack data structure in which each element additionally has a "priority" associated with it. In a priority queue, an element with high priority is removed before an element with low priority. In this implementation, a binary heap is used. A binary heap is a binary tree with following properties:

1. A binary heap is a complete binary tree. This means all levels of the tree, except possibly the last one (deepest) are fully filled and if the last level of the tree is not complete, the nodes of that level are filled from left to right.
2. The key stored in each node is either greater than or equal to (≥) or less than or equal to (≤) the keys in the node's children, according to some order(dictated by the compare function).

* Check an application of this ADT [here](https://github.com/pavlosdais/n-puzzle)

# Performance
<img align="right" width=380 alt="priority queue picture" src="https://bradfieldcs.com/algos/trees/priority-queues-with-binary-heaps/figures/heap-order.png">

If n is the number of elements in the priority queue:

Algorithm  | Average case   | Worst case
---------- | -------        | ----------
Space	   | Θ(n)	        | O(n)
Insert	   | Θ(log n)	    | O(n)
Remove	   | Θ(log n)	    | O(log n)
