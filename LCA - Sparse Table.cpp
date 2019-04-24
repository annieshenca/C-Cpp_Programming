/*
Lowest Common Ancestor Problem
Option #3: Sparse Table
    Time Complexity: O(nlogn).
    Steps:
	    1. From u, go all the way up to the root and record all vertices along the way.
	    2. From v, same thing go up to root, but stop if we encounter a common vertex for the first time.

https://www.hackerrank.com/topics/lowest-common-ancestor
*/
