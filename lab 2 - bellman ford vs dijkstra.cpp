#include <bits/stdc++.h>
#include <chrono>
using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj_list(n);
    
    // Read edges from input
    for(int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj_list[u].push_back({v, w});
        // For undirected graph, uncomment the line below
        // adj_list[v].push_back({u, w});
    }

    // Dijkstra's Algorithm
    vector<int> dist(n, 1e9);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    int source = 0;
    pq.push({0, source});
    dist[source] = 0;
    
    auto start = chrono::high_resolution_clock::now();
    while(!pq.empty())
    {
        pair<int, int> top = pq.top();
        int d = top.first;
        int u = top.second;
        pq.pop();
        
        if(d > dist[u])
            continue;
            
        for(int i = 0; i < (int)adj_list[u].size(); i++)
        {
            int v = adj_list[u][i].first;
            int w = adj_list[u][i].second;
            
            if(dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken by Dijkstra algo: " << duration.count() << " microseconds" << endl;

    cout << "Dijkstra output: ";
    for(int d : dist)
        cout << d << " ";
    cout << endl;

    // Reset distances for Bellman-Ford
    vector<int> bf_dist(n, 1e9);
    bf_dist[source] = 0;
    
    start = chrono::high_resolution_clock::now();
    
    // Bellman-Ford Algorithm - run n-1 times
    for(int iteration = 0; iteration < n - 1; iteration++)
    {
        bool updated = false;
        for(int u = 0; u < n; u++)
        {
            if(bf_dist[u] == 1e9) continue;
            
            for(int i = 0; i < (int)adj_list[u].size(); i++)
            {
                int v = adj_list[u][i].first;
                int w = adj_list[u][i].second;
                
                if(bf_dist[v] > bf_dist[u] + w)
                {
                    bf_dist[v] = bf_dist[u] + w;
                    updated = true;
                }
            }
        }
        // Early termination if no updates
        if(!updated) break;
    }
    
    // Check for negative cycles
    bool hasNegativeCycle = false;
    for(int u = 0; u < n; u++)
    {
        if(bf_dist[u] == 1e9) continue;
        
        for(int i = 0; i < (int)adj_list[u].size(); i++)
        {
            int v = adj_list[u][i].first;
            int w = adj_list[u][i].second;
            
            if(bf_dist[v] > bf_dist[u] + w)
            {
                hasNegativeCycle = true;
                break;
            }
        }
        if(hasNegativeCycle) break;
    }
    
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken by Bellman-Ford algo: " << duration.count() << " microseconds" << endl;

    if(hasNegativeCycle) {
        cout << "Bellman-Ford output: Negative cycle detected!" << endl;
    } else {
        cout << "Bellman-Ford output: ";
        for(int d : bf_dist)
            cout << d << " ";
        cout << endl;
    }

    return 0;
}
/*5 10
0 1 7
0 2 6
1 4 9
1 3 -3
2 3 5
2 4 -4
2 1 8
3 2 -2
4 3 7
4 0 2*/