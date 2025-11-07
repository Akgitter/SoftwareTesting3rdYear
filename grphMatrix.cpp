//
// Created by HP on 30-10-2025.
// Write a program to for generating graph matrix.

#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>

using namespace std;

// Creates an n x n adjacency matrix initialized appropriately.
// If weighted is true, matrix is initialized with a sentinel (INF) meaning "no edge",
// otherwise initialized with 0 meaning "no edge".
vector<vector<long long>> createMatrix(int n, bool weighted) {
    const long long INF = numeric_limits<long long>::max() / 4;
    vector<vector<long long>> mat(n, vector<long long>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (weighted) mat[i][j] = INF;
            else mat[i][j] = 0;
        }
    }
    return mat;
}

// Print the adjacency matrix. For weighted graphs, print "INF" for no-edge sentinel.
void printMatrix(const vector<vector<long long>>& mat, bool weighted) {
    const long long INF = numeric_limits<long long>::max() / 4;
    int n = (int)mat.size();
    // Header
    cout << "Adjacency Matrix (" << n << " x " << n << "):\n";
    // Print column indices
    cout << "    ";
    for (int j = 0; j < n; ++j) cout << setw(4) << j+1;
    cout << '\n';
    cout << "    ";
    for (int j = 0; j < n; ++j) cout << "____";
    cout << '\n';

    for (int i = 0; i < n; ++i) {
        cout << setw(3) << i+1 << "|";
        for (int j = 0; j < n; ++j) {
            if (weighted) {
                if (mat[i][j] == INF) cout << setw(4) << "INF";
                else cout << setw(4) << mat[i][j];
            } else {
                cout << setw(4) << mat[i][j];
            }
        }
        cout << '\n';
    }
}

int main() {
    cout << "Graph Adjacency Matrix Generator\n";
    cout << "--------------------------------\n";
    int n;
    cout << "Enter number of vertices (n): ";
    if (!(cin >> n) || n <= 0) {
        cerr << "Invalid number of vertices. Exiting.\n";
        return 1;
    }

    bool weighted = false;
    char wchoice;
    cout << "Is the graph weighted? (y/n): ";
    cin >> wchoice;
    if (wchoice == 'y' || wchoice == 'Y') weighted = true;

    bool directed = false;
    char dchoice;
    cout << "Is the graph directed? (y/n): ";
    cin >> dchoice;
    if (dchoice == 'y' || dchoice == 'Y') directed = true;

    int m;
    cout << "Enter number of edges: ";
    if (!(cin >> m) || m < 0) {
        cerr << "Invalid number of edges. Exiting.\n";
        return 1;
    }

    auto mat = createMatrix(n, weighted);
    const long long INF = numeric_limits<long long>::max() / 4;

    cout << "Enter each edge in the format:\n";
    if (weighted) cout << "  u v w   (u and v are vertex indices 1..n, w is signed integer weight)\n";
    else cout << "  u v     (u and v are vertex indices 1..n)\n";
    cout << "For undirected graphs each edge will update both [u][v] and [v][u].\n";

    for (int e = 0; e < m; ++e) {
        int u, v;
        long long w = 1; // default for unweighted
        if (weighted) {
            if (!(cin >> u >> v >> w)) {
                cerr << "Invalid edge input. Exiting.\n";
                return 1;
            }
        } else {
            if (!(cin >> u >> v)) {
                cerr << "Invalid edge input. Exiting.\n";
                return 1;
            }
        }
        if (u < 1 || u > n || v < 1 || v > n) {
            cerr << "Edge vertices out of range: " << u << ", " << v << ". Skipping this edge.\n";
            continue;
        }
        int ui = u - 1;
        int vi = v - 1;
        if (weighted) {
            mat[ui][vi] = w;
            if (!directed) mat[vi][ui] = w;
        } else {
            // For unweighted graphs mark presence as 1. If multiple edges appear, keep 1.
            mat[ui][vi] = 1;
            if (!directed) mat[vi][ui] = 1;
        }
    }

    printMatrix(mat, weighted);

    return 0;
}
