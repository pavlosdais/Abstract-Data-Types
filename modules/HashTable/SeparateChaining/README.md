This is an implentation using [separate chaining](https://en.wikipedia.org/wiki/Hash_table#Separate_chaining). In separate chaining, each slot of the hash table is a linked list. When two or more elements are hashed to the same location (when a collision occurs), these elements are represented into a singly-linked list much like a chain. If there are n elements and b is the number of the buckets there would be n/b entries on each bucket. This value n/b is called the load factor that represents the load that is there on our map. So, theoretically, when the load factor increases so does the complexity of the operations. In order for the load factor to be kept low and remain almost constant complexity, we increase the number of buckets (approximately doubling) and rehash once the load factor increases to more than a pre-defined value (the default value here is 1.2).

## Performance
<img align="right" width=320 alt="separate chaining picture" src="https://he-s3.s3.amazonaws.com/media/uploads/0e2c706.png">

If n is the number of elements in the hash table:

Algorithm  | Average case | Worst case
---------- | -------      | ----------
Space	   | Θ(n)	      | O(n)
Insert	   | Θ(1)	      | O(n)
Remove	   | Θ(1)	      | O(n)
Search	   | Θ(1)	      | O(n)
