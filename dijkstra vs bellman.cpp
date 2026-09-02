#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct Edge
{
    int u, v, w;
};

// Generate a random directed weighted graph
void generateGraph(
    int n,
    int m,
    vector<vector<pair<int,int> > >& adj,
    vector<Edge>& edges)
{
    srand(time(0));

    set<pair<int,int> > used;

    while ((int)edges.size() < m)
    {
        int u = rand() % n;
        int v = rand() % n;

        if (u == v)
            continue;

        if (used.count(make_pair(u, v)))
            continue;

        used.insert(make_pair(u, v));

        int w = rand() % 20 + 1;

        // Adjacency list
        adj[u].push_back(make_pair(v, w));

        // Edge list for Bellman-Ford
        Edge newEdge;
        newEdge.u = u;
        newEdge.v = v;
        newEdge.w = w;
        edges.push_back(newEdge);
    }
}

// Save adjacency list to file
void saveGraph(
    int n,
    vector<vector<pair<int,int> > >& adj)
{
    ofstream file("graph.txt");

    file << n << endl;

    for (int u = 0; u < n; u++)
    {
        file << u + 1 << " "
             << adj[u].size();

        for (size_t i = 0; i < adj[u].size(); i++)
        {
            int v = adj[u][i].first;
            int w = adj[u][i].second;
            
            file << " "
                 << v + 1
                 << " "
                 << w;
        }

        file << endl;
    }

    file.close();
}

// Read adjacency list from file
void readGraph(
    int& n,
    vector<vector<pair<int,int> > >& adj,
    vector<Edge>& edges)
{
    ifstream file("graph.txt");

    file >> n;

    adj.resize(n);

    for (int u = 0; u < n; u++)
    {
        int vertex, degree;

        file >> vertex >> degree;

        for (int i = 0; i < degree; i++)
        {
            int v, w;

            file >> v >> w;

            v--;

            adj[u].push_back(make_pair(v, w));

            Edge newEdge;
            newEdge.u = u;
            newEdge.v = v;
            newEdge.w = w;
            edges.push_back(newEdge);
        }
    }

    file.close();
}

// Dijkstra using adjacency list
int dijkstra(
    int n,
    vector<vector<pair<int,int> > >& adj,
    int source,
    int destination)
{
    vector<int> dist(n, INF);

    priority_queue<
        pair<int,int>,
        vector<pair<int,int> >,
        greater<pair<int,int> >
    > pq;

    dist[source] = 0;

    pq.push(make_pair(0, source));

    while (!pq.empty())
    {
        pair<int,int> top = pq.top();
        int d = top.first;
        int u = top.second;

        pq.pop();

        if (d != dist[u])
            continue;

        for (size_t i = 0; i < adj[u].size(); i++)
        {
            int v = adj[u][i].first;
            int w = adj[u][i].second;
            
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    return dist[destination];
}

// Bellman-Ford using edge list
int bellmanFord(
    int n,
    vector<Edge>& edges,
    int source,
    int destination)
{
    vector<int> dist(n, INF);

    dist[source] = 0;

    for (int i = 1; i <= n - 1; i++)
    {
        bool changed = false;

        for (size_t e = 0; e < edges.size(); e++)
        {
            Edge edge = edges[e];
            
            if (dist[edge.u] != INF &&
                dist[edge.u] + edge.w < dist[edge.v])
            {
                dist[edge.v] = dist[edge.u] + edge.w;
                changed = true;
            }
        }

        if (!changed)
            break;
    }

    return dist[destination];
}

int main()
{
    int n, m;

    cout << "Enter number of vertices: ";
    cin >> n;

    cout << "Enter number of edges: ";
    cin >> m;

    // =========================================
    // STEP 1: Generate ONE graph
    // =========================================

    vector<vector<pair<int,int> > > adj(n);

    vector<Edge> edges;

    generateGraph(
        n,
        m,
        adj,
        edges
    );

    // =========================================
    // STEP 2: Save adjacency list
    // =========================================

    saveGraph(n, adj);

    cout << "\nGraph saved to graph.txt\n";

    // =========================================
    // STEP 3: Read SAME graph from file
    // =========================================

    int fileN;

    vector<vector<pair<int,int> > > fileAdj;

    vector<Edge> fileEdges;

    readGraph(
        fileN,
        fileAdj,
        fileEdges
    );

    cout << "Graph read from graph.txt\n";

    // =========================================
    // Display adjacency list
    // =========================================

    cout << "\nAdjacency List:\n";

    for (int u = 0; u < fileN; u++)
    {
        cout << u + 1 << " -> ";

        for (size_t i = 0; i < fileAdj[u].size(); i++)
        {
            int v = fileAdj[u][i].first;
            int w = fileAdj[u][i].second;
            
            cout << "("
                 << v + 1
                 << ","
                 << w
                 << ") ";
        }

        cout << endl;
    }

    // =========================================
    // STEP 4: Input u and v
    // =========================================

    int u, v;

    cout << "\nEnter u and v: ";
    cin >> u >> v;

    u--;
    v--;

    // =========================================
    // STEP 5: Dijkstra
    // =========================================

    chrono::high_resolution_clock::time_point startD =
        chrono::high_resolution_clock::now();

    int dijkstraUV =
        dijkstra(
            fileN,
            fileAdj,
            u,
            v
        );

    int dijkstraVU =
        dijkstra(
            fileN,
            fileAdj,
            v,
            u
        );

    chrono::high_resolution_clock::time_point endD =
        chrono::high_resolution_clock::now();

    // =========================================
    // STEP 6: Bellman-Ford
    // =========================================

    chrono::high_resolution_clock::time_point startB =
        chrono::high_resolution_clock::now();

    int bellmanUV =
        bellmanFord(
            fileN,
            fileEdges,
            u,
            v
        );

    int bellmanVU =
        bellmanFord(
            fileN,
            fileEdges,
            v,
            u
        );

    chrono::high_resolution_clock::time_point endB =
        chrono::high_resolution_clock::now();

    // =========================================
    // Execution time
    // =========================================

    double dijkstraTime =
        chrono::duration<double>(
            endD - startD
        ).count();

    double bellmanTime =
        chrono::duration<double>(
            endB - startB
        ).count();

    // =========================================
    // RESULT
    // =========================================

    cout << "\n================================";
    cout << "\nRESULT";
    cout << "\n================================\n";

    cout << "\nDijkstra d("
         << u + 1 << ","
         << v + 1 << ") = "
         << dijkstraUV;

    cout << "\nBellman-Ford d("
         << u + 1 << ","
         << v + 1 << ") = "
         << bellmanUV;

    cout << "\n";

    cout << "\nDijkstra d("
         << v + 1 << ","
         << u + 1 << ") = "
         << dijkstraVU;

    cout << "\nBellman-Ford d("
         << v + 1 << ","
         << u + 1 << ") = "
         << bellmanVU;

    cout << "\n\n================================";
    cout << "\nCOMPARISON";
    cout << "\n================================\n";

    if (dijkstraUV == bellmanUV)
        cout << "d(u,v): MATCH\n";
    else
        cout << "d(u,v): NOT MATCH\n";

    if (dijkstraVU == bellmanVU)
        cout << "d(v,u): MATCH\n";
    else
        cout << "d(v,u): NOT MATCH\n";

    cout << "\n================================";
    cout << "\nTIME";
    cout << "\n================================\n";

    cout << "Dijkstra      : "
         << dijkstraTime
         << " seconds\n";

    cout << "Bellman-Ford  : "
         << bellmanTime
         << " seconds\n";

    return 0;
}