/*
Lowest Common Ancestor Problem
Option #1: Naïve Algorithm
    Time Complexity: O(h) with h meaning the max distance of root to from a leaf.
    Steps:
	    1. From u, go all the way up to the root and record all vertices along the way.
	    2. From v, same thing go up to root, but stop if we encounter a common vertex for the first time.

https://www.hackerrank.com/topics/lowest-common-ancestor
*/


int parent[MAX_NODES]; // Keep track of parent per node in tree.
bool visited[MAX_NODES];
// The tree is stored as a undirected graph using adjacency list
vector<vector<int>> tree[MAX_NODES];


/* Traverse the tree and computes the parent array such that,
 * The pre-order traveral begins by calling GetParents(root, -1). */
void GetParents (int node, int par) {
    for (int i = 0; i < tree[node].size(); ++i) {
        // Pre-order traveral.
        if (tree[node][i] != par) {
            parent[tree[node][i]] = node;
            GetParents(tree[node][i], node)
        }
    }
}


/* Computes the LCA of nodes u and v. */
int LCA (int u, int v) {
    // Traverse from node u up until root, and mark all the encountered
    // vertices along the path.
    int lca;

    // Go through u's parents until root. Mark nodes as visited in the mean time.
    while(1) {
        visited[u] = true;
        // Keep going up from u's parents to root node. Once root is found,
        // get out of the while loop.
        if (u == root_node) {
            break;
        }
        u = parent[u]
    }

    // Now go up through v's parents to root. Once we find a node in v's route
    // that's already visited, that means u has visited the same node as well.
    // Hence then we now found the lowest common ancestor node!
    while(1) {
        if (visited[v]) {
            lca = v;
            break;
        }
        v = parent[v];
    }

    // Return the lowest common ancestor node as result.
    return lca;
}



















/////////////////////////////////////////////////////////////////////////////
