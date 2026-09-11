#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <iomanip>
using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    long long w;
};

int V;
vector<vector<Edge>> adj;
vector<vector<long long>> D;

int findEdge(int u, int v) {
    for (int i = 0; i < (int)adj[u].size(); i++)
        if (adj[u][i].to == v) return i;
    return -1;
}

vector<long long> dijkstra(int src) {
    vector<long long> dist(V, INF);
    priority_queue<
        pair<long long, int>,
        vector<pair<long long, int>>,
        greater<pair<long long, int>>
    > pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d != dist[u]) continue;
        for (auto e : adj[u]) {
            if (dist[u] + e.w < dist[e.to]) {
                dist[e.to] = dist[u] + e.w;
                pq.push({dist[e.to], e.to});
            }
        }
    }
    return dist;
}

void initializeAPSP() {
    D.assign(V, vector<long long>(V, INF));
    for (int i = 0; i < V; i++) D[i][i] = 0;
    for (int u = 0; u < V; u++)
        for (auto e : adj[u])
            D[u][e.to] = min(D[u][e.to], e.w);
    for (int k = 0; k < V; k++)
        for (int i = 0; i < V; i++) {
            if (D[i][k] == INF) continue;
            for (int j = 0; j < V; j++) {
                if (D[k][j] == INF) continue;
                if (D[i][k] + D[k][j] < D[i][j])
                    D[i][j] = D[i][k] + D[k][j];
            }
        }
}

void insertEdge(int u, int v, long long w) {
    int pos = findEdge(u, v);
    if (pos != -1) {
        cout << "Edge already exists.\n";
        if (w < adj[u][pos].w) {
            cout << "New weight is smaller. Treating as weight decrease.\n";
            adj[u][pos].w = w;
            for (int i = 0; i < V; i++) {
                if (D[i][u] == INF) continue;
                for (int j = 0; j < V; j++) {
                    if (D[v][j] == INF) continue;
                    long long nd = D[i][u] + w + D[v][j];
                    if (nd < D[i][j]) D[i][j] = nd;
                }
            }
        }
        return;
    }
    adj[u].push_back({v, w});
    for (int i = 0; i < V; i++) {
        if (D[i][u] == INF) continue;
        for (int j = 0; j < V; j++) {
            if (D[v][j] == INF) continue;
            long long nd = D[i][u] + w + D[v][j];
            if (nd < D[i][j]) D[i][j] = nd;
        }
    }
    cout << "Edge inserted successfully.\n";
}

vector<int> findAffectedSources(int u, int v, long long oldW) {
    vector<int> affected;
    for (int s = 0; s < V; s++) {
        if (D[s][u] == INF) continue;
        if (D[s][u] + oldW == D[s][v]) affected.push_back(s);
    }
    return affected;
}

void deleteEdge(int u, int v) {
    int pos = findEdge(u, v);
    if (pos == -1) { cout << "Edge does not exist.\n"; return; }
    long long oldW = adj[u][pos].w;
    vector<int> affected = findAffectedSources(u, v, oldW);
    adj[u].erase(adj[u].begin() + pos);
    for (int s : affected) D[s] = dijkstra(s);
    cout << "Edge deleted successfully.\n";
    cout << "Affected sources recomputed: " << affected.size() << "\n";
}

void increaseWeight(int u, int v, long long newW) {
    int pos = findEdge(u, v);
    if (pos == -1) { cout << "Edge does not exist.\n"; return; }
    long long oldW = adj[u][pos].w;
    if (newW <= oldW) {
        cout << "New weight is not greater than old weight.\n";
        if (newW < oldW) {
            cout << "Treating as weight decrease.\n";
            adj[u][pos].w = newW;
            for (int i = 0; i < V; i++) {
                if (D[i][u] == INF) continue;
                for (int j = 0; j < V; j++) {
                    if (D[v][j] == INF) continue;
                    long long nd = D[i][u] + newW + D[v][j];
                    if (nd < D[i][j]) D[i][j] = nd;
                }
            }
        }
        return;
    }
    vector<int> affected = findAffectedSources(u, v, oldW);
    adj[u][pos].w = newW;
    for (int s : affected) D[s] = dijkstra(s);
    cout << "Edge weight increased successfully.\n";
    cout << "Affected sources recomputed: " << affected.size() << "\n";
}

void decreaseWeight(int u, int v, long long newW) {
    int pos = findEdge(u, v);
    if (pos == -1) { cout << "Edge does not exist.\n"; return; }
    long long oldW = adj[u][pos].w;
    if (newW >= oldW) {
        cout << "New weight is not smaller than old weight.\n";
        if (newW > oldW) increaseWeight(u, v, newW);
        return;
    }
    adj[u][pos].w = newW;
    for (int i = 0; i < V; i++) {
        if (D[i][u] == INF) continue;
        for (int j = 0; j < V; j++) {
            if (D[v][j] == INF) continue;
            long long nd = D[i][u] + newW + D[v][j];
            if (nd < D[i][j]) D[i][j] = nd;
        }
    }
    cout << "Edge weight decreased successfully.\n";
}

void printGraph() {
    cout << "\nCurrent Graph:\n";
    for (int u = 0; u < V; u++) {
        cout << u << " -> ";
        for (auto e : adj[u]) cout << "(" << e.to << "," << e.w << ") ";
        cout << "\n";
    }
}

void printMatrix() {
    cout << "\nAPSP Distance Matrix:\n\n";
    cout << setw(6) << "";
    for (int j = 0; j < V; j++) cout << setw(8) << j;
    cout << "\n";
    for (int i = 0; i < V; i++) {
        cout << setw(6) << i;
        for (int j = 0; j < V; j++) {
            if (D[i][j] == INF) cout << setw(8) << "INF";
            else cout << setw(8) << D[i][j];
        }
        cout << "\n";
    }
}

void queryDistance() {
    int u, v;
    cout << "Enter source: " << flush;
    cin >> u;
    cout << "Enter destination: " << flush;
    cin >> v;
    if (u < 0 || u >= V || v < 0 || v >= V) {
        cout << "Invalid vertex.\n"; return;
    }
    if (D[u][v] == INF) cout << "No path exists.\n";
    else cout << "Shortest distance from " << u
              << " to " << v << " = " << D[u][v] << "\n";
}

int main() {
    cout << "===== Dynamic APSP (auto-expanding) =====\n\n";

    int E;
    cout << "Enter number of edges: " << flush;
    cin >> E;

    cout << "\nEnter edges in format:\n";
    cout << "u v weight   (any vertex >= 0, weight >= 0)\n\n";

    // ---- First pass: read all edges into a temp list ----
    struct Tmp { int u, v; long long w; };
    vector<Tmp> tmp;
    int maxVertex = -1;

    for (int i = 0; i < E; i++) {
        int u, v;
        long long w;
        cout << "Edge " << (i + 1) << ": " << flush;
        cin >> u >> v >> w;

        if (u < 0 || v < 0 || w < 0) {
            cout << "Vertices and weight must be >= 0. Try again.\n";
            i--;
            continue;
        }
        tmp.push_back({u, v, w});
        maxVertex = max(maxVertex, max(u, v));
    }

    // ---- Auto-size the graph ----
    V = (maxVertex < 0) ? 1 : maxVertex + 1;
    cout << "\nDetected " << V << " vertices (0.." << V - 1 << ").\n";

    adj.assign(V, {});

    for (auto &t : tmp) {
        int pos = findEdge(t.u, t.v);
        if (pos == -1) adj[t.u].push_back({t.v, t.w});
        else adj[t.u][pos].w = min(adj[t.u][pos].w, t.w);
    }

    initializeAPSP();

    cout << "\nInitial APSP calculated using Floyd-Warshall.\n";
    printGraph();
    printMatrix();

    while (true) {
        cout << "\n====================================\n";
        cout << "          Dynamic APSP Menu\n";
        cout << "====================================\n";
        cout << "1. Add Edge\n";
        cout << "2. Delete Edge\n";
        cout << "3. Increase Edge Weight\n";
        cout << "4. Decrease Edge Weight\n";
        cout << "5. Print Graph\n";
        cout << "6. Print APSP Matrix\n";
        cout << "7. Query Distance\n";
        cout << "8. Exit\n";
        cout << "\nEnter choice: " << flush;

        int choice;
        cin >> choice;

        if (choice == 1) {
            int u, v; long long w;
            cout << "Enter u v weight: " << flush;
            cin >> u >> v >> w;
            if (u < 0 || u >= V || v < 0 || v >= V || w < 0) {
                cout << "Invalid input (vertices 0.." << V - 1
                     << ", weight >= 0).\n";
                continue;
            }
            insertEdge(u, v, w);
            printMatrix();
        }
        else if (choice == 2) {
            int u, v;
            cout << "Enter u v: " << flush;
            cin >> u >> v;
            if (u < 0 || u >= V || v < 0 || v >= V) {
                cout << "Invalid vertices.\n"; continue;
            }
            deleteEdge(u, v);
            printMatrix();
        }
        else if (choice == 3) {
            int u, v; long long w;
            cout << "Enter u v newWeight: " << flush;
            cin >> u >> v >> w;
            if (u < 0 || u >= V || v < 0 || v >= V || w < 0) {
                cout << "Invalid input.\n"; continue;
            }
            increaseWeight(u, v, w);
            printMatrix();
        }
        else if (choice == 4) {
            int u, v; long long w;
            cout << "Enter u v newWeight: " << flush;
            cin >> u >> v >> w;
            if (u < 0 || u >= V || v < 0 || v >= V || w < 0) {
                cout << "Invalid input.\n"; continue;
            }
            decreaseWeight(u, v, w);
            printMatrix();
        }
        else if (choice == 5) printGraph();
        else if (choice == 6) printMatrix();
        else if (choice == 7) queryDistance();
        else if (choice == 8) { cout << "\nProgram terminated.\n"; break; }
        else cout << "Invalid choice.\n";
    }
    return 0;
}
