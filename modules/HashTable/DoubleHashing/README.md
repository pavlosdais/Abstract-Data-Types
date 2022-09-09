This is an implentation using [double hashing](https://en.wikipedia.org/wiki/Double_hashing). Double hashing is used in conjunction with [open addressing](https://en.wikipedia.org/wiki/Open_addressing) in hash tables to resolve hash collisions, by using a secondary hash of the key as an offset when a collision occurs. The double hashing technique uses the first hash value as an index into the table and then repeatedly steps forward an interval until the desired value is located, an empty location is reached, or the entire table has been searched. The interval is set by a second hash function. Here, the second function used is the popular: <br/>
`hash_func2 = PRIME_NUM – (hash_func1 % PRIME_NUM)`, where PRIME_NUM is a prime smaller than the hash table's capacity.

# Performance
<img align="right" width=330 alt="double hashing picture" src="https://upload.wikimedia.org/wikipedia/commons/thumb/7/7d/Hash_table_3_1_1_0_1_0_0_SP.svg/640px-Hash_table_3_1_1_0_1_0_0_SP.svg.png">

If n is the number of elements in the hash table:

Algorithm  | Average case | Worst case
---------- | -------      | ----------
Space	   | Θ(n)	      | O(n)
Insert	   | Θ(1)	      | O(n)
Remove	   | Θ(1)	      | O(n)
Search	   | Θ(1)	      | O(n)
