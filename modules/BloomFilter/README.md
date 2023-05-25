A [Bloom filter](https://en.wikipedia.org/wiki/Bloom_filter) is a data structure that relies on hashing and operates with a degree of probability. It offers exceptional space efficiency and is commonly employed for adding elements to a set and checking if an element belongs to the set. However, the actual elements are not directly included in the set; rather, a hash of each element is added. While false positive matches can occur, false negatives are eliminated, meaning a query outcome is either "possibly in set" or "definitely not in set." As the number of items added increases, the likelihood of false positives also grows

# Performance
<img align="right" width=320 alt="bloom filter picture" src="https://bravenewgeek.com/wp-content/uploads/2015/02/bloom_filter.png">

If n is the number of elements in the bloom filter:

Algorithm  | Average case   | Worst case
---------- | -------        | ----------
Space	   | Θ(n)	        | O(n)
Insert	   | Θ(1)	        | O(1)
Search	   | Θ(1)	        | O(1)
