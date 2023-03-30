A [vector](https://en.wikipedia.org/wiki/Dynamic_array) (or dynamic array) is a data structure that allows elements to be added or removed. Vectors overcome the limit of static arrays, which have a fixed capacity that needs to be specified at allocation. They overcome this limitation since they can grow in size, in this implementation by doubling in size.

# Performance
<img align="right" width=330 alt="vector picture" src="https://www.interviewcake.com/images/svgs/dynamic_arrays__capacity_size_end_index.svg?bust=210">

If n is the number of elements in the vector:

Algorithm     | Average case  | Worst case
----------    | -------       | ----------
Space	        | Θ(n)	        | O(n)
Push Back     | Θ(1)	        | O(n)
Clear/set at  | Θ(1)	        | O(1)
Binary Search |	Θ(logn)       | O(logn)
Search        | Θ(n)          | O(n)
Sort          | Θ(logn n)     | O(n<sup>2</sup>)