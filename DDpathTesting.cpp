// Write a Program to input graph matrix and perform DD path testing.

#include <iostream>
#include <vector>
#include <stack>

using namespace std;


void displayGraph(const vector<vector<int>> &graph) {
    cout << "Graph Matrix:" << endl;
    for (const auto &row : graph) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}


void findPaths(const vector<vector<int>> &graph, int start, int end, vector<bool> &visited, vector<int> &path, vector<vector<int>> &allPaths) {
    visited[start] = true;
    path.push_back(start);

    if (start == end) {
        allPaths.push_back(path);
    } else {
        for (int i = 0; i < graph.size(); i++) {
            if (graph[start][i] == 1 && !visited[i]) {
                findPaths(graph, i, end, visited, path, allPaths);
            }
        }
    }

    path.pop_back();
    visited[start] = false;
}


void ddPathTesting(const vector<vector<int>> &graph) {
    int n = graph.size();
    vector<vector<int>> allPaths;

    // Find all paths between decision nodes
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph[i][j] == 1) {
                vector<bool> visited(n, false);
                vector<int> path;
                findPaths(graph, i, j, visited, path, allPaths);
            }
        }
    }


    cout << "DD Paths:" << endl;
    for (const auto &path : allPaths) {
        for (int node : path) {
            cout << node << " ";
        }
        cout << endl;
    }
}

int main() {
    int n;
    cout << "Enter the number of nodes in the graph: ";
    cin >> n;

    vector<vector<int>> graph(n, vector<int>(n, 0));

    cout << "Enter the adjacency matrix of the graph:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> graph[i][j];
        }
    }

    displayGraph(graph);
    ddPathTesting(graph);

    return 0;
}
