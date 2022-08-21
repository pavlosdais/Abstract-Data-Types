In this implentation we use red-black trees in order to implement the buckets (instead of, for example, linked lists). This way, we combine the two data structures getting the best from both worlds. The downside of such an implementation is that in the vast majority of cases, there will be few objects in the bucket, and the insertion may cause a delay (due to extra mallocs, etc). So, as an optimization, we will keep the first FIXED_SIZE (eg the first 3) elements of each bucket in an array, and only if we have more will we insert them into the red-black tree. The biggest advantage of this implentation is that, even in the worst case scenario, we maintain logarithmic complexity on all operations.

## Performance
If n is the number of elements in the hash table:

Algorithm  | Best case    | Worst case
---------- | -------      | ----------
Space	   | O(n)	      | O(n)
Insert	   | O(1)	      | O(log n)
Remove	   | O(1)	      | O(log n)
Search	   | O(1)	      | O(log n)