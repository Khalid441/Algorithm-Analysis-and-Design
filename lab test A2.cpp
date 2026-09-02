#include <bits/stdc++.h>
#include <fstream>
#include <ctime>
using namespace std;

const int INF = 1e9;

// Generate one complete directed weighted graph
void generateGraph(int n, vector<vector<int>>& graph)
{
    srand(time(0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = rand() % 20 + 1;
        }
    }
}

// Save the generated graph
void saveGraph(int n, vector<vector<int>>& graph)
{
    ofstream file("graph.txt");

    file << n << endl;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            file << graph[i][j] << " ";

        file << endl;
    }

    file.close();
}

// Floyd-Warshall
vector<vector<int>> floydWarshall(
    int n,
    vector<vector<int>> graph)
{
    vector<vector<int>> dist = graph;

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = min(
                        dist[i][j],
                        dist[i][k] + dist[k][j]
                    );
            }
        }
    }

    return dist;
}

// Save Floyd-Warshall distance matrix
void saveMatrix(
    int n,
    vector<vector<int>>& dist)
{
    ofstream file("distance_matrix.txt");

    file << n << endl;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            file << dist[i][j] << " ";

        file << endl;
    }

    file.close();
}

// Read distance matrix from file
vector<vector<int>> readMatrix(int& n)
{
    ifstream file("distance_matrix.txt");

    file >> n;

    vector<vector<int>> dist(
        n,
        vector<int>(n)
    );

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            file >> dist[i][j];
    }

    file.close();

    return dist;
}

// Dijkstra on THE SAME graph
int dijkstra(
    int n,
    vector<vector<int>>& graph,
    int source,
    int destination)
{
    vector<int> dist(n, INF);
    vector<bool> visited(n, false);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        if (visited[u])
            continue;

        visited[u] = true;

        for (int v = 0; v < n; v++)
        {
            if (!visited[v] &&
                graph[u][v] != INF)
            {
                if (dist[u] + graph[u][v] < dist[v])
                {
                    dist[v] =
                        dist[u] + graph[u][v];

                    pq.push({dist[v], v});
                }
            }
        }
    }

    return dist[destination];
}

int main()
{
    int n;

    cout << "Enter number of vertices: ";
    cin >> n;

    // ==========================================
    // STEP 1: Generate ONE complete graph
    // ==========================================

    vector<vector<int>> graph(
        n,
        vector<int>(n)
    );

    generateGraph(n, graph);

    // Save the SAME graph
    saveGraph(n, graph);

    cout << "\nComplete graph generated.";
    cout << "\nSaved as graph.txt\n";

    // Display graph
    cout << "\nGenerated Graph:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << graph[i][j] << "\t";

        cout << endl;
    }

    // ==========================================
    // STEP 2: Floyd-Warshall on SAME graph
    // ==========================================

    vector<vector<int>> fw =
        floydWarshall(n, graph);

    // Save result
    saveMatrix(n, fw);

    cout << "\nFloyd-Warshall distance matrix";
    cout << " saved as distance_matrix.txt\n";

    // ==========================================
    // STEP 3: Read Floyd matrix from file
    // ==========================================

    int fileN;

    vector<vector<int>> fileMatrix =
        readMatrix(fileN);

    // ==========================================
    // STEP 4: Input u and v
    // ==========================================

    int u, v;

    cout << "\nEnter u and v: ";
    cin >> u >> v;

    // Convert to 0-based
    u--;
    v--;

    // ==========================================
    // STEP 5: Dijkstra on SAME graph
    // ==========================================

    int duv =
        dijkstra(n, graph, u, v);

    int dvu =
        dijkstra(n, graph, v, u);

    // ==========================================
    // STEP 6: Get Floyd answers FROM FILE
    // ==========================================

    int fw_uv = fileMatrix[u][v];
    int fw_vu = fileMatrix[v][u];

    // ==========================================
    // STEP 7: Compare
    // ==========================================

    cout << "\n================================";
    cout << "\nRESULT";
    cout << "\n================================";

    cout << "\n\nDijkstra d(" << u + 1
         << "," << v + 1 << ") = "
         << duv;

    cout << "\nFloyd d(" << u + 1
         << "," << v + 1 << ") = "
         << fw_uv;

    cout << "\n";

    cout << "\nDijkstra d(" << v + 1
         << "," << u + 1 << ") = "
         << dvu;

    cout << "\nFloyd d(" << v + 1
         << "," << u + 1 << ") = "
         << fw_vu;

    cout << "\n\n================================";
    cout << "\nCOMPARISON";
    cout << "\n================================";

    cout << "\n";

    if (duv == fw_uv)
        cout << "d(" << u + 1 << "," << v + 1
             << ") : MATCH\n";
    else
        cout << "d(" << u + 1 << "," << v + 1
             << ") : NOT MATCH\n";

    if (dvu == fw_vu)
        cout << "d(" << v + 1 << "," << u + 1
             << ") : MATCH\n";
    else
        cout << "d(" << v + 1 << "," << u + 1
             << ") : NOT MATCH\n";

    return 0;
}