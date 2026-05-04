#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Add edge
void addEdge(vector<vector<int>>& graph, int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

// Parallel DFS using tasks
void parallelDFSUtil(vector<vector<int>>& graph, int node, vector<bool>& visited) {

    bool doWork = false;

    #pragma omp critical
    {
        if (!visited[node]) {
            visited[node] = true;
            doWork = true;
            cout << node << " ";
        }
    }

    // SAFE exit (outside OpenMP block)
    if (!doWork)
        return;

    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            #pragma omp task
            parallelDFSUtil(graph, neighbor, visited);
        }
    }
}

// Driver
void parallelDFS(vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);

    #pragma omp parallel
    {
        #pragma omp single
        parallelDFSUtil(graph, start, visited);
    }
}

// Main
int main() {
    int n = 6;
    vector<vector<int>> graph(n);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 5);

    double start = omp_get_wtime();
    cout << "DFS Traversal: ";
    parallelDFS(graph, 0);
    double end = omp_get_wtime();

    cout << "\nTime taken: " << (end - start) << " seconds\n";

    return 0;
}
