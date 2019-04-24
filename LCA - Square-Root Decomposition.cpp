/*
Lowest Common Ancestor Problem
Option #2: Square Root Decomposition
    Time Complexity: O(N).
    Section off the tree by the square root of the height of the tree.

    Steps:
	1. From u, go all the way up to the root and record all vertices along the way.
	2. From v, same thing go up to root, but stop if we encounter a common vertex for the first time.
*/
