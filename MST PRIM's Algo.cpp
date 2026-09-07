#include <bits/stdc++.h>
using namespace std;

int prim(vector <vector <pair <int, int>>> &adList, vector <bool> &vis, int src) {
    int cost = 0;

    priority_queue <pair <int, int>, vector <pair <int, int>>, greater <pair <int, int>>> pq;
    pq.push({0, src});

    while(!pq.empty()) {
        int w = pq.top().first;
        int u = pq.top().second;

        pq.pop();

        if(vis[u]) {
            continue;
        }

        vis[u] = true;
        cost += w;
        //cout << "Cost: " << cost << '\n';

        for(auto &nei : adList[u]) {
            int v = nei.second;
            int wt = nei.first;

            if(vis[v]) {
                continue;
            }

            pq.push({wt, v});
        }
    }

    return cost;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector <vector <pair <int, int>>> adList(n);
    for(int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;

        adList[u].push_back({w, v});
        adList[v].push_back({w, u});
    }

    vector <bool> vis(n, false);

    cout << "Cost: " << prim(adList, vis, 5) << '\n';
}
/*
9 14
0 1 4
0 7 8
1 2 8
1 7 11
2 3 7
2 5 4
2 8 2
3 4 9
3 5 14
4 5 10
5 6 2
6 7 1
6 8 6
7 8 7

*/

