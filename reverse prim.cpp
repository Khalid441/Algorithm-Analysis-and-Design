#include<bits/stdc++.h>
using namespace std;
int main()
{ //MST
    int v,e;
    cin>>v>>e;
    vector<vector<pair<int,int>>>adj(v);
    for(int i=0; i<e; i++)
    {
        int u,v,w;
        cin>>u>>v>>w;
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});

    }
    vector<bool>vis(v,false);
    priority_queue<pair<int,int>>pq;
    pq.push({0,0});
    int total=0;
    while(!pq.empty())
    {
        int w=pq.top().first;
        int u=pq.top().second;
        pq.pop();
        if(vis[u]==true) continue;
        vis[u]=true;
        total+=w;
        for(int i=0; i<adj[u].size(); i++)
        {
            int v=adj[u][i].first;
            int wt=adj[u][i].second;
            if(vis[v]==false) pq.push({wt,v});
        }

    }

    cout<<"Cost : "<<total<<endl;
}


/*9 14
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


