//
// Created by HP on 23-10-2025.
//
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

// Represents a node (statement or decision) in the control flow graph
struct Node {
    int id;
    string label;
    vector<int> next; // adjacency list
    bool is_decision;
};

// Helper to print adjacency list
void print_graph(const vector<Node>& nodes, const string& title) {
    cout << "\n" << title << ":\n";
    for (const Node& node : nodes) {
        cout << node.label << " (id=" << node.id << ") -> ";
        for (int n : node.next) {
            cout << nodes[n].label << " (id=" << n << "), ";
        }
        cout << endl;
    }
}

// Helper to find next decision/junction from a node (for DD path graph)
int find_next_dd(const vector<Node>& nodes, int start, set<int>& dd_nodes) {
    // BFS to next DD node
    queue<int> q;
    set<int> visited;
    q.push(start);
    visited.insert(start);
    while (!q.empty()) {
        int curr = q.front(); q.pop();
        if (curr != start && dd_nodes.count(curr)) return curr;
        for (int n : nodes[curr].next) {
            if (!visited.count(n)) {
                visited.insert(n);
                q.push(n);
            }
        }
    }
    return -1; // none found
}

// Build DD path graph: nodes are decision/junctions, edges are paths
vector<Node> build_dd_path_graph(const vector<Node>& cfg) {
    vector<Node> dd_nodes;
    set<int> dd_indices;
    // A node is a DD node if it's a decision (branch) or has >1 incoming or >1 outgoing edge
    map<int,int> indegree;
    for (const Node& n : cfg) for (int nxt : n.next) indegree[nxt]++;
    for (const Node& n : cfg) {
        if (n.is_decision || n.next.size() > 1 || indegree[n.id] > 1)
            dd_indices.insert(n.id);
    }
    // Map old id to new dd_node index
    map<int,int> id_to_dd;
    int k = 0;
    for (int idx : dd_indices) {
        dd_nodes.push_back(cfg[idx]);
        id_to_dd[idx] = k++;
    }
    // Add edges between DD nodes
    for (int i = 0; i < dd_nodes.size(); ++i) {
        int from = dd_nodes[i].id;
        for (int n : cfg[from].next) {
            int to_dd = find_next_dd(cfg, n, dd_indices);
            if (to_dd != -1)
                dd_nodes[i].next.push_back(id_to_dd[to_dd]);
        }
    }
    // Relabel ids as DD indices
    for (int i = 0; i < dd_nodes.size(); ++i) dd_nodes[i].id = i;
    return dd_nodes;
}

int main() {
    cout << "Enter the number of nodes (statements/decisions) in your control flow graph: ";
    int N;
    cin >> N;
    cin.ignore();

    vector<Node> nodes(N);

    cout << "Enter each node's label (e.g., S1, D1, End), and if it's a decision (1 for yes, 0 for no):\n";
    for (int i = 0; i < N; ++i) {
        string label;
        int is_dec;
        cout << "Node " << i << " label: ";
        getline(cin, label);
        cout << "Is it a decision node? (1=yes, 0=no): ";
        cin >> is_dec;
        cin.ignore();
        nodes[i] = {i, label, {}, is_dec};
    }

    cout << "Now, for each node, enter the indices of next nodes (comma-separated), or -1 for none (end):\n";
    for (int i = 0; i < N; ++i) {
        cout << "Node " << i << " (" << nodes[i].label << ") next nodes: ";
        string line;
        getline(cin, line);
        stringstream ss(line);
        string t;
        while (getline(ss, t, ',')) {
            int x = stoi(t);
            if (x == -1) break;
            nodes[i].next.push_back(x);
        }
    }

    print_graph(nodes, "Control Flow Graph (CFG)");

    vector<Node> dd_graph = build_dd_path_graph(nodes);
    print_graph(dd_graph, "DD Path Graph (nodes are decisions/junctions)");

    cout << "\nDone.\n";
    return 0;
}