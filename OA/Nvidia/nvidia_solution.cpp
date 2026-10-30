#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int LOG = 18;

int n;
vector<pair<int, int>> adj[200005];
int up[LOG][200005], depthLvl[200005];
ll distRoot[200005];

int lca(int a, int b)
{
    if (depthLvl[a] < depthLvl[b])
        swap(a, b);
    int diff = depthLvl[a] - depthLvl[b];
    for (int k = 0; k < LOG; k++)
        if (diff >> k & 1)
            a = up[k][a];
    if (a == b)
        return a;
    for (int k = LOG - 1; k >= 0; k--)
        if (up[k][a] != up[k][b])
            a = up[k][a], b = up[k][b];
    return up[0][a];
}

ll dist(int a, int b)
{
    return distRoot[a] + distRoot[b] - 2 * distRoot[lca(a, b)];
}

int par[200005];
int e1[200005], e2[200005]; // diameter endpoints of each component
ll diam[200005];

int find(int x)
{
    while (par[x] != x)
        par[x] = par[par[x]], x = par[x];
    return x;
}

int main()
{
    scanf("%d", &n);
    struct Edge
    {
        int u, v;
        ll w;
    };
    vector<Edge> edges(n - 1);
    for (auto &e : edges)
    {
        scanf("%d %d %lld", &e.u, &e.v, &e.w);
        adj[e.u].push_back({e.v, (int)e.w});
        adj[e.v].push_back({e.u, (int)e.w});
    }
    if (n == 1)
    {
        printf("0\n");
        return 0;
    }

    // iterative DFS from node 1 (a chain of 2e5 nodes would overflow recursion)
    vector<int> st = {1};
    up[0][1] = 1;
    while (!st.empty())
    {
        int u = st.back();
        st.pop_back();
        for (auto [v, w] : adj[u])
            if (v != up[0][u])
            {
                up[0][v] = u;
                depthLvl[v] = depthLvl[u] + 1;
                distRoot[v] = distRoot[u] + w;
                st.push_back(v);
            }
    }
    for (int k = 1; k < LOG; k++)
        for (int v = 1; v <= n; v++)
            up[k][v] = up[k - 1][up[k - 1][v]];

    // Process edges by increasing weight. When edge (x,y,w) joins two components,
    // every path crossing it has max edge exactly w, so its value is sum + w.
    // Best path through the edge = farthest from x in A + w + farthest from y in B + w.
    // The farthest node from any node of a tree is an endpoint of its diameter.
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b)
         { return a.w < b.w; });
    for (int i = 1; i <= n; i++)
        par[i] = e1[i] = e2[i] = i;

    ll ans = 0;
    for (auto &e : edges)
    {
        int rx = find(e.u), ry = find(e.v);
        ll fa = max(dist(e.u, e1[rx]), dist(e.u, e2[rx]));
        ll fb = max(dist(e.v, e1[ry]), dist(e.v, e2[ry]));
        ans = max(ans, fa + fb + 2 * e.w);

        // new diameter of the merged component
        int cand[4] = {e1[rx], e2[rx], e1[ry], e2[ry]};
        int best1 = e1[rx], best2 = e2[rx];
        ll bestD = diam[rx];
        if (diam[ry] > bestD)
            bestD = diam[ry], best1 = e1[ry], best2 = e2[ry];
        for (int i = 0; i < 2; i++)
            for (int j = 2; j < 4; j++)
            {
                ll d = dist(cand[i], cand[j]);
                if (d > bestD)
                    bestD = d, best1 = cand[i], best2 = cand[j];
            }
        par[ry] = rx;
        e1[rx] = best1, e2[rx] = best2, diam[rx] = bestD;
    }
    printf("%lld\n", ans);
}
