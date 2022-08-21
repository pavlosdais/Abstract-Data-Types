[Priority queue](https://en.wikipedia.org/wiki/Priority_queue) is an abstract data-type similar to a regular queue or stack data structure in which each element additionally has a "priority" associated with it. In a priority queue, an element with high priority is removed before an element with low priority.

# Performance
If n is the number of elements in the priority queue:

Algorithm  | Average case | Worst case
---------- | -------      | ----------
Space	     | O(n)	        | O(n)
Insert	   | O(log n)	    | O(n)
Remove	   | O(log n)	    | O(log n)
