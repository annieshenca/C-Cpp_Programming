/*
Lowest Common Ancestor Problem
Option #2: Square Root Decomposition
    Time Complexity: O(N).
    Section off the tree by the square root of the height of the tree.
    We store the parent of the current node as the value.

    Similiar to the LCA option 1 before, but now we have cases:
      1. if P[u] != P[v]
        Then move up the tree by node = P[node]. Each movement causes O(1) up.
      2. if P[u] == P[v]
        This means the LCA node is either itself/above/below the P[u].

*/


int parent[MAX_NODES], level[MAX_NODES], P[MAX_NODES];
vector<vector<int>> tree[MAX_NODES];


void traverse (int node, int head, int prev_sec) {
    int cur_sec = sqrt(level[node]) + 1; // To ignore the level 0.
    // This is the top-most level
    if (cur_sec == 1) {
        P[node] = 1;
    } else {
        if (cur_sec == prev_sec + 1) {
            P[node] = parent[node];
            head = node;
        } else {
            P[node] = parent[head];
        }
    }

    for (int i = 0; i < tree[node].size(); i++) {
        if (tree[node][i] != parent[node]) {
            // Recurse.
            traverse(tree[node][i], head, cur_sec);
        }
    }
}


void LCA (int u, int v) {
    // If u and v parents are not the same, and not in the same level, kept
    // going until so.
    while (P[u] != P[v]) {
        if (level[u] > level[v]) {
            u = P[u];
        } else {
            v = P[v];
        }
    }

    // Once u and v parents are the same now, check their levels, and move
    // upwards the tree when necessary.
    while (u != v) {
        if (level[u] != level[v]) {
            u = parent[u];
        } else {
            v = parent[v];
        }
    }
    return u; // Either u or v would return the same LCA node.
}
