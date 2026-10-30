#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // path value = sum + max edge = max over edges e on the path of (sum + w_e),
    // i.e. the longest path where exactly one edge may be counted twice.
    // d0[v]: longest downward path from v, no edge doubled
    // d1[v]: longest downward path from v, one edge doubled
    vector<int> par(n + 1, 0), order;
    vector<ll> d0(n + 1, 0), d1(n + 1, 0);
    vector<int> pw(n + 1, 0);
    order.reserve(n);
    vector<int> st = {1};
    while (!st.empty())
    {
        int u = st.back();
        st.pop_back();
        order.push_back(u);
        for (auto [v, w] : adj[u])
            if (v != par[u])
                par[v] = u, pw[v] = w, st.push_back(v);
    }

    ll ans = 0;
    vector<ll> b0(n + 1, 0), b1(n + 1, 0); // best branches seen so far at each node
    for (int i = n - 1; i >= 0; i--)       // children before parents
    {
        int c = order[i], p = par[c];
        d0[c] = b0[c], d1[c] = b1[c];
        if (!p)
            break;
        ll t0 = d0[c] + pw[c];
        ll t1 = max(d1[c] + pw[c], d0[c] + 2LL * pw[c]);
        ans = max(ans, max(b0[p] + t1, b1[p] + t0));
        b0[p] = max(b0[p], t0);
        b1[p] = max(b1[p], t1);
    }
    cout << ans << "\n";
}
