#include <stdio.h>
#include <stdlib.h>

typedef struct Road {
    int u, v, c, f;
} Road;

int find(int parent[], int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent, parent[i]);
}

void union_sets(int parent[], int rank[], int x, int y) {
    int rootX = find(parent, x);
    int rootY = find(parent, y);

    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY])
            parent[rootX] = rootY;
        else if (rank[rootX] > rank[rootY])
            parent[rootY] = rootX;
        else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

int compare(const void *a, const void *b) {
    Road *r1 = (Road *)a;
    Road *r2 = (Road *)b;
    return r1->c - r2->c;
}

// Adjust the MST to get exactly k guards by swapping edges
int adjust_MST(int n, int m, int k, Road *roads, int num_guards, Road *mst_roads, int mst_size, int cost) {
    // Union-Find data structures for the adjusted MST
    int parent[n], rank[n];
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    // Iterate over the MST edges and try to replace as needed
    for (int i = 0; i < mst_size; i++) {
        // Check if we need to adjust the guard count
        if (num_guards > k && mst_roads[i].f == 0) {
            // Try to replace this guarded edge with an unguarded edge (if any)
            for (int j = 0; j < m; j++) {
                if (roads[j].f == 1 && find(parent, roads[j].u) != find(parent, roads[j].v)) {
                    // Found an unguarded edge that connects two different components
                    union_sets(parent, rank, roads[j].u, roads[j].v);
                    cost += roads[j].c - mst_roads[i].c; // Adjust cost
                    num_guards--; // Decrease guard count
                    break; // Move to the next edge
                }
            }
        } else if (num_guards < k && mst_roads[i].f == 1) {
            // Try to replace this unguarded edge with a guarded edge (if any)
            for (int j = 0; j < m; j++) {
                if (roads[j].f == 0 && find(parent, roads[j].u) != find(parent, roads[j].v)) {
                    // Found a guarded edge that connects two different components
                    union_sets(parent, rank, roads[j].u, roads[j].v);
                    cost += roads[j].c - mst_roads[i].c;
                    num_guards++;
                    break;
                }
            }
        }
        if (num_guards == k) {
            return cost;
        }
    }
    return -1;
}

// Modified Kruskal's algorithm to find MST with exactly k guards
int kruskal_MST(int n, int m, int k, Road *roads) {
    // Sort roads from least to greatest cost
    qsort(roads, m, sizeof(Road), compare);

    // Union-Find data structures
    int parent[n], rank[n];
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    int cost = 0;
    int guards_count = 0;
    int roads_count = 0;
    Road mst_roads[n - 1];

    // Iterate through sorted edges and build the MST
    for (int i = 0; i < m; i++) {
        // Check if adding road (u, v) would form a cycle
        if (find(parent, roads[i].u) != find(parent, roads[i].v)) {
            union_sets(parent, rank, roads[i].u, roads[i].v);
            mst_roads[roads_count++] = roads[i];
            cost += roads[i].c;

            if (roads[i].f == 0) {
                guards_count++;
            }

            // If we have formed a spanning tree with exactly n - 1 edges
            if (roads_count == n - 1) {
                // Adjust MST if guard count is not exactly k
                if (guards_count != k) {
                    return adjust_MST(n, m, k, roads, guards_count, mst_roads, roads_count, cost);
                }
                return cost;
            }
        }
    }

    // If no valid MST with exactly k guards was found
    return -1;
}

int fast_read() {
    int x = 0, w = 1;
    char ch = 0;
    while (ch < '0' || ch > '9') {  
        if (ch == '-') w = -1;     
        ch = getchar();           
    }
    while (ch >= '0' && ch <= '9') { 
        x = x * 10 + (ch - '0'); 
        ch = getchar();  
    }
    return x * w; 
}

int main() {
    int n = fast_read();
    int m = fast_read();
    int k = fast_read();

    // Array to store m number of roads
    Road roads[m];

    // Input the roads
    for (int i = 0; i < m; i++) {
        roads[i].u = fast_read();
        roads[i].v = fast_read();
        roads[i].c = fast_read();
        roads[i].f = fast_read();
    }

    // Find the MST with exactly k guards
    int result = kruskal_MST(n, m, k, roads);
    printf("%d\n", result);

    return 0;
}
