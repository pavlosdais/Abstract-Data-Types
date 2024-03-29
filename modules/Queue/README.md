[Queue](https://en.wikipedia.org/wiki/Queue_(abstract_data_type)) is a linear data structure which follows a particular order in which the operations are performed. The order is First In First Out (FIFO).

The 2 basic operations performed are:
* Enqueue: Adds an item at the end of the queue.
* Dequeue: Removes an item from the start of the queue.
 
# Performance
<img align="right" width=360 alt="queue picture" src="https://upload.wikimedia.org/wikipedia/commons/thumb/5/52/Data_Queue.svg/1200px-Data_Queue.svg.png">
If n is the number of elements in the queue:

Algorithm     | Average case  | Worst case
----------    | -------       | ----------
Space	      | Θ(n)	      | O(n)
Enqueue	      | Θ(1)	      | O(1)
Sorted Insert | Θ(n)          | O(n)
Dequeue	      | Θ(1)	      | O(1)
