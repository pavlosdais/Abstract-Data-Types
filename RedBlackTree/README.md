[Red-Black Tree](https://en.wikipedia.org/wiki/Red%E2%80%93black_tree) is a binary search tree with one extra bit of storage per node: its color, which can be either RED or BLACK. By constraining the way nodes can be colored on any path from the root to a leaf, red-black trees ensure that no such path is more than twice as long as any other, so that the tree is approximately balanced.

# Properties
1. Every node is colored either red or black.
2. The root of the tree is always black.
3. There are no two adjacent red nodes (A red node cannot have a red parent or red child).
4. Every path from a node (including root) to any of its leaf nodes has the same number of black nodes.
5. All leaf nodes are black nodes.

When the tree is modified, the new tree is rearranged and repainted to restore the coloring properties that constrain how unbalanced the tree can become in the worst case. 
The properties are designed such that this rearranging and recoloring can be performed efficiently.

# Performance
Although the balance of the tree is not perfect, it is good enough to reduce the searching time and maintain logarithmic complexity. <br/>
So, if n is the number of values in the tree:

Algorithm  | Average case | Worst case
---------- | -------      | ----------
Space	   | O(n)	      | O(n)
Insert	   | O(log n)	  | O(log n)
Remove	   | O(log n)	  | O(log n)
Search	   | O(log n)	  | O(log n)

# Learn more
For more information as well as examples click [here](http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap14.htm).
