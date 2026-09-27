#include<bits/stdc++.h>
using namespace std;
int findPar(int u,vector<int>&par)
{

    if(par[u]==u) return u;
    return par[u]=findPar(par[u],par);

}
void unionset( int u,int v,vector<int>&par)
{
     u=findPar(u,par);
     v=findPar(v,par);
    if(u!=v) par[v]=u;

}

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
        if(u<v) ed.push_back({w,{u,v}});
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
    int u= ed[i].second.first;
    int v=ed[i].second.second;
    if(findPar(u,par)!=findPar(v,par))
    {
        cout<<u<<" = "<<v<<" : "<<w<<endl;
        tcost+=w;
        unionset(u,v,par) ;
        cnt++;
        if(cnt==v-1) break;
    }
}
cout<<"MST  cost: "<<tcost<<endl;



}
/*0 1 4
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
7 8 7*/

