#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> sz, par;

    DSU(int n) {
        sz.assign(n, 1);
        par.resize(n);

        for(int i = 0; i < n; i++)
            par[i] = i;
    }

    int find(int v) {
        if(v == par[v])
            return v;

        return par[v] = find(par[v]);
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);

        if(a != b) {
            if(sz[a] < sz[b])
                swap(a, b);

            par[b] = a;
            sz[a] += sz[b];
        }
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    vector<array<int, 3>> edges(m);

    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        edges[i] = {u, v, w};
    }

    // Set stores all different spanning tree costs
    set<int> costs;

    int total = (1 << m);

    for(int mask = 0; mask < total; mask++) {

        if(__builtin_popcount(mask) != n - 1)
            continue;

        DSU dsu(n);

        int cost = 0;
        bool valid = true;

        for(int j = 0; j < m; j++) {

            if((mask >> j) & 1) {

                int u = edges[j][0];
                int v = edges[j][1];
                int w = edges[j][2];

                if(dsu.find(u) == dsu.find(v)) {
                    valid = false;
                    break;
                }

                dsu.unite(u, v);
                cost += w;
            }
        }

        if(valid)
            costs.insert(cost);
    }

    if(costs.empty()) {
        cout << "No spanning tree exists.\n";
        return 0;
    }

    // Minimum and maximum
    int minimum = *costs.begin();
    int maximum = *costs.rbegin();

    // Average
    long long sum = 0;

    for(auto x : costs)
        sum += x;

    double average = (double)sum / costs.size();

    cout << "Minimum: " << minimum << '\n';
    cout << "Maximum: " << maximum << '\n';

    cout << "Average: "
         << fixed << setprecision(2)
         << average << '\n';

    // Check average using set::find()
    if(average == (int)average &&
       costs.find((int)average) != costs.end()) {

        cout << "Average spanning tree is possible.\n";
    }
    else {
        cout << "Average spanning tree is NOT possible.\n";
    }
}
