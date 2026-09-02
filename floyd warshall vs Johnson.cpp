#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

// ------------------------------------------------
// Generate complete directed weighted graph
// ------------------------------------------------

void generateCompleteGraph(
    int n,
    vector<vector<long long> >& graph)
{
    srand(time(0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = rand() % 20 + 1; // weights 1-20
        }
    }
}

// ------------------------------------------------
// Floyd-Warshall
// ------------------------------------------------

vector<vector<long long> > floydWarshall(
    int n,
    vector<vector<long long> > graph)
{
    vector<vector<long long> > dist = graph;

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (dist[i][k] != INF &&
                    dist[k][j] != INF)
                {
                    dist[i][j] =
                        min(
                            dist[i][j],
                            dist[i][k] + dist[k][j]
                        );
                }
            }
        }
    }

    return dist;
}

// ------------------------------------------------
// Bellman-Ford for Johnson (from super source)
// ------------------------------------------------

bool bellmanFord(
    int n,
    vector<vector<long long> >& graph,
    vector<long long>& h)
{
    // Add super source (index n)
    h.assign(n + 1, INF);
    h[n] = 0;

    // Run Bellman-Ford n times (n+1 vertices total)
    for (int iter = 0; iter < n; iter++)
    {
        bool changed = false;

        // Relax edges from super source (weight 0 to all vertices)
        for (int v = 0; v < n; v++)
        {
            if (h[n] != INF && h[n] + 0 < h[v])
            {
                h[v] = h[n] + 0;
                changed = true;
            }
        }

        // Relax original edges
        for (int u = 0; u < n; u++)
        {
            if (h[u] == INF) continue;
            
            for (int v = 0; v < n; v++)
            {
                if (graph[u][v] != INF &&
                    h[u] + graph[u][v] < h[v])
                {
                    h[v] = h[u] + graph[u][v];
                    changed = true;
                }
            }
        }

        if (!changed)
            break;
    }

    // Check for negative cycles
    for (int u = 0; u < n; u++)
    {
        if (h[u] == INF) continue;
        
        for (int v = 0; v < n; v++)
        {
            if (graph[u][v] != INF &&
                h[u] + graph[u][v] < h[v])
            {
                return false; // Negative cycle detected
            }
        }
    }

    return true;
}

// ------------------------------------------------
// Johnson's Algorithm
// ------------------------------------------------

bool johnson(
    int n,
    vector<vector<long long> >& graph,
    vector<vector<long long> >& result)
{
    // Step 1: Bellman-Ford from super source
    vector<long long> h;
    
    if (!bellmanFord(n, graph, h))
        return false;

    // Step 2: Reweight edges
    vector<vector<long long> > reweighted(n, vector<long long>(n, INF));
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (graph[i][j] != INF)
            {
                reweighted[i][j] = graph[i][j] + h[i] - h[j];
            }
        }
    }

    // Step 3: Dijkstra from each vertex
    result.assign(n, vector<long long>(n, INF));

    for (int source = 0; source < n; source++)
    {
        vector<long long> dist(n, INF);
        vector<bool> visited(n, false);

        priority_queue<
            pair<long long, int>,
            vector<pair<long long, int> >,
            greater<pair<long long, int> >
        > pq;

        dist[source] = 0;
        pq.push(make_pair(0, source));

        while (!pq.empty())
        {
            pair<long long, int> top = pq.top();
            long long d = top.first;
            int u = top.second;
            pq.pop();

            if (visited[u])
                continue;

            visited[u] = true;

            for (int v = 0; v < n; v++)
            {
                if (!visited[v] && reweighted[u][v] != INF)
                {
                    if (dist[u] + reweighted[u][v] < dist[v])
                    {
                        dist[v] = dist[u] + reweighted[u][v];
                        pq.push(make_pair(dist[v], v));
                    }
                }
            }
        }

        // Restore original distances
        for (int v = 0; v < n; v++)
        {
            if (dist[v] != INF)
            {
                result[source][v] = dist[v] - h[source] + h[v];
            }
        }
    }

    return true;
}

// ------------------------------------------------
// Save matrix to file
// ------------------------------------------------

void saveMatrix(
    string filename,
    int n,
    vector<vector<long long> >& matrix)
{
    ofstream file(filename.c_str());

    file << n << endl;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == INF)
                file << "INF ";
            else
                file << matrix[i][j] << " ";
        }
        file << endl;
    }

    file.close();
}

// ------------------------------------------------
// Main
// ------------------------------------------------

int main()
{
    int n;

    cout << "Enter number of vertices: ";
    cin >> n;

    // Create complete graph
    vector<vector<long long> > graph(
        n,
        vector<long long>(n, INF)
    );

    generateCompleteGraph(n, graph);

    cout << "\nComplete graph generated with " << n << " vertices\n";

    // -----------------------------
    // Floyd-Warshall
    // -----------------------------

    cout << "\nRunning Floyd-Warshall...\n";
    
    chrono::high_resolution_clock::time_point startFW =
        chrono::high_resolution_clock::now();

    vector<vector<long long> > fw =
        floydWarshall(n, graph);

    chrono::high_resolution_clock::time_point endFW =
        chrono::high_resolution_clock::now();

    saveMatrix("floyd.txt", n, fw);

    // -----------------------------
    // Johnson's Algorithm
    // -----------------------------

    cout << "Running Johnson's Algorithm...\n";

    chrono::high_resolution_clock::time_point startJ =
        chrono::high_resolution_clock::now();

    vector<vector<long long> > johnsonResult;

    bool possible = johnson(n, graph, johnsonResult);

    chrono::high_resolution_clock::time_point endJ =
        chrono::high_resolution_clock::now();

    if (!possible)
    {
        cout << "\nNegative cycle detected!";
        cout << "\nJohnson cannot be applied.\n";
        return 0;
    }

    saveMatrix("johnson.txt", n, johnsonResult);

    // -----------------------------
    // Compare Results
    // -----------------------------

    int matches = 0;
    int mismatches = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (fw[i][j] == johnsonResult[i][j])
                matches++;
            else
                mismatches++;
        }
    }

    // -----------------------------
    // Print Results
    // -----------------------------

    double fwTime =
        chrono::duration<double>(endFW - startFW).count();

    double johnsonTime =
        chrono::duration<double>(endJ - startJ).count();

    cout << "\n========== RESULT ==========\n";
    cout << "Total comparisons : " << n * n << endl;
    cout << "Matches           : " << matches << endl;
    cout << "Mismatches        : " << mismatches << endl;

    cout << "\n========== TIME ==========\n";
    cout << "Floyd-Warshall : " << fwTime << " seconds\n";
    cout << "Johnson        : " << johnsonTime << " seconds\n";

    // Display sample of results
    cout << "\n========== SAMPLE OUTPUT ==========\n";
    cout << "\nShortest paths from vertex 0:\n";
    for (int j = 0; j < min(n, 5); j++)
    {
        cout << "0 -> " << j << ": ";
        if (fw[0][j] == INF)
            cout << "INF";
        else
            cout << fw[0][j];
        cout << endl;
    }

    return 0;
}