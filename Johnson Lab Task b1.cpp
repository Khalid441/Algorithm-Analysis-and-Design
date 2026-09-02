// johnson.cpp
// Generates a random undirected, non-negative weighted graph with V vertices
// and E edges, then computes All-Pairs Shortest Paths using Johnson's Algorithm.
//
// Output:
//   graph_edges.txt      -> generated graph: u,v,w
//   shortest_paths.txt   -> all-pairs shortest distances: u,v,d
//
// Compile:
//   g++ -O2 -std=c++11 johnson.cpp -o johnson
//
// Run:
//   ./johnson

#include <bits/stdc++.h>
using namespace std;

const int V = 33;
const int E = 333;
const long long INF = LLONG_MAX / 4;

struct Edge {
    int u, v;
    long long w;
};

int main() {

    // =========================================================
    // 1. Generate random undirected, non-negative weighted graph
    // =========================================================

    srand(time(0));

    vector<Edge> edges;
    set<pair<int, int> > usedPairs;

    while ((int)edges.size() < E) {

        int u = rand() % V;
        int v = rand() % V;

        // No self-loop
        if (u == v)
            continue;

        // Normalize the pair
        if (u > v)
            swap(u, v);

        // No duplicate edge
        if (usedPairs.count(make_pair(u, v)))
            continue;

        usedPairs.insert(make_pair(u, v));

        // Weight from 1 to 100
        long long w = rand() % 100 + 1;

        edges.push_back({u, v, w});
    }

    // =========================================================
    // 2. Build adjacency list
    // =========================================================

    vector<vector<pair<int, long long> > > adj(V);

    for (size_t i = 0; i < edges.size(); i++) {

        Edge e = edges[i];

        // Undirected graph
        adj[e.u].push_back(make_pair(e.v, e.w));
        adj[e.v].push_back(make_pair(e.u, e.w));
    }

    // =========================================================
    // 3. Save generated graph
    // =========================================================

    {
        ofstream fout("graph_edges.txt");

        for (size_t i = 0; i < edges.size(); i++) {
            fout << edges[i].u << ","
                 << edges[i].v << ","
                 << edges[i].w << "\n";
        }

        fout.close();
    }

    // =========================================================
    // 4. Johnson's Algorithm
    // =========================================================

    // Add a new vertex q
    int Vq = V + 1;
    int q = V;

    // Convert undirected edges into two directed edges
    vector<Edge> directedEdges;

    directedEdges.reserve(edges.size() * 2 + V);

    for (size_t i = 0; i < edges.size(); i++) {

        Edge e = edges[i];

        directedEdges.push_back({e.u, e.v, e.w});
        directedEdges.push_back({e.v, e.u, e.w});
    }

    // q -> every vertex with weight 0
    for (int v = 0; v < V; v++) {
        directedEdges.push_back({q, v, 0});
    }

    // =========================================================
    // 5. Bellman-Ford
    // =========================================================

    vector<long long> h(Vq, INF);

    h[q] = 0;

    for (int i = 0; i < Vq - 1; i++) {

        bool relaxed = false;

        for (size_t j = 0; j < directedEdges.size(); j++) {

            Edge e = directedEdges[j];

            if (h[e.u] != INF &&
                h[e.u] + e.w < h[e.v]) {

                h[e.v] = h[e.u] + e.w;
                relaxed = true;
            }
        }

        if (!relaxed)
            break;
    }

    // =========================================================
    // 6. Check for negative cycle
    // =========================================================

    for (size_t i = 0; i < directedEdges.size(); i++) {

        Edge e = directedEdges[i];

        if (h[e.u] != INF &&
            h[e.u] + e.w < h[e.v]) {

            cerr << "Negative-weight cycle detected!\n";
            return 1;
        }
    }

    // =========================================================
    // 7. Reweight edges
    // =========================================================

    vector<vector<pair<int, long long> > > radj(V);

    for (size_t i = 0; i < edges.size(); i++) {

        Edge e = edges[i];

        // u -> v
        long long w1 =
            e.w + h[e.u] - h[e.v];

        // v -> u
        long long w2 =
            e.w + h[e.v] - h[e.u];

        radj[e.u].push_back(
            make_pair(e.v, w1)
        );

        radj[e.v].push_back(
            make_pair(e.u, w2)
        );
    }

    // =========================================================
    // 8. Dijkstra from every vertex
    // =========================================================

    vector<vector<long long> > dist(
        V,
        vector<long long>(V, INF)
    );

    for (int src = 0; src < V; src++) {

        vector<long long> d(
            V,
            INF
        );

        d[src] = 0;

        priority_queue<
            pair<long long, int>,
            vector<pair<long long, int> >,
            greater<pair<long long, int> >
        > pq;

        pq.push(make_pair(0, src));

        vector<bool> visited(V, false);

        while (!pq.empty()) {

            // C++11 replacement for:
            // auto [du, u] = pq.top();

            pair<long long, int> current = pq.top();
            pq.pop();

            long long du = current.first;
            int u = current.second;

            if (visited[u])
                continue;

            visited[u] = true;

            // C++11 replacement for:
            // for (auto &[v, w] : radj[u])

            for (size_t i = 0; i < radj[u].size(); i++) {

                int v = radj[u][i].first;
                long long w = radj[u][i].second;

                if (d[u] + w < d[v]) {

                    d[v] = d[u] + w;

                    pq.push(
                        make_pair(d[v], v)
                    );
                }
            }
        }

        // =====================================================
        // 9. Convert reweighted distances to original distances
        // =====================================================

        for (int v = 0; v < V; v++) {

            if (d[v] != INF) {

                dist[src][v] =
                    d[v] - h[src] + h[v];

            } else {

                dist[src][v] = INF;
            }
        }
    }

    // =========================================================
    // 10. Write all-pairs shortest paths
    // =========================================================

    {
        ofstream fout("shortest_paths.txt");

        for (int u = 0; u < V; u++) {

            for (int v = 0; v < V; v++) {

                // Skip u -> u
                if (u == v)
                    continue;

                if (dist[u][v] >= INF) {

                    fout << u << ","
                         << v << ",INF\n";

                } else {

                    fout << u << ","
                         << v << ","
                         << dist[u][v]
                         << "\n";
                }
            }
        }

        fout.close();
    }

    // =========================================================
    // 11. Final message
    // =========================================================

    cerr << "Done.\n";
    cerr << "Generated graph : graph_edges.txt ("
         << edges.size() << " edges)\n";

    cerr << "Shortest paths  : shortest_paths.txt\n";

    return 0;
}