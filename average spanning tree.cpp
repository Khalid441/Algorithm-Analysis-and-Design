#include <bits/stdc++.h>
using namespace std;

// ---------------- DSU ----------------

int findPar(int u, vector<int>& par)
{
    if (par[u] == u)
        return u;

    return par[u] = findPar(par[u], par);
}

void unionset(int u, int v, vector<int>& par)
{
    u = findPar(u, par);
    v = findPar(v, par);

    if (u != v)
        par[v] = u;
}

// ---------------- Kruskal ----------------

int kruskal(
    int n,
    vector<pair<int, pair<int, int>>> ed,
    bool maximum,
    vector<pair<int, pair<int, int>>>& tree)
{
    if (maximum)
    {
        // Maximum spanning tree
        sort(ed.begin(), ed.end(), greater<pair<int, pair<int, int>>>());
    }
    else
    {
        // Minimum spanning tree
        sort(ed.begin(), ed.end());
    }

    vector<int> par(n);

    for (int i = 0; i < n; i++)
        par[i] = i;

    int cost = 0;
    int cnt = 0;

    for (int i = 0; i < ed.size(); i++)
    {
        int w = ed[i].first;
        int u = ed[i].second.first;
        int v = ed[i].second.second;

        if (findPar(u, par) != findPar(v, par))
        {
            tree.push_back(ed[i]);

            cost += w;
            unionset(u, v, par);

            cnt++;

            if (cnt == n - 1)
                break;
        }
    }

    return cost;
}

// ------------------------------------------------------
// Find a spanning tree with EXACT target cost
// ------------------------------------------------------

bool findAverageTree(
    int index,
    int n,
    int target,
    vector<pair<int, pair<int, int>>>& ed,
    vector<int>& par,
    vector<pair<int, pair<int, int>>>& tree,
    int currentCost)
{
    // We have selected n-1 edges
    if (tree.size() == n - 1)
    {
        if (currentCost == target)
            return true;

        return false;
    }

    // No more edges
    if (index == ed.size())
        return false;

    // Cost already exceeded
    if (currentCost > target)
        return false;

    int w = ed[index].first;
    int u = ed[index].second.first;
    int v = ed[index].second.second;

    // -----------------------------
    // OPTION 1: Take this edge
    // -----------------------------

    int pu = findPar(u, par);
    int pv = findPar(v, par);

    if (pu != pv)
    {
        // Save DSU state
        vector<int> oldPar = par;

        unionset(u, v, par);

        tree.push_back(ed[index]);

        if (findAverageTree(
                index + 1,
                n,
                target,
                ed,
                par,
                tree,
                currentCost + w))
        {
            return true;
        }

        // Backtrack
        tree.pop_back();
        par = oldPar;
    }

    // -----------------------------
    // OPTION 2: Don't take edge
    // -----------------------------

    if (findAverageTree(
            index + 1,
            n,
            target,
            ed,
            par,
            tree,
            currentCost))
    {
        return true;
    }

    return false;
}

// ---------------- Print Tree ----------------

void printTree(
    string name,
    vector<pair<int, pair<int, int>>> tree,
    int cost)
{
    cout << "\n" << name << endl;
    cout << "----------------------" << endl;

    for (int i = 0; i < tree.size(); i++)
    {
        int w = tree[i].first;
        int u = tree[i].second.first;
        int v = tree[i].second.second;

        cout << u << " = " << v << " : " << w << endl;
    }

    cout << "Cost = " << cost << endl;
}

// ---------------- Main ----------------

int main()
{
    int n, e;

    cin >> n >> e;

    vector<pair<int, pair<int, int>>> ed;

    // Input edges
    for (int i = 0; i < e; i++)
    {
        int u, v, w;

        cin >> u >> v >> w;

        ed.push_back({w, {u, v}});
    }

    // =================================================
    // MINIMUM SPANNING TREE
    // =================================================

    vector<pair<int, pair<int, int>>> minTree;

    int minCost = kruskal(
        n,
        ed,
        false,
        minTree
    );

    printTree(
        "Minimum Spanning Tree",
        minTree,
        minCost
    );

    // =================================================
    // MAXIMUM SPANNING TREE
    // =================================================

    vector<pair<int, pair<int, int>>> maxTree;

    int maxCost = kruskal(
        n,
        ed,
        true,
        maxTree
    );

    printTree(
        "Maximum Spanning Tree",
        maxTree,
        maxCost
    );

    // =================================================
    // AVERAGE
    // =================================================

    int sum = minCost + maxCost;

    double average = sum / 2.0;

    cout << "\nAverage = "
         << average << endl;

    // =================================================
    // FIND ACTUAL AVERAGE SPANNING TREE
    // =================================================

    if (sum % 2 != 0)
    {
        cout << "\nAverage is not an integer." << endl;
        return 0;
    }

    int target = sum / 2;

    // Sort edges by weight
    sort(ed.begin(), ed.end());

    vector<int> par(n);

    for (int i = 0; i < n; i++)
        par[i] = i;

    vector<pair<int, pair<int, int>>> averageTree;

    bool found = findAverageTree(
        0,
        n,
        target,
        ed,
        par,
        averageTree,
        0
    );

    // =================================================
    // RESULT
    // =================================================

    if (found)
    {
        printTree(
            "Average Spanning Tree",
            averageTree,
            target
        );
    }
    else
    {
        cout << "\nNo spanning tree with cost "
             << target
             << " exists." << endl;
    }

    return 0;
}
