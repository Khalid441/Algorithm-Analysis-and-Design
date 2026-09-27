#include <bits/stdc++.h>
using namespace std;

//Minimum Spanning Tree

struct DSU {
    vector <int> par, sz;
    int n;

    DSU(int n)
    : n(n) {
        sz.assign(n, 1);
        par.assign(n, 0);

        for(int i = 0; i < n; ++i) {
            par[i] = i;
        }
    }

    int find(int v) {
        if(par[v] == v) {
            return v;
        }

        return par[v] = find(par[v]);
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);

        if(a != b) {
            if(sz[a] < sz[b]) {
                swap(a, b);
            }

            par[b] = par[a];
            sz[a] += sz[b];
        }
    }
};

bool cmp(array <int, 3> &a, array <int, 3> &b) {
    if(a[2] != b[2]) {
        return a[2] < b[2];
    }

    if(a[0] != b[0]) {
        return a[0] < b[0];
    }

    return a[1] < b[1];
}

int main() {
    int n, m;
    cin >> n >> m;

    vector <array <int, 3>> edges(m);
    for(auto &arr : edges) {
        cin >> arr[0] >> arr[1] >> arr[2];
    }

    sort(edges.begin(), edges.end(), cmp);

    DSU dsu(n);

    vector <array <int, 3>> mst;
    int cost = 0;
    for(auto &arr : edges) {
        if(mst.size() == n - 1) {
            break;
        }

        int u = arr[0], v = arr[1], w = arr[2];
        if(dsu.find(u) != dsu.find(v)) {
            mst.push_back(arr);
            cost += w;
            dsu.unite(u, v);
        }
    }

    cout << "\nMinimum Spanning Tree cost: " << cost << '\n';

    cout << "Minimum Spanning Tree:\n";
    for(auto &arr : mst) {
        cout << arr[0] << ' ' << arr[1] << ' ' << arr[2] << '\n';
    }
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
