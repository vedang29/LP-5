#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// Add edge (undirected graph)
void addEdge(vector<vector<int>>& graph, int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

// Parallel BFS
void parallelBFS(vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);

    queue<int> q;
    q.push(start);
    visited[start] = true;

    cout << "BFS Traversal: ";

    while (!q.empty()) {
        int size = q.size();

        vector<int> currentLevel;

        // Process current level
        for (int i = 0; i < size; i++) {
            int node = q.front();
            q.pop();
            cout << node << " ";

            currentLevel.push_back(node);
        }

        // Parallel processing of neighbors
        #pragma omp parallel for
        for (int i = 0; i < currentLevel.size(); i++) {
            int node = currentLevel[i];

            for (int neighbor : graph[node]) {
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
    }
    cout << endl;
}

// Main
int main() {
    int n = 6; // number of nodes
    vector<vector<int>> graph(n);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 5);

    double start = omp_get_wtime();
    parallelBFS(graph, 0);
    double end = omp_get_wtime();

    cout << "Time taken: " << (end - start) << " seconds\n";

    return 0;
}
