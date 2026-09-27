#include <bits/stdc++.h>
using namespace std;

const int MAX = 100;

int capacity[MAX][MAX];
int flow[MAX][MAX];
int n;
void createGraph()
{
    srand(time(0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                capacity[i][j] = rand() % 20 + 1;
            }
        }
    }
}

bool bfs(int source, int sink, int parent[])
{
    bool visited[MAX] = {false};

    queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++)
        {
            if (!visited[v] &&
                capacity[u][v] - flow[u][v] > 0)
            {
                parent[v] = u;
                visited[v] = true;
                q.push(v);

                if (v == sink)
                    return true;
            }
        }
    }

    return false;
}
int maxFlow(int source, int sink)
{
    int totalFlow = 0;
    int parent[MAX];

    while (bfs(source, sink, parent))
    {
        int pathFlow = INT_MAX;
        int v = sink;
        while (v != source)
        {
            int u = parent[v];

            pathFlow = min(pathFlow,
                           capacity[u][v] - flow[u][v]);

            v = u;
        }
        v = sink;

        while (v != source)
        {
            int u = parent[v];

            flow[u][v] += pathFlow;
            flow[v][u] -= pathFlow;

            v = u;
        }

        totalFlow += pathFlow;
    }

    return totalFlow;
}
void displayGraph()
{
    cout << "\nGenerated Graph:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                cout << i << " -> "
                     << j << " = "
                     << capacity[i][j] << endl;
            }
        }
    }
}

int main()
{
    int source, sink;

    cout << "Enter number of vertices: ";
    cin >> n;

    createGraph();

    displayGraph();

    cout << "\nEnter source: ";
    cin >> source;

    cout << "Enter sink: ";
    cin >> sink;

    int answer = maxFlow(source, sink);

    cout << "\nMaximum Flow = "
         << answer << endl;

    return 0;
}
