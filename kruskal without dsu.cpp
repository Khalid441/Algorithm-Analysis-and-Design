#include<bits/stdc++.h>
using namespace std;

int main()
{
    int v,e;
    cin>>v>>e;

    vector<vector<pair<int,int>>>adj(v);

    for(int i=0;i<e;i++)
    {
        int u,v,w;
        cin>>u>>v>>w;

        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }

    vector<pair<int,pair<int,int>>>ed;

    for(int u=0;u<v;u++)
    {
        for(int i=0;i<adj[u].size();i++)
        {
            int v=adj[u][i].first;
            int w=adj[u][i].second;

            if(u<v)
                ed.push_back({w,{u,v}});
        }
    }

    sort(ed.begin(),ed.end());
    vector<int>par(v);

    for(int i=0;i<v;i++)
    {
        par[i]=i;
    }

    int tcost=0,cnt=0;

    cout<<"MST: "<<endl;

    for(int i=0;i<ed.size();i++)
    {
        int w=ed[i].first;
        int u=ed[i].second.first;
        int v=ed[i].second.second;

        if(par[u]!=par[v])
        {
            cout<<u<<" = "<<v<<" : "<<w<<endl;

            tcost+=w;
            cnt++;
            int oldpar=par[v];

            for(int j=0;j<par.size();j++)
            {
                if(par[j]==oldpar)
                    par[j]=par[u];
            }

            if(cnt==v-1)
                break;
        }
    }

    cout<<"MST cost: "<<tcost<<endl;
}
