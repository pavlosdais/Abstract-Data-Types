This is an implentation using [linear hashing](https://en.wikipedia.org/wiki/Linear_hashing). Linear hashing is a dynamic data structure which implements a hash table and grows or shrinks one bucket at a time. A Linear hashing file expands by splitting a pre-determined bucket into two. The trigger for the split in this implementation is an overflow of elements at the bucket (by default 4). In order to access a record with key, a family of hash functions, called collectively a dynamic hash function is applied to the key c. At any time, at most two hash functions h(i) and h(i+1) are used.

# Performance
<img align="right" width=450 alt="linear hashing picture" src="http://queper.in/drupal/queper/qpr_src/qpr_blogs/dbsys/linear_hashing_example_05.png">

If n is the number of elements in the hash table:

Algorithm  | Average case | Worst case
---------- | -------      | ----------
Space	   | Θ(n)	      | O(n)
Insert	   | Θ(1)	      | O(n)
Remove	   | Θ(1)	      | O(n)
Search	   | Θ(1)	      | O(n)

# Learn more
For more information as well as examples click [here](https://www.alexdelis.eu/M149/e_ds_linearhashing.pdf).
