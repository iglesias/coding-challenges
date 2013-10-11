#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

struct edge
{
    int dst;
    int w;
    
    edge() : dst(0), w(0) { }
    edge(int aDst, int aW) : dst(aDst), w(aW) { }
};

struct compare
{
    bool operator()(const edge& lhs, const edge& rhs)
    {
         return lhs.w > rhs.w;
    }
};

typedef vector<vector<edge> > graph;
typedef priority_queue<edge, vector<edge>, compare> pq;
typedef long long ll;

void insert_edges(pq& q, const graph& g, int id)
{
    for (int i = 0; i < g[id].size(); ++i) q.push(g[id][i]);
}

ll prim(const graph& g)
{
    vector<bool> in_tree(g.size(), false);
    pq q;
    
    insert_edges(q, g, 1);
    in_tree[1] = true;
    int in_tree_count = 1;
    
    ll cost = 0;
    
    while (in_tree_count < g.size()-1)
    {
        edge e;
        while (true)
        {
            e = q.top();
            q.pop();
            if (in_tree[e.dst] == false) break;
        }
        
        cost += e.w;
        insert_edges(q, g, e.dst);
        in_tree[e.dst] = true;
        in_tree_count++;
    }

    return cost;
}

int main()
{
    int N, E;
    cin >> N >> E;
    graph g(N+1);
    
    for (int i = 0; i < E; i++)
    {
        int U, V, W;
        cin >> U >> V >> W;
        g[U].push_back(edge(V, W));
        g[V].push_back(edge(U, W));
    }
    
    int T1, T2;
    cin >> T1 >> T2;
    
    if (prim(g) <= 0)   cout << min(T1, T2) << endl;
    else                cout << max(T1, T2) << endl;
    
    return 0;
}

