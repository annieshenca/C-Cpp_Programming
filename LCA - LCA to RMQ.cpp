/*
Lowest Common Ancestor Problem
Option #4: LCA to RMQ
    Time Complexity: O(N).
    Section off the tree by the square root of the height of the tree.
    We store the parent of the current node as the value.

    Similiar to the LCA option 1 before, but now we have cases:
      1. if P[u] != P[v]
        Then move up the tree by node = P[node]. Each movement causes O(1) up.
      2. if P[u] == P[v]
        This means the LCA node is either itself/above/below the P[u].

https://www.hackerrank.com/topics/lowest-common-ancestor
*/
