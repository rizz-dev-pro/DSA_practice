# 🚀 C++ DSA — Advanced (Part II)

![level](https://img.shields.io/badge/level-advanced-red) ![C++14](https://img.shields.io/badge/C%2B%2B-14-blue?logo=cplusplus) ![contests](https://img.shields.io/badge/for-contests%20%7C%20placements%20%7C%20interviews-brightgreen)

> [!IMPORTANT]
> **Prerequisite:** the core sheet [notes.md](notes.md) (sections 1–21). This part has full implementations (C++14), theory, interview classics and a practice roadmap. Code assumes the template in [section 1](notes.md#s1).

<a id="contents"></a>
## 📑 Contents
1. 🕸️ [Advanced graph algorithms](#a1)
2. 🏗️ [Advanced data structures](#a2)
3. 🧠 [Advanced dynamic programming](#a3)
4. 🔤 [Advanced string algorithms](#a4)
5. 🔐 [Advanced math](#a5)
6. ♟️ [Game theory & computational geometry](#a6)
7. 📐 [Theory, analysis & proofs](#a7)
8. 💼 [Interview & placement toolkit](#a8)
9. 🗺️ [Practice roadmap & resources](#a9)

---

<a id="a1"></a>
## 🕸️ 1. Advanced graph algorithms

**Which algorithm?**
| Problem | Tool |
|---|---|
| Strongly connected components, condensation DAG | Tarjan / Kosaraju |
| Critical edges / vertices (bridges, articulation) | low-link DFS |
| k-th ancestor, LCA, path length on tree | binary lifting / Euler tour + sparse table |
| Path/subtree queries with updates on a tree | Euler tour + BIT/segtree, or HLD |
| Count paths with a property (length k, sum) | centroid decomposition |
| Max flow / min cut / assignment / matching | Dinic, Kuhn, Hopcroft–Karp |
| Min-cost assignment / transport | min-cost max-flow |
| Boolean clauses `(a or b)` | 2-SAT via SCC |
| Visit every edge once | Eulerian path (Hierholzer) |
| Offline connectivity with deletions | DSU rollback + divide & conquer on time |

### SCC (Tarjan) O(V+E)
```cpp
int timer_ = 0, ncomp = 0;
vector<int> tin, low, comp, stk;
vector<bool> onst;
void dfs(int u) {
    tin[u] = low[u] = ++timer_;
    stk.push_back(u);
    onst[u] = true;
    for (int v : g[u]) {
        if (!tin[v]) {
            dfs(v);
            low[u] = min(low[u], low[v]);
        } else if (onst[v])
            low[u] = min(low[u], tin[v]);
    }
    if (low[u] == tin[u]) {  // u is the root of an SCC
        while (true) {
            int v = stk.back();
            stk.pop_back();
            onst[v] = false;
            comp[v] = ncomp;
            if (v == u) break;
        }
        ncomp++;
    }
}
// init: tin(n,0), low(n), comp(n,-1), onst(n,false); for u: if(!tin[u]) dfs(u);
```
- Component ids come out in **reverse topological order** of the condensation DAG (comp 0 is a sink).
- Condense: for each edge `u->v` with `comp[u]!=comp[v]` add `comp[u]->comp[v]`; then DP on the DAG (longest path, reachability with bitsets, counting).
- Kosaraju alternative: DFS order on G, then DFS on the reversed graph in reverse finishing order.

### Bridges & articulation points (handles multi-edges via edge ids)
```cpp
vector<vector<pair<int, int>>> g;  // {to, edge id}
vector<int> tin, low;
vector<bool> isCut, isBridge;
int timer_ = 0;
void dfs(int u, int pe) {
    tin[u] = low[u] = ++timer_;
    int children = 0;
    for (auto& e : g[u]) {
        int v = e.first, id = e.second;
        if (id == pe) continue;
        if (tin[v])
            low[u] = min(low[u], tin[v]);  // back edge
        else {
            dfs(v, id);
            low[u] = min(low[u], low[v]);
            children++;
            if (low[v] > tin[u]) isBridge[id] = true;  // no back edge above u
            if (low[v] >= tin[u] && pe != -1) isCut[u] = true;
        }
    }
    if (pe == -1 && children > 1) isCut[u] = true;  // root rule
}
```
- 2-edge-connected components: remove bridges, take components. **Bridge tree** is a tree of those components.
- Block-cut tree for 2-vertex-connected components.

### LCA by binary lifting (O(n log n) build, O(log n) query)
```cpp
const int LOG = 18;  // 2^18 > 2e5
int up[LOG][N], dep[N];
void dfs(int u, int p) {  // call dfs(root, root)
    up[0][u] = p;
    for (int k = 1; k < LOG; k++)
        up[k][u] = up[k - 1][up[k - 1][u]];
    for (int v : g[u])
        if (v != p) {
            dep[v] = dep[u] + 1;
            dfs(v, u);
        }
}
int kth(int a, int k) {
    for (int i = 0; i < LOG; i++)
        if (k >> i & 1) a = up[i][a];
    return a;
}
int lca(int a, int b) {
    if (dep[a] < dep[b]) swap(a, b);
    a = kth(a, dep[a] - dep[b]);
    if (a == b) return a;
    for (int k = LOG - 1; k >= 0; k--)
        if (up[k][a] != up[k][b]) a = up[k][a], b = up[k][b];
    return up[0][a];
}
// dist(a,b) = dep[a] + dep[b] - 2*dep[lca(a,b)]
```
- O(1) LCA: Euler tour (size 2n-1) + sparse table of min depth.
- Max edge on path (MST queries): store `mx[k][v]` beside `up[k][v]`.
- Binary lifting also gives k-th successor in functional graphs.

### Euler tour (subtree = contiguous range)
```cpp
void dfs(int u, int p) {
    tin[u] = timer_++;
    for (int v : g[u])
        if (v != p) dfs(v, u);
    tout[u] = timer_;
}  // subtree = [tin, tout)
```
- Subtree update/query → BIT/segtree on `tin`. Path-to-root sum with point updates: add `+x` at `tin[u]`, `-x` at `tout[u]`, query = prefix sum at `tin[v]`.

### Heavy-light decomposition (path queries in O(log² n))
```cpp
int par[N], dep[N], heavy[N], head[N], pos[N], sz[N], cur_ = 0;  // par[root] = -1
int dfs1(int u) {
    sz[u] = 1;
    int mx = 0;
    heavy[u] = -1;
    for (int v : g[u])
        if (v != par[u]) {
            par[v] = u;
            dep[v] = dep[u] + 1;
            sz[u] += dfs1(v);
            if (sz[v] > mx) mx = sz[v], heavy[u] = v;
        }
    return sz[u];
}
void dfs2(int u, int h) {
    head[u] = h;
    pos[u] = cur_++;
    if (heavy[u] != -1) dfs2(heavy[u], h);  // heavy child gets the next position
    for (int v : g[u])
        if (v != par[u] && v != heavy[u]) dfs2(v, v);
}
ll queryPath(int a, int b) {  // vertex values, segtree over pos[]
    ll res = -LINF;
    while (head[a] != head[b]) {
        if (dep[head[a]] < dep[head[b]]) swap(a, b);
        res = max(res, seg.query(pos[head[a]], pos[a] + 1));
        a = par[head[a]];
    }
    if (dep[a] > dep[b]) swap(a, b);
    return max(res, seg.query(pos[a],
                              pos[b] + 1));  // for edge values skip pos[a] (use pos[a]+1)
}
// subtree of u = [pos[u], pos[u] + sz[u])
```

### Centroid decomposition (O(n log n) paths-through-centroid)
```cpp
int sz[N];
bool removed[N];
int calc(int u, int p) {
    sz[u] = 1;
    for (int v : g[u])
        if (v != p && !removed[v]) sz[u] += calc(v, u);
    return sz[u];
}
int centroid(int u, int p, int tot) {
    for (int v : g[u])
        if (v != p && !removed[v] && sz[v] * 2 > tot) return centroid(v, u, tot);
    return u;
}
void decompose(int u) {
    int c = centroid(u, -1, calc(u, -1));
    removed[c] = true;
    // count/combine all paths passing through c here (DFS from c, merge subtree results with a counter)
    for (int v : g[c])
        if (!removed[v]) decompose(v);
}
```
- Every path is counted at exactly one centroid; depth of centroid tree ≤ log n. Use for "number of paths of length k", nearest marked vertex queries.

### Max flow — Dinic O(V²E), unit-capacity O(E√V)
```cpp
struct Dinic {
    struct E {
        int to;
        ll cap;
    };
    vector<E> e;
    vector<vector<int>> g;
    vector<int> level, it;
    Dinic(int n) : g(n), level(n), it(n) {}
    void add(int u, int v, ll c) {
        g[u].push_back(e.size());
        e.push_back({v, c});
        g[v].push_back(e.size());
        e.push_back({u, 0});
    }
    bool bfs(int s, int t) {
        fill(all(level), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int id : g[u])
                if (e[id].cap > 0 && level[e[id].to] < 0) {
                    level[e[id].to] = level[u] + 1;
                    q.push(e[id].to);
                }
        }
        return level[t] >= 0;
    }
    ll dfs(int u, int t, ll f) {
        if (u == t) return f;
        for (int& i = it[u]; i < (int)g[u].size(); i++) {
            int id = g[u][i], v = e[id].to;
            if (e[id].cap > 0 && level[v] == level[u] + 1) {
                ll d = dfs(v, t, min(f, e[id].cap));
                if (d > 0) {
                    e[id].cap -= d;
                    e[id ^ 1].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    ll maxflow(int s, int t) {
        ll fl = 0;
        while (bfs(s, t)) {
            fill(all(it), 0);
            while (ll f = dfs(s, t, LLONG_MAX))
                fl += f;
        }
        return fl;
    }
};
```
- **Min cut** = max flow; the source side is the set reachable from `s` in the residual graph (`cap > 0` after the final BFS).
- Modelling: split vertex `v` into `v_in -> v_out` with the vertex capacity; undirected edge = two directed edges; lower bounds via circulation; project selection / closure = min cut; bipartite matching = unit-capacity flow; edge-disjoint paths = flow with capacity 1; vertex-disjoint paths = vertex splitting.

### Bipartite matching
```cpp
vector<int> mt;
vector<bool> vis;  // mt[v] = left partner of right v, init -1
bool kuhn(int u) {
    for (int v : g[u])
        if (!vis[v]) {
            vis[v] = true;
            if (mt[v] < 0 || kuhn(mt[v])) {
                mt[v] = u;
                return true;
            }
        }
    return false;
}
// for each left u: fill(all(vis), false); res += kuhn(u);       O(VE)
```
- Hopcroft–Karp (BFS layers + DFS) is O(E√V); or use Dinic on the unit network.
- **König:** in bipartite graphs, min vertex cover = max matching; max independent set = V − matching; min path cover of a DAG = V − max matching (split nodes).
- Hall's condition: a perfect matching exists iff every subset S of the left has `|N(S)| ≥ |S|`. Weighted assignment → Hungarian O(n³) or MCMF.

### Min-cost max-flow (SPFA, small/medium graphs)
```cpp
struct MCMF {
    struct E {
        int to, cap;
        ll cost;
    };
    vector<E> e;
    vector<vector<int>> g;
    int n;
    MCMF(int n) : g(n), n(n) {}
    void add(int u, int v, int c, ll w) {
        g[u].push_back(e.size());
        e.push_back({v, c, w});
        g[v].push_back(e.size());
        e.push_back({u, 0, -w});
    }
    pair<ll, ll> run(int s, int t) {  // {flow, cost}
        ll flow = 0, cost = 0;
        while (true) {
            vector<ll> d(n, LINF);
            vector<int> pe(n, -1);
            vector<bool> inq(n, false);
            queue<int> q;
            d[s] = 0;
            q.push(s);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                inq[u] = false;
                for (int id : g[u]) {
                    int v = e[id].to;
                    if (e[id].cap > 0 && d[u] + e[id].cost < d[v]) {
                        d[v] = d[u] + e[id].cost;
                        pe[v] = id;
                        if (!inq[v]) {
                            inq[v] = true;
                            q.push(v);
                        }
                    }
                }
            }
            if (d[t] == LINF) break;
            int f = INT_MAX;
            for (int v = t; v != s; v = e[pe[v] ^ 1].to)
                f = min(f, e[pe[v]].cap);
            for (int v = t; v != s; v = e[pe[v] ^ 1].to) {
                e[pe[v]].cap -= f;
                e[pe[v] ^ 1].cap += f;
            }
            flow += f;
            cost += (ll)f * d[t];
        }
        return {flow, cost};
    }
};
```

### 2-SAT
```cpp
// variable i: node 2i = (x_i true), 2i+1 = (x_i false);  literal l, negation l^1
void addClause(int a, int b) {
    g[a ^ 1].push_back(b);
    g[b ^ 1].push_back(a);
}  // (a OR b)
// implication a -> b is clause (!a OR b). Run SCC (Tarjan above).
// unsatisfiable iff comp[2i] == comp[2i+1] for some i
// assignment: x_i = true iff comp[2i] < comp[2i+1]      (Tarjan ids are reverse-topological)
```

### Eulerian path / circuit (Hierholzer)
```cpp
vector<int> path;
void euler(int u) {
    while (!adj[u].empty()) {
        int v = adj[u].back();
        adj[u].pop_back();
        euler(v);
    }
    path.push_back(u);
}
// directed: reverse(path) at the end. Exists iff in==out for all (circuit) or exactly one start (out-in=1) and one end (in-out=1), edges connected.
// undirected: store edge ids and a used[] array so each edge is removed from both ends; condition: 0 or 2 odd-degree vertices.
```

### DSU with rollback (offline dynamic connectivity)
```cpp
struct RDSU {
    vector<int> p, sz;
    vector<pair<int, int>> hist;
    RDSU(int n) : p(n), sz(n, 1) { iota(all(p), 0); }
    int find(int x) {
        while (p[x] != x)
            x = p[x];
        return x;
    }  // no path compression
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            hist.push_back({-1, -1});
            return false;
        }
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        hist.push_back({a, b});
        return true;
    }
    void rollback() {
        pair<int, int> h = hist.back();
        hist.pop_back();
        if (h.first < 0) return;
        sz[h.first] -= sz[h.second];
        p[h.second] = h.second;
    }
};
```
Use with a segment tree over time: put each edge in the O(log T) nodes covering its lifetime, DFS the tree, `unite` on entry, `rollback` on exit.

### More graph facts
- **Shortest paths:** dense Dijkstra O(V²) with an array; A* needs a consistent heuristic; shortest path with at most k edges = k rounds of Bellman–Ford; count shortest paths by adding counts on ties; Dijkstra on state `(node, extra)`; second-shortest path: keep two best distances per node.
- **MST:** Kruskal sorted edges + DSU; Prim with heap; MST is unique if all weights distinct; for each non-MST edge, the max edge on the MST path between its ends (binary lifting) gives the second-best MST and "can this edge be in some MST" queries.
- **Topological order DP:** longest path, number of paths, earliest/latest start (critical path).
- **Functional graph** (each node one out-edge): rho shape; find cycle + tail with Floyd or colors; binary lifting for `f^k(x)`.
- **Tree facts:** diameter via 2 BFS; center = middle of diameter; sum of distances by rerooting; centroid minimizes max subtree; number of labelled trees = n^(n−2) (Cayley); Prüfer sequence encodes a tree; Kirchhoff's matrix-tree theorem counts spanning trees (determinant of Laplacian minor).
- **Planar/special:** bipartite iff no odd cycle; DAG has a topological order iff acyclic; tournament has a Hamiltonian path; Euler's formula V−E+F=2 (planar).
- **Grids:** BFS/0-1 BFS/Dijkstra on `(r,c,state)`; flood fill with DSU; "shortest path with k wall breaks" → state `(r,c,k)`.

[⬆️ Back to contents](#contents)

---

<a id="a2"></a>
## 🏗️ 2. Advanced data structures

| Need | Structure |
|---|---|
| Range update + range query | lazy segment tree, or 2 BITs (range add / range sum) |
| k-th smallest in range / count ≤ x in range | persistent segtree, wavelet tree, merge-sort tree |
| Offline range queries, add/remove cheap | Mo's algorithm |
| Insert/erase at position, split/merge sequences | implicit treap |
| `dp[i] = min(dp[j] + m_j * x_i)` | convex hull trick / Li Chao tree |
| Static range min/max, O(1) | sparse table |
| Range min with point updates | segment tree |
| Order statistics + dynamic set | BIT (compressed) or pb_ds tree |
| Connectivity only grows | DSU |
| Sliding window min/max | monotonic deque |
| 2D point queries | 2D BIT, offline sweep + BIT, k-d tree |

### Lazy segment tree (range add, range sum)
```cpp
struct Seg {
    int n;
    vector<ll> t, lz;
    Seg(int n) : n(n), t(4 * n), lz(4 * n) {}
    void apply(int x, int l, int r, ll v) {
        t[x] += v * (r - l + 1);
        lz[x] += v;
    }
    void push(int x, int l, int r) {
        if (lz[x]) {
            int m = (l + r) / 2;
            apply(2 * x, l, m, lz[x]);
            apply(2 * x + 1, m + 1, r, lz[x]);
            lz[x] = 0;
        }
    }
    void upd(int x, int l, int r, int a, int b, ll v) {
        if (a > r || b < l) return;
        if (a <= l && r <= b) {
            apply(x, l, r, v);
            return;
        }
        push(x, l, r);
        int m = (l + r) / 2;
        upd(2 * x, l, m, a, b, v);
        upd(2 * x + 1, m + 1, r, a, b, v);
        t[x] = t[2 * x] + t[2 * x + 1];
    }
    ll qry(int x, int l, int r, int a, int b) {
        if (a > r || b < l) return 0;
        if (a <= l && r <= b) return t[x];
        push(x, l, r);
        int m = (l + r) / 2;
        return qry(2 * x, l, m, a, b) + qry(2 * x + 1, m + 1, r, a, b);
    }
};  // usage: Seg s(n); s.upd(1, 0, n-1, l, r, v); s.qry(1, 0, n-1, l, r);
```
- **Design recipe:** node stores aggregate; lazy tag; `apply(tag)` updates the aggregate **and** composes with the existing tag; `push` sends the tag to children; always `push` before recursing and recompute (`pull`) after.
- Range **assign** + sum: tag = "assigned value or none"; assignment overrides older add tags. Range add + **max/min**: `t[x] += v`, `lz[x] += v` (no length factor). Combined affine tag `x -> a*x + b` composes as `(a2*a1, a2*b1 + b2)`.
- Segment tree **descent**: find first index with value ≥ x by comparing the left child's max.
- **Segment tree beats**: range `a[i] = min(a[i], x)` with sum queries (amortized O(log n)), keep max, second max and count of max.
- **Merge-sort tree**: each node holds the sorted segment; count elements ≤ x in a range in O(log² n).

### Fenwick (BIT) extras
```cpp
// k-th smallest (1-indexed) in a BIT of counts, O(log n): smallest pos with prefix(pos) >= k
int kth(ll k) {
    int pos = 0;
    for (int pw = 1 << LOG; pw; pw >>= 1)
        if (pos + pw <= n && t[pos + pw] < k) {
            pos += pw;
            k -= t[pos];
        }
    return pos + 1;
}

// range add, range sum with two BITs (B1, B2 are ordinary point-update/prefix-sum BITs)
void rangeAdd(int l, int r, ll v) {
    B1.add(l, v);
    B1.add(r + 1, -v);
    B2.add(l, v * (l - 1));
    B2.add(r + 1, -v * r);
}
ll prefix(int i) {
    return B1.sum(i) * i - B2.sum(i);
}  // range sum [l,r] = prefix(r) - prefix(l-1)

// 2D BIT: for (i=x; i<=n; i+=i&-i) for (j=y; j<=m; j+=j&-j) t[i][j] += v;   query mirrors it
```
- Inversions: iterate left to right, `count of earlier elements > a[i]` = `i - prefix(rank[a[i]])`.

### Persistent segment tree (k-th smallest in range [l, r])
```cpp
struct Node {
    int l, r, cnt;
} t[N * 20];
int root[N], tot = 0;  // node 0 = null (l=r=cnt=0)
int upd(int p, int l, int r, int pos) {
    int x = ++tot;
    t[x] = t[p];
    t[x].cnt++;
    if (l < r) {
        int m = (l + r) / 2;
        if (pos <= m)
            t[x].l = upd(t[p].l, l, m, pos);
        else
            t[x].r = upd(t[p].r, m + 1, r, pos);
    }
    return x;
}
int kth(int a, int b, int l, int r, int k) {  // versions a = root[l-1], b = root[r]
    if (l == r) return l;
    int m = (l + r) / 2, c = t[t[b].l].cnt - t[t[a].l].cnt;
    return k <= c ? kth(t[a].l, t[b].l, l, m, k) : kth(t[a].r, t[b].r, m + 1, r, k - c);
}
// compress values to 1..M; root[i] = upd(root[i-1], 1, M, rank(a[i])); answer = kth(root[l-1], root[r], 1, M, k)
```
- Memory = O(n log n) nodes. Also gives "number of distinct values in range" (with last-occurrence trick) and rollbacks/time travel.

### Mo's algorithm — O((n + q)√n)
```cpp
struct Q {
    int l, r, id;
};
int B;  // block = max(1, n / sqrt(q))  (or sqrt(n))
sort(all(qs), [&](const Q& a, const Q& b) {
    int x = a.l / B, y = b.l / B;
    if (x != y) return x < y;
    return (x & 1) ? a.r > b.r : a.r < b.r;
});
int cl = 0, cr = -1;
for (auto& q : qs) {
    while (cl > q.l)
        add(--cl);
    while (cr < q.r)
        add(++cr);
    while (cl < q.l)
        rem(cl++);
    while (cr > q.r)
        rem(cr--);
    ans[q.id] = cur;
}
```
- Always extend before shrinking (as above) so counts never go negative.
- Variants: Mo on trees (Euler tour with first/last occurrence), Mo with updates O(n^(5/3)), Mo with rollback.

### Sqrt decomposition
Blocks of size ~√n store aggregates; update/query = partial blocks (O(√n) elementwise) + full blocks (O(√n)). Use when operations are hard to compose lazily (e.g. "count elements equal to x in range with updates", "rebuild every √q operations").

### Convex hull trick / Li Chao tree
```cpp
struct Line {
    ll m, b;
    ll eval(ll x) const { return m * x + b; }
};
// Li Chao over sorted query points xs[0..M-1]; minimum of lines
vector<ll> xs;
vector<Line>
    t(4 * M,
      Line{0, LINF});  // call add(1, 0, M-1, line); query(1, 0, M-1, index of x in xs)
void add(int x, int l, int r, Line nw) {
    int m = (l + r) / 2;
    bool lef = nw.eval(xs[l]) < t[x].eval(xs[l]),
         mid = nw.eval(xs[m]) < t[x].eval(xs[m]);
    if (mid) swap(t[x], nw);
    if (l == r) return;
    if (lef != mid)
        add(2 * x, l, m, nw);
    else
        add(2 * x + 1, m + 1, r, nw);
}
ll query(int x, int l, int r, int i) {
    ll res = t[x].eval(xs[i]);
    if (l == r) return res;
    int m = (l + r) / 2;
    return min(res, i <= m ? query(2 * x, l, m, i) : query(2 * x + 1, m + 1, r, i));
}
```
- Monotone slopes + monotone queries → deque-based CHT in O(n). Otherwise Li Chao (O(log n) per op) — easiest and safest.
- Watch overflow: slopes × x can exceed 1e18 → use `__int128` in comparisons.

### Implicit treap (sequence with split/merge)
```cpp
mt19937 rng(712367);
struct T {
    int val, pri, sz = 1;
    T *l = nullptr, *r = nullptr;
    T(int v) : val(v), pri(rng()) {}
};
int sz(T* t) { return t ? t->sz : 0; }
void upd(T* t) {
    if (t) t->sz = 1 + sz(t->l) + sz(t->r);
}
void split(T* t, int k, T*& a, T*& b) {  // a = first k elements, b = rest
    if (!t) {
        a = b = nullptr;
        return;
    }
    if (sz(t->l) < k) {
        split(t->r, k - sz(t->l) - 1, t->r, b);
        a = t;
    } else {
        split(t->l, k, a, t->l);
        b = t;
    }
    upd(t);
}
void merge(T*& t, T* a, T* b) {
    if (!a || !b) {
        t = a ? a : b;
        return;
    }
    if (a->pri > b->pri) {
        merge(a->r, a->r, b);
        t = a;
    } else {
        merge(b->l, a, b->l);
        t = b;
    }
    upd(t);
}
// insert at pos: split(root,pos,a,b); merge(a,a,new T(v)); merge(root,a,b)
// range reverse/add: add lazy flags (push down in split/merge); erase [l,r): split twice, drop middle
```
- Treap expected depth O(log n). Alternatives: splay, AVL/red-black (what `std::set` uses), skip list.

### Other structures worth knowing
- **Sparse table** (static RMQ O(1)); **disjoint sparse table** for non-idempotent ops (sum) in O(1).
- **Wavelet tree / matrix:** k-th smallest, count ≤ x in range, in O(log σ) without persistence.
- **k-d tree:** nearest-neighbour / range counting in 2D.
- **Trie variants:** binary trie for max-XOR / XOR with `k`; persistent trie for XOR on ranges.
- **Binary indexed tree of sorted vectors / offline BIT** for 2D dominance counting (sort by x, BIT on y).
- **Monotone queue / stack** for DP optimizations and window problems.
- **Link-cut tree / Euler-tour tree:** dynamic forests (rare).
- **LRU/LFU caches:** hash map + doubly linked list (see section [8](#a8)).
- **Fibonacci heap:** O(1) amortized decrease-key; theoretical Dijkstra O(E + V log V); in practice binary heap wins.
- **Bloom filter, count-min sketch, HyperLogLog:** probabilistic membership/frequency/cardinality (section [7](#a7)).

[⬆️ Back to contents](#contents)

---

<a id="a3"></a>
## 🧠 3. Advanced dynamic programming

> [!TIP]
> Before coding any DP write: **state meaning**, **transition**, **order**, **base**, **answer**, **complexity = states × transitions**. If it is too slow, ask which of these can be shrunk (monotonicity, prefix sums, data structure, convexity).

### Digit DP (count numbers in [0, N] with a property)
```cpp
string s;
int n;
ll memo[20][200][2][2];  // pos, state (e.g. digit sum), tight, started
ll go(int pos, int st, bool tight, bool started) {
    if (pos == n) return ok(st);  // property check on final state
    ll& r = memo[pos][st][tight][started];
    if (r != -1) return r;
    ll res = 0;
    int lim = tight ? s[pos] - '0' : 9;
    for (int d = 0; d <= lim; d++) {
        bool ns = started || d > 0;
        res += go(pos + 1, ns ? next(st, d) : st, tight && d == lim, ns);
    }
    return r = res;
}
// count in [L, R] = f(R) - f(L-1) ; reset memo (-1) per bound if state depends on the bound; for fixed property memo[tight=0] can be shared
```
- Typical states: digit sum mod k, last digit, count of a digit, "no two adjacent equal", bitmask of used digits.

### Bitmask DP
```cpp
// TSP / Hamiltonian cycle O(2^n n^2)
vector<vector<ll>> dp(1 << n, vector<ll>(n, LINF));
dp[1][0] = 0;
for (int m = 1; m < (1 << n); m++)
    for (int last = 0; last < n; last++)
        if (dp[m][last] < LINF)
            for (int nx = 0; nx < n; nx++)
                if (!(m >> nx & 1))
                    dp[m | 1 << nx][nx] =
                        min(dp[m | 1 << nx][nx], dp[m][last] + d[last][nx]);
// answer = min over last of dp[full][last] + d[last][0]
```
- **Assignment / matching DP:** `dp[mask]` = best after assigning the first `popcount(mask)` people to jobs in `mask` — O(2^n n).
- **Subset of subsets (3^n):** `for (s = m; s; s = (s-1) & m)`, e.g. partition into groups.
- **Broken-profile (plug) DP:** tile a grid row by row; state = mask of the last m cells.

### SOS DP (sum over subsets) O(n 2^n)
```cpp
for (int i = 0; i < B; i++)
    for (int m = 0; m < (1 << B); m++)
        if (m >> i & 1) f[m] += f[m ^ (1 << i)];  // f[m] = sum of f over subsets of m
// supersets: if (!(m >> i & 1)) f[m] += f[m | (1 << i)];       inverse (subtract) to undo
```
- Used for AND/OR convolution, "count pairs with `a & b == 0`" (subset sums over complements), counting by mask.

### Tree DP and rerooting
```cpp
// sum of distances from every node to all others
void dfs1(int u, int p) {
    sz[u] = 1;
    down[u] = 0;
    for (int v : g[u])
        if (v != p) {
            dfs1(v, u);
            sz[u] += sz[v];
            down[u] += down[v] + sz[v];
        }
}
void dfs2(int u, int p) {
    for (int v : g[u])
        if (v != p) {
            ans[v] = ans[u] - sz[v] + (n - sz[v]);
            dfs2(v, u);
        }
}
// ans[root] = down[root]
```
- **Rerooting pattern:** `down[u]` = answer from the subtree; `up[v]` = answer from the rest of the tree, computed from the parent using prefix/suffix aggregates over siblings (needed when the combine op is not invertible, e.g. max).
- Max independent set, vertex cover, matching on trees: `dp[u][0/1]`. Tree knapsack `dp[u][k]` merges children in O(n²) total (size-bounded).
- Small-to-large merging (DSU on tree / "sack"): keep heavy child's structure, insert light children → O(n log n) insertions.

### DP optimizations
| Technique | Applies to | Condition | Cost |
|---|---|---|---|
| Prefix sums / sliding window min | range transitions | window moves monotonically | O(n) |
| Monotone deque | `dp[i] = min(dp[j]) + c`, j in window | – | O(n) |
| Convex hull trick / Li Chao | `dp[i] = min_j(dp[j] + b_j * a_i)` | lines | O(n log n) |
| Divide & conquer | `dp[i][j] = min_k(dp[i-1][k] + C(k, j))` | `opt(j)` monotone | O(k n log n) |
| Knuth | `dp[l][r] = min_k(dp[l][k] + dp[k][r]) + C(l,r)` | `opt[l][r-1] ≤ opt[l][r] ≤ opt[l+1][r]` | O(n²) |
| Aliens trick (lambda) | "exactly k" with convex cost in k | convex/concave in k | O(n log C) |
| Bitset | boolean knapsack / reachability | – | /64 |
| Matrix exponentiation | linear recurrences, huge n | fixed linear transition | O(k³ log n) |

```cpp
// divide & conquer optimization: cur[mid] = min over k in [optl, optr] of prev[k] + cost(k+1, mid)
void solve(int l, int r, int optl, int optr) {
    if (l > r) return;
    int mid = (l + r) / 2;
    pair<ll, int> best = {LINF, -1};
    for (int k = optl; k <= min(mid - 1, optr); k++)
        best = min(best, make_pair(prev_[k] + cost(k + 1, mid), k));
    cur[mid] = best.first;
    solve(l, mid - 1, optl, best.second);
    solve(mid + 1, r, best.second, optr);
}
// Knuth: for len in 2..n for l: r=l+len-1; for k in [opt[l][r-1], opt[l+1][r]]  (store opt)
```

### More classic DP
- **Knapsack variants:** bounded (binary splitting of counts → 0/1; or monotone queue per residue), multiple constraints (2D), exactly-W vs at-most-W, value-indexed DP when weights are huge (`dp[value] = min weight`), meet-in-the-middle for n ≤ 40.
- **LIS variants:** count LIS (BIT of (len, count)), longest bitonic, Russian-doll (sort by w asc, h desc), LIS on a tree/DAG.
- **Interval DP:** matrix chain, burst balloons, palindrome partitioning, stone merging (Knuth), optimal BST, "remove boxes".
- **Grid DP:** paths with obstacles, min-cost, DP with state `(r, c, k)`, max square/rectangle (histogram stack), cherry pickup (two walkers, `dp[step][c1][c2]`).
- **String DP:** LCS/edit distance/LPS, distinct subsequences, wildcard/regex matching (`dp[i][j]` over pattern/text), shortest common supersequence, interleaving strings.
- **Counting DP:** partitions of n (`p(n)` via coin DP or pentagonal numbers), Stirling numbers (second kind: `S(n,k)=k·S(n−1,k)+S(n−1,k−1)`), Catalan DP, derangements, inclusion–exclusion + DP.
- **Probability / expectation DP:** `E[s] = 1 + Σ p·E[s']` (states solved in reverse topological order; cycles → linear system); coupon collector `n·H_n`; gambler's ruin.
- **Game DP:** `win[state] = exists move to a losing state` (see section [6](#a6)).
- **DP on SCC-condensed DAG / dag of states** and **bitset DP** for reachability/path counts mod 2.
- **State compression:** rolling arrays, only last k layers, hash states when sparse, "DP over sorted order then greedy exchange".
- **Reconstruction:** store `par[state]`/choice, or re-derive by walking backwards checking `dp[s] == dp[prev] + cost`.
- **Recognising DP:** optimal substructure + overlapping subproblems; counting/min/max over choices; constraints small in one dimension (n ≤ 20 → mask, values ≤ 1e3–1e5 → value index, length ≤ 5000 → O(n²)).

[⬆️ Back to contents](#contents)

---

<a id="a4"></a>
## 🔤 4. Advanced string algorithms

| Problem | Tool |
|---|---|
| Find pattern in text | KMP / Z / hashing |
| Many patterns at once | Aho–Corasick |
| Palindromic substrings | Manacher (O(n)), palindromic tree (eertree) |
| Compare/locate arbitrary substrings | hashing, suffix array + LCP |
| Count distinct substrings / k-th substring | suffix array + LCP, suffix automaton |
| Longest common substring of 2+ strings | suffix automaton, suffix array on `s#t` |
| Smallest rotation | Booth / Duval / min over `s+s` |
| Lyndon factorization | Duval |
| Longest common prefix queries | LCP + sparse table |

### Hashing (mod 2^61−1 with random base)
```cpp
typedef unsigned long long ull;
const ull MODH = (1ULL << 61) - 1;
ull mulm(ull a, ull b) {
    __uint128_t t = (__uint128_t)a * b;
    ull r = (ull)(t & MODH) + (ull)(t >> 61);
    return r >= MODH ? r - MODH : r;
}
ull B = chrono::steady_clock::now().time_since_epoch().count() % (MODH - 1000) + 500;
vector<ull> H(n + 1), PW(n + 1);  // PW[0]=1
for (int i = 0; i < n; i++) {
    H[i + 1] = (mulm(H[i], B) + s[i]) % MODH;
    PW[i + 1] = mulm(PW[i], B);
}
ull get(int l, int r) {
    return (H[r] + MODH - mulm(H[l], PW[r - l])) % MODH;
}  // hash of s[l, r)
```
- Single 32-bit mods collide with ~1e5 strings (birthday); use 2^61−1 or double mod and a random base. Never use a fixed base against hacks.

### Z-function & KMP applications
- Pattern occurrences: `z` on `P + '#' + T` or prefix function. Count occurrences of each prefix: `cnt[pi[i]]++`, then propagate `cnt[pi[i-1]] += cnt[i]` backwards.
- Period `p` of string: `n - pi[n-1]` if it divides `n` (else period = n). All borders: follow `pi[n-1], pi[pi[n-1]-1], …`.
- KMP automaton `aut[i][c]` for DP over "pattern matched so far" (counting strings avoiding/containing a pattern).

### Manacher (all palindromes in O(n))
```cpp
vector<int> d1(n),
    d2(n);  // d1[i]: # odd palindromes centred at i; d2[i]: # even palindromes centred between i-1 and i
for (int i = 0, l = 0, r = -1; i < n; i++) {
    int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
    while (i - k >= 0 && i + k < n && s[i - k] == s[i + k])
        k++;
    d1[i] = k--;
    if (i + k > r) {
        l = i - k;
        r = i + k;
    }
}
for (int i = 0, l = 0, r = -1; i < n; i++) {
    int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
    while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k])
        k++;
    d2[i] = k--;
    if (i + k > r) {
        l = i - k - 1;
        r = i + k;
    }
}
```
- Longest palindromic substring = max over `2*d1[i]-1` and `2*d2[i]`; count of palindromic substrings = `Σ d1 + Σ d2`.

### Suffix array (O(n log² n)) + LCP (Kasai)
```cpp
vector<int> sa(n), rk(n), tmp(n);
iota(all(sa), 0);
for (int i = 0; i < n; i++)
    rk[i] = s[i];
for (int k = 1;; k <<= 1) {
    auto cmp = [&](int a, int b) {
        if (rk[a] != rk[b]) return rk[a] < rk[b];
        int ra = a + k < n ? rk[a + k] : -1, rb = b + k < n ? rk[b + k] : -1;
        return ra < rb;
    };
    sort(all(sa), cmp);
    tmp[sa[0]] = 0;
    for (int i = 1; i < n; i++)
        tmp[sa[i]] = tmp[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
    rk = tmp;
    if (rk[sa[n - 1]] == n - 1) break;
}
vector<int> lcp(n, 0);  // lcp[r] = LCP(sa[r-1], sa[r])
for (int i = 0, h = 0; i < n; i++) {
    if (rk[i] > 0) {
        int j = sa[rk[i] - 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h])
            h++;
        lcp[rk[i]] = h;
        if (h) h--;
    } else
        h = 0;
}
```
- Distinct substrings = `n(n+1)/2 − Σ lcp`. LCP of arbitrary suffixes = min over `lcp` range (sparse table). Pattern search = binary search over `sa`. Longest repeated substring = max `lcp`. For O(n log n) use radix sort doubling; for O(n) SA-IS.

### Aho–Corasick
```cpp
struct AC {
    int nx[MAXN][26], fail[MAXN], cnt[MAXN], tot = 1;  // root = 0
    void insert(const string& s) {
        int u = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!nx[u][x]) nx[u][x] = tot++;
            u = nx[u][x];
        }
        cnt[u]++;
    }
    void build() {
        queue<int> q;
        for (int x = 0; x < 26; x++)
            if (nx[0][x]) q.push(nx[0][x]);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cnt[u] +=
                cnt[fail[u]];  // occurrences of patterns ending at u (via suffix links)
            for (int x = 0; x < 26; x++) {
                int v = nx[u][x];
                if (v) {
                    fail[v] = nx[fail[u]][x];
                    q.push(v);
                } else
                    nx[u][x] = nx[fail[u]][x];
            }
        }
    }
    // run text: u = nx[u][c]; total += cnt[u];
};
```
- The completed automaton is a DFA: use it for DP "count strings of length L avoiding all forbidden words".

### Suffix automaton
```cpp
struct State {
    int len, link;
    int next[26];
    State() { memset(next, -1, sizeof next); }
};
vector<State> st(1);
int last = 0;  // st[0].link = -1
void init() {
    st[0].len = 0;
    st[0].link = -1;
}
void extend(char ch) {
    int c = ch - 'a', cur = st.size();
    st.emplace_back();
    st[cur].len = st[last].len + 1;
    int p = last;
    while (p != -1 && st[p].next[c] == -1) {
        st[p].next[c] = cur;
        p = st[p].link;
    }
    if (p == -1)
        st[cur].link = 0;
    else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len)
            st[cur].link = q;
        else {
            int cl = st.size();
            st.push_back(st[q]);
            st[cl].len = st[p].len + 1;
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = cl;
                p = st[p].link;
            }
            st[q].link = st[cur].link = cl;
        }
    }
    last = cur;
}
```
- ≤ 2n states. Distinct substrings = `Σ (len[v] − len[link[v]])`. Occurrence counts: mark non-clone states with 1, push counts up suffix links in decreasing `len` order. Longest common substring of `s,t`: build on `s`, walk `t`.

### Other string tools
- **Duval (Lyndon factorization)** / **minimum rotation:** least `i` such that `s+s` starting at `i` is minimal.
- **Palindromic tree (eertree):** O(n) distinct palindromes with counts.
- **Trie + XOR/binary trie**, **radix tree**, **suffix tree** (via SAM of reversed string).
- **Lexicographic problems:** next permutation, k-th permutation by factorial number system, de Bruijn sequences.
- **Edit-distance variants:** banded DP O(nk), Hirschberg for linear-space LCS.

[⬆️ Back to contents](#contents)

---

<a id="a5"></a>
## 🔐 5. Advanced math

### Primality & factorization for 64-bit numbers
```cpp
typedef unsigned long long u64;
typedef __uint128_t u128;
u64 mulm(u64 a, u64 b, u64 m) { return (u128)a * b % m; }
u64 powm(u64 a, u64 e, u64 m) {
    u64 r = 1;
    a %= m;
    for (; e; e >>= 1, a = mulm(a, a, m))
        if (e & 1) r = mulm(r, a, m);
    return r;
}
bool isPrime(u64 n) {  // deterministic Miller–Rabin for n < 3.3e24
    if (n < 2) return false;
    static const u64 bs[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (u64 p : bs)
        if (n % p == 0) return n == p;
    u64 d = n - 1;
    int s = 0;
    while (!(d & 1))
        d >>= 1, s++;
    for (u64 a : bs) {
        u64 x = powm(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int i = 1; i < s; i++) {
            x = mulm(x, x, n);
            if (x == n - 1) {
                comp = false;
                break;
            }
        }
        if (comp) return false;
    }
    return true;
}
u64 rho(u64 n) {  // a nontrivial factor of composite n, ~O(n^(1/4))
    if (n % 2 == 0) return 2;
    static mt19937_64 rng(712367);
    while (true) {
        u64 x = rng() % (n - 2) + 2, y = x, c = rng() % (n - 1) + 1, d = 1;
        while (d == 1) {
            x = (mulm(x, x, n) + c) % n;
            y = (mulm(y, y, n) + c) % n;
            y = (mulm(y, y, n) + c) % n;
            d = __gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}
void factor(u64 n, vector<u64>& out) {
    if (n == 1) return;
    if (isPrime(n)) {
        out.push_back(n);
        return;
    }
    u64 d = rho(n);
    factor(d, out);
    factor(n / d, out);
}
```

### Linear sieve: primes, φ, μ
```cpp
vector<int> primes, mu(N + 1), phi(N + 1);
vector<bool> comp(N + 1);
mu[1] = phi[1] = 1;
for (int i = 2; i <= N; i++) {
    if (!comp[i]) {
        primes.push_back(i);
        mu[i] = -1;
        phi[i] = i - 1;
    }
    for (int p : primes) {
        if ((ll)i * p > N) break;
        comp[i * p] = true;
        if (i % p == 0) {
            mu[i * p] = 0;
            phi[i * p] = phi[i] * p;
            break;
        }
        mu[i * p] = -mu[i];
        phi[i * p] = phi[i] * (p - 1);
    }
}
```
- Every composite is marked exactly once (by its smallest prime) → O(N). Also gives smallest-prime-factor `lp[i]=p` for O(log n) factorization.
- **Möbius inversion:** `g(n) = Σ_{d|n} f(d)  ⇔  f(n) = Σ_{d|n} μ(n/d) g(d)`. Count coprime pairs `(a,b) ≤ n`: `Σ_d μ(d)·⌊n/d⌋²`. Dirichlet prefix sums / Dirichlet convolution for multiplicative functions.
- Σ_{d|n} φ(d) = n. Number of integers ≤ n coprime to n is φ(n). Multiplicative functions are determined by prime powers.
- **Sum over floor values** `Σ ⌊n/i⌋ · f(i)`: O(√n) blocks with prefix sums of `f`.

### Modular arithmetic extras
```cpp
ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    ll g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}
ll invmod(ll a, ll m) {
    ll x, y;
    ll g = exgcd(a, m, x, y);
    return g == 1 ? (x % m + m) % m : -1;
}  // any m with gcd(a,m)=1
// CRT: x ≡ a1 (mod m1), x ≡ a2 (mod m2), general moduli; returns false if inconsistent
bool crt(ll a1, ll m1, ll a2, ll m2, ll& a, ll& m) {
    ll x, y, g = exgcd(m1, m2, x, y);
    if ((a2 - a1) % g) return false;
    ll m2g = m2 / g;
    ll l = m1 / g * m2;
    __int128 t = (__int128)((a2 - a1) / g) * x % m2g;
    if (t < 0) t += m2g;
    a = (ll)(((__int128)m1 * t + a1) % l);
    if (a < 0) a += l;
    m = l;
    return true;
}
```
- Inverses of `1..n` mod prime in O(n): `inv[i] = (MOD - MOD/i) * inv[MOD % i] % MOD`.
- **Discrete log** (baby-step giant-step) O(√m); **primitive root** test via prime factors of `φ(m)`; quadratic residues via Euler's criterion `a^((p−1)/2)`.
- **Wilson:** `(p−1)! ≡ −1 (mod p)`. **Lucas:** `C(n,r) mod p = ∏ C(n_i, r_i)` over base-p digits. For prime power or composite mod: factor and CRT.
- Garner's algorithm to reconstruct big numbers from residues.

### NTT — polynomial multiplication mod 998244353 in O(n log n)
```cpp
const int MODN = 998244353, GEN = 3;
void ntt(vector<ll>& a, bool inv) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        ll w = power(GEN, (MODN - 1) / len, MODN);
        if (inv) w = power(w, MODN - 2, MODN);
        for (int i = 0; i < n; i += len) {
            ll wn = 1;
            for (int j = 0; j < len / 2; j++) {
                ll u = a[i + j], v = a[i + j + len / 2] * wn % MODN;
                a[i + j] = (u + v) % MODN;
                a[i + j + len / 2] = (u - v + MODN) % MODN;
                wn = wn * w % MODN;
            }
        }
    }
    if (inv) {
        ll ni = power(n, MODN - 2, MODN);
        for (auto& x : a)
            x = x * ni % MODN;
    }
}
vector<ll> multiply(vector<ll> a, vector<ll> b) {
    int need = a.size() + b.size() - 1, n = 1;
    while (n < need)
        n <<= 1;
    a.resize(n);
    b.resize(n);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < n; i++)
        a[i] = a[i] * b[i] % MODN;
    ntt(a, true);
    a.resize(need);
    return a;
}
```
- Uses: counting convolutions (subset-sum counts, string matching with wildcards, big-number multiplication), polynomial power/exp/inverse (advanced), `1e9+7` needs three-mod NTT or FFT with splitting. FFT with `complex<double>` for approximate results (watch precision above ~1e6 magnitude).
- Related transforms: **XOR convolution (Walsh–Hadamard)**, OR/AND via SOS (section [3](#a3)).

### Linear algebra
```cpp
ll detMod(vector<vector<ll>> a) {  // determinant mod prime MOD
    int n = a.size();
    ll res = 1;
    for (int c = 0; c < n; c++) {
        int p = -1;
        for (int r = c; r < n; r++)
            if (a[r][c]) {
                p = r;
                break;
            }
        if (p < 0) return 0;
        if (p != c) {
            swap(a[p], a[c]);
            res = (MOD - res) % MOD;
        }
        res = res * a[c][c] % MOD;
        ll iv = power(a[c][c], MOD - 2);
        for (int r = c + 1; r < n; r++) {
            ll f = a[r][c] * iv % MOD;
            for (int k = c; k < n; k++)
                a[r][k] = ((a[r][k] - f * a[c][k]) % MOD + MOD) % MOD;
        }
    }
    return res;
}
```
- Gaussian elimination solves `Ax=b` (over a field); real numbers: partial pivoting with `eps`; over **GF(2)** use `bitset` rows and XOR → O(n³/64) (lights-out, XOR-basis problems).
- **XOR basis (linear basis):** insert numbers by highest bit; max XOR subset = greedy over the basis; rank = size.
- **Matrix exponentiation:** `Mat mul(A,B)` triple loop with `% MOD` and skip zeros; `F(n)` via `[[1,1],[1,0]]^n`; linear recurrence of order k in O(k³ log n); Kitamasa/Berlekamp–Massey for faster. Counting walks of length k in a graph = `A^k`.
- Kirchhoff: number of spanning trees = any cofactor of Laplacian `L = D − A` (use `detMod`).

### Combinatorics & counting toolbox
| Object | Formula / idea |
|---|---|
| Permutations with repetition | `n! / (c1! c2! …)` |
| Circular arrangements | `(n−1)!` (rotation-equivalent) |
| Derangements | `D(n) = (n−1)(D(n−1)+D(n−2))`, ≈ `n!/e` |
| Catalan | `C(2n,n)/(n+1)`; ballot/reflection principle for lattice paths staying below a line |
| Stirling 2nd kind `S(n,k)` | partitions into k nonempty sets; `S(n,k)=k S(n−1,k)+S(n−1,k−1)` |
| Stirling 1st kind | permutations with k cycles |
| Bell numbers | all set partitions |
| Partitions p(n) | coin-DP O(n√n) via pentagonal numbers |
| Surjections onto k | `Σ (−1)^i C(k,i)(k−i)^n` (inclusion–exclusion) |
| Burnside / Pólya | #orbits = average fixed points of group elements (necklaces, colourings up to symmetry) |
| Inclusion–exclusion | count "none of properties" = `Σ (−1)^|S| |∩ S|`; mask over ≤ 20 properties |
| Probability/expectation | linearity of expectation (no independence needed), indicator variables, expected value DP |
| Lagrange interpolation | recover degree-d polynomial from d+1 values; sums of powers `Σ i^k` |
| Hockey-stick / Vandermonde | `Σ_{i≤n} C(i,k) = C(n+1,k+1)`, `Σ C(a,i)C(b,k−i)=C(a+b,k)` |
- Counting tricks: fix the structure then multiply; count complement; bijection to lattice paths; "contribution of each element/pair/position"; symmetry (pair up equal-probability events).

### Other facts worth remembering
- Sum of divisors / count of divisors are multiplicative; max divisor count for n ≤ 1e18 is 103680; n ≤ 1e9: 1344.
- Pigeonhole in "subarray sum divisible by n" (a multiple always exists among prefixes mod n).
- `gcd` of Fibonacci: `gcd(F_a, F_b) = F_gcd(a,b)`; `gcd(a^n − 1, a^m − 1) = a^gcd(n,m) − 1`.
- Zeckendorf representation (every integer = sum of non-consecutive Fibonacci numbers); balanced ternary.
- Pick's theorem and polygon area in section [6](#a6).

[⬆️ Back to contents](#contents)

---

<a id="a6"></a>
## ♟️ 6. Game theory & computational geometry

### Combinatorial games
- **Win/lose DP:** a state is *winning* if some move reaches a *losing* state; losing if all moves reach winning states (terminal = lose under normal play). Memoize on state; reduce state by symmetry.
- **Nim:** XOR of pile sizes ≠ 0 ⇒ first player wins; move = reduce a pile so the XOR becomes 0. **Misère Nim:** same unless every pile is 1 (then parity of piles flips).
- **Sprague–Grundy:** `g(state) = mex{ g(next) }`; a sum of independent games has Grundy value = XOR of components; position is losing iff the total is 0.
```cpp
int grundy(int s) {
    if (memo[s] != -1) return memo[s];
    set<int> seen;
    for (int nx : moves(s))
        seen.insert(grundy(nx));
    int g = 0;
    while (seen.count(g))
        g++;
    return memo[s] = g;
}
```
- Known results: **subtraction game** (take 1..k): lose iff `n % (k+1) == 0`; **Wythoff** (two piles) uses the golden ratio; **staircase Nim** = Nim on odd-indexed steps; **Kayles**, **Grundy's game** have periodic patterns — compute small values, find the pattern.
- **Minimax with alpha-beta** for two-player zero-sum search; **minimax DP on intervals** (take from either end of an array: `dp[l][r] = max(a[l] - dp[l+1][r], a[r] - dp[l][r-1])`).
- Parity/strategy-stealing/symmetry arguments often give O(1) answers: first player wins by mirroring or reducing to Nim.

### Geometry toolkit (use integers when possible)
```cpp
struct P {
    ll x, y;
    P operator-(P o) const { return {x - o.x, y - o.y}; }
    P operator+(P o) const { return {x + o.x, y + o.y}; }
    ll cross(P o) const { return x * o.y - y * o.x; }
    ll dot(P o) const { return x * o.x + y * o.y; }
    bool operator<(const P& o) const { return x != o.x ? x < o.x : y < o.y; }
    bool operator==(const P& o) const { return x == o.x && y == o.y; }
};
ll cross(P a, P b, P c) {
    return (b - a).cross(c - a);
}  // > 0: c is left of a->b (CCW); 0: collinear
int sgn(ll v) { return (v > 0) - (v < 0); }
```
- **Convex hull (Andrew's monotone chain), O(n log n):**
```cpp
vector<P> hull(vector<P> p) {
    sort(all(p));
    p.erase(unique(all(p)), p.end());
    int n = p.size(), k = 0;
    if (n < 3) return p;
    vector<P> h(2 * n);
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(h[k - 2], h[k - 1], p[i]) <= 0)
            k--;
        h[k++] = p[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && cross(h[k - 2], h[k - 1], p[i]) <= 0)
            k--;
        h[k++] = p[i];
    }
    h.resize(k - 1);
    return h;  // CCW, no collinear points (use < 0 to keep them)
}
```
- **Polygon area (shoelace):** `2·A = |Σ cross(p[i], p[i+1])|`. **Pick's theorem:** `A = I + B/2 − 1` (I interior, B boundary lattice points; `B = Σ gcd(|dx|,|dy|)`).
- **Segment intersection:** segments `ab`, `cd` intersect iff orientations differ (`sgn(cross(a,b,c)) != sgn(cross(a,b,d))` and similarly the other pair) or an endpoint lies on the other segment (collinear + bounding-box check).
- **Point in polygon:** ray casting (count crossings; handle vertices on the ray by half-open rule) or winding number; for a convex polygon, binary search by angle in O(log n).
- **Point–line distance:** `|cross| / length`; **angle sort** by half-plane then cross product (no `atan2` needed).
- **Closest pair** O(n log n): divide & conquer (strip of width δ checks ≤ 7 neighbours) or sweep with ordered set; **rotating calipers:** diameter, width of a convex polygon; **half-plane intersection**; **Minkowski sum**; **sweep line** for segment intersections / union of rectangles (segment tree on y).
- **Doubles:** compare with `eps` (1e-9 relative scale), use `atan2l`, `hypot`, `long double`; avoid `acos` of values slightly outside [−1,1] (clamp). Prefer integer cross/dot whenever coordinates are integral.
- Typical combos: sort by angle + two pointers (count triangles containing the origin), convex hull trick = lower envelope of lines, Pick + shoelace for lattice polygons.

[⬆️ Back to contents](#contents)

---

<a id="a7"></a>
## 📐 7. Theory, analysis & proofs

### Asymptotics & recurrences
- `O` upper, `Ω` lower, `Θ` tight; `o`, `ω` strict. Growth order: `1 < log n < √n < n < n log n < n² < n³ < 2ⁿ < n! < nⁿ`. `log(n!) = Θ(n log n)`; `Σ 1/i = Θ(log n)`; `Σ i = Θ(n²)`; `Σ 2^i = Θ(2^k)`.
- **Master theorem** for `T(n) = a·T(n/b) + f(n)`, let `c = log_b a`:
  1. `f(n) = O(n^(c−ε))` ⇒ `T = Θ(n^c)`
  2. `f(n) = Θ(n^c · log^k n)` ⇒ `T = Θ(n^c · log^(k+1) n)`
  3. `f(n) = Ω(n^(c+ε))` and regularity ⇒ `T = Θ(f(n))`
  
  Examples: merge sort `2T(n/2)+n → n log n`; binary search `T(n/2)+1 → log n`; Karatsuba `3T(n/2)+n → n^1.585`; Strassen `7T(n/2)+n² → n^2.81`; quickselect average `T(n/2)+n → n`.
- When the master theorem does not apply: recursion tree, substitution (guess + induction), Akra–Bazzi; `T(n)=T(n−1)+n → n²`; `T(n)=2T(n−1)+1 → 2ⁿ`; `T(n)=T(n/3)+T(2n/3)+n → n log n`.
- **Lower bounds:** comparison sorting needs `Ω(n log n)` (decision tree has `n!` leaves); searching a sorted list `Ω(log n)`; merging two sorted lists `Ω(n)`; finding max needs `n−1` comparisons; element distinctness `Ω(n log n)` in the comparison model.

### Amortized analysis
- **Aggregate:** total cost of n ops / n. **Accounting:** charge credits to cheap ops. **Potential method:** amortized = actual + ΔΦ.
- Classics: dynamic array doubling → O(1) amortized push; binary counter increment → O(1); monotonic stack/deque → each element pushed/popped once; DSU with rank + path compression → O(α(n)); splay tree O(log n); two-stack queue O(1); Fibonacci heap `decrease-key` O(1); hash table rehash.
- Amortized ≠ per-operation worst case; persistent structures with rollback can break amortization (use non-amortized DS there).

### Sorting reference
| Algorithm | Best | Average | Worst | Space | Stable | Notes |
|---|---|---|---|---|---|---|
| Bubble / insertion / selection | n / n / n² | n² | n² | O(1) | B,I yes; S no | insertion great for tiny/nearly sorted |
| Merge sort | n log n | n log n | n log n | O(n) | yes | external sort; counts inversions |
| Quick sort | n log n | n log n | n² | O(log n) | no | random pivot / median of 3; introsort in `std::sort` |
| Heap sort | n log n | n log n | n log n | O(1) | no | in-place worst-case guarantee |
| Counting / radix / bucket | n+k | n+k | n+k (bucket n²) | O(n+k) | yes | non-comparison; integer keys |
| `std::sort` | | n log n | n log n | | no | introsort; `stable_sort` is merge |
- **Quickselect** (k-th element): partition around a random pivot, recurse into one side → expected O(n); median-of-medians gives worst-case O(n).
- **Partition schemes:** Lomuto (simple), Hoare (fewer swaps), 3-way (many duplicates, Dutch flag).

### P, NP and friends
- **P:** solvable in polynomial time. **NP:** a given solution can be verified in poly time. **NP-hard:** at least as hard as every problem in NP. **NP-complete:** NP-hard and in NP. A poly-time algorithm for any NP-complete problem would show P = NP (open).
- **Reduction** `A ≤p B`: map instances of A to B in poly time so that answers agree; to prove B hard, reduce a known NP-hard A to B.
- Canonical NP-complete problems: **SAT, 3-SAT, Clique, Independent Set, Vertex Cover, Hamiltonian Cycle/Path, TSP (decision), Subset Sum, Partition, 0/1 Knapsack (decision), Graph k-colouring (k ≥ 3), Set Cover, 3-Dimensional Matching, Longest Path, Bin Packing, Steiner Tree.**
- **Looks hard but easy:** 2-SAT (SCC), Euler path vs Hamiltonian path, shortest vs longest simple path (DAG longest path is easy), min cut vs max cut, bipartite matching vs 3D matching, 2-colouring vs 3-colouring, MST vs degree-constrained spanning tree, LP.
- **Pseudo-polynomial:** knapsack/subset-sum run in `O(nW)` — polynomial in the *value* of `W`, exponential in its bit length.
- **Coping with hardness:** exponential DP over subsets (`2^n n`), meet-in-the-middle (`2^(n/2)`), branch & bound, backtracking with pruning, FPT algorithms (exponential only in a parameter k), approximation, heuristics (simulated annealing, hill-climbing — popular in heuristic contests).
- **Approximation:** vertex cover 2-approx (take both ends of a maximal matching); metric TSP 2-approx (MST preorder) and 1.5 (Christofides); set cover `H_n ≈ ln n` greedy; knapsack FPTAS; load balancing (LPT) 4/3; bin packing first-fit-decreasing 11/9 OPT + 6/9.

### Greedy: proof techniques
- **Exchange argument:** take any optimal solution, swap to agree with greedy's first choice without getting worse; induct.
- **Greedy stays ahead:** show after each step greedy's partial solution is at least as good as any other's.
- **Structural/matroid:** if feasible sets form a matroid (hereditary + exchange property), the greedy by weight is optimal (MST, scheduling unit jobs with deadlines/profits).
- Always test with small random brute force; if a counter-example exists, think DP.

### Randomized & probabilistic techniques
- **Las Vegas** (always correct, random time: randomized quicksort/quickselect, treap) vs **Monte Carlo** (fast, small error probability: Miller–Rabin, hashing, Freivalds' matrix check, Karger's min-cut).
- **Universal hashing** `h(x)=((ax+b) mod p) mod m`; random bases for string hashing; **birthday bound:** collisions appear after ~√M items.
- **Bloom filter:** m bits, k hash functions, n items → false-positive ≈ `(1 − e^(−kn/m))^k`, optimal `k = (m/n) ln 2`; no false negatives, no deletions. **Count-min sketch** (frequency overestimates), **HyperLogLog** (cardinality in a few KB), **reservoir sampling** (keep item i with prob `k/i`), **skip list** (expected O(log n) search).
- **Randomization in contests:** shuffle before sorting adversarial input, random pivot, random base, `mt19937` seeded by time; randomized checks (`A·B·x == C·x`), hill-climbing/annealing for heuristic problems.
- **Probability reminders:** `E[X+Y]=E[X]+E[Y]` always; `E[X] = Σ P(X ≥ k)` for non-negative integer X; geometric distribution mean `1/p`; coupon collector `n H_n`; Markov / Chebyshev tails; union bound.

### Data-structure trade-offs (interview favourites)
| Question | Answer |
|---|---|
| Hash map vs balanced BST | O(1) avg vs O(log n) worst; BST gives order, range, successor |
| Array vs linked list | cache locality + O(1) random access vs O(1) insert at known node |
| Heap vs sorted array | heap: O(log n) insert + O(1) peek; sorted: O(n) insert |
| BFS vs DFS | BFS: shortest path unweighted, level order, O(width) memory; DFS: O(depth) memory, topo/cycle/components |
| Dijkstra vs Bellman–Ford | non-negative O((V+E)log V) vs negatives O(VE) + negative-cycle detection |
| Kruskal vs Prim | sparse/edge list vs dense/adjacency |
| Quick vs merge sort | in-place + cache vs stable + guaranteed n log n |
| DP vs greedy | DP explores all options w/ substructure; greedy needs a proof |
| Recursion vs iteration | stack depth/space vs explicit stack |
| B-tree vs BST | wide nodes minimise disk reads |
| Stack overflow causes | deep recursion (~1e5 frames), huge local arrays |

### Memory & machine-level notes
- Cache: row-major traversal of 2D arrays is much faster than column-major; `vector` beats `list`; struct-of-arrays for hot loops; avoid `unordered_map` of tiny objects in tight loops.
- Alignment/padding in `struct`; sizeof(`bool`) = 1 byte; `bitset` = 1 bit.
- Virtual calls, `std::function`, `shared_ptr` add overhead; prefer `reserve`, `emplace_back`, `reinterpret` I/O with `fread` when input is 1e6+ numbers.
- Time/space limit awareness: 256MB ≈ 6e7 ints; recursion depth ≈ 1e5–1e6 depending on frame size.

[⬆️ Back to contents](#contents)

---

<a id="a8"></a>
## 💼 8. Interview & placement toolkit

### How to run a coding round (works for OAs and live interviews)
1. **Clarify:** input size/range, duplicates, negatives, empty input, sorted or not, one query or many, memory limit, output format.
2. **Examples:** walk a small case and one edge case by hand; state expected output.
3. **Brute force first** (say its complexity), then find the bottleneck and name the technique that removes it.
4. **State the plan** in 2–3 sentences before typing (data structures + invariant).
5. **Code cleanly:** meaningful names, small helper functions, no premature optimisation.
6. **Test aloud** on the examples + edge cases; trace the code, not what you intended.
7. **State time & space complexity**, then offer improvements/trade-offs.
8. If stuck: simplify (smaller constraints), draw, ask for a hint, write the brute force and optimise, don't go silent.

> [!TIP]
> **Online assessments:** read all problems first, solve easiest first, keep ~10% time for re-checking, aim for partial credit (brute force passes some tests), keep a pre-written template with fast I/O, watch for integer overflow and `long long`, and check whether the function signature or `main` + stdin is expected.

### Interview-style definitions (LeetCode)
```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```

### Linked list classics
```cpp
ListNode* reverseList(ListNode* h) {
    ListNode* prev = nullptr;
    while (h) {
        ListNode* nx = h->next;
        h->next = prev;
        prev = h;
        h = nx;
    }
    return prev;
}
ListNode* detectCycle(ListNode* h) {  // start of cycle
    ListNode *s = h, *f = h;
    while (f && f->next) {
        s = s->next;
        f = f->next->next;
        if (s == f) {
            s = h;
            while (s != f) {
                s = s->next;
                f = f->next;
            }
            return s;
        }
    }
    return nullptr;
}
ListNode* mergeTwo(ListNode* a, ListNode* b) {
    ListNode dummy(0), *t = &dummy;
    while (a && b) {
        if (a->val < b->val) {
            t->next = a;
            a = a->next;
        } else {
            t->next = b;
            b = b->next;
        }
        t = t->next;
    }
    t->next = a ? a : b;
    return dummy.next;
}
ListNode* middle(ListNode* h) {
    ListNode *s = h, *f = h;
    while (f && f->next) {
        s = s->next;
        f = f->next->next;
    }
    return s;
}  // second middle for even length
ListNode* removeNthFromEnd(ListNode* h, int n) {
    ListNode d(0);
    d.next = h;
    ListNode *a = &d, *b = &d;
    while (n--)
        b = b->next;
    while (b->next) {
        a = a->next;
        b = b->next;
    }
    a->next = a->next->next;
    return d.next;
}
```
- Reverse in k-groups, palindrome list (reverse second half), add two numbers, copy list with random pointer (interleave nodes), intersection of two lists (two pointers switching heads), sort list (merge sort), reorder list, flatten multilevel list.

### Binary tree classics
```cpp
vector<int> inorderIter(TreeNode* r) {
    vector<int> out;
    stack<TreeNode*> st;
    TreeNode* cur = r;
    while (cur || !st.empty()) {
        while (cur) {
            st.push(cur);
            cur = cur->left;
        }
        cur = st.top();
        st.pop();
        out.push_back(cur->val);
        cur = cur->right;
    }
    return out;
}
vector<vector<int>> levelOrder(TreeNode* r) {
    vector<vector<int>> res;
    if (!r) return res;
    queue<TreeNode*> q;
    q.push(r);
    while (!q.empty()) {
        int k = q.size();
        res.push_back({});
        while (k--) {
            TreeNode* u = q.front();
            q.pop();
            res.back().push_back(u->val);
            if (u->left) q.push(u->left);
            if (u->right) q.push(u->right);
        }
    }
    return res;
}
TreeNode* lca(TreeNode* r, TreeNode* p, TreeNode* q) {
    if (!r || r == p || r == q) return r;
    TreeNode* l = lca(r->left, p, q);
    TreeNode* rr = lca(r->right, p, q);
    return l && rr ? r : (l ? l : rr);
}
bool isValidBST(TreeNode* r, long long lo = LLONG_MIN, long long hi = LLONG_MAX) {
    if (!r) return true;
    if (r->val <= lo || r->val >= hi) return false;
    return isValidBST(r->left, lo, r->val) && isValidBST(r->right, r->val, hi);
}
int maxPathSum(TreeNode* r, int& best) {
    if (!r) return 0;
    int l = max(0, maxPathSum(r->left, best)), rr = max(0, maxPathSum(r->right, best));
    best = max(best, l + rr + r->val);
    return max(l, rr) + r->val;
}  // best init INT_MIN
```
- Also: diameter, balanced check, symmetric tree, path sum I/II/III (prefix sums on root paths), build tree from preorder+inorder, serialize/deserialize (preorder with `#`), flatten to linked list, right-side view, zigzag order, BST iterator, kth smallest in BST, morris traversal, successor in BST, recover swapped BST nodes.

### Design problems
```cpp
// LRU cache: O(1) get/put with list + unordered_map
class LRUCache {
    int cap;
    list<pair<int, int>> lst;
    unordered_map<int, list<pair<int, int>>::iterator> mp;

public:
    LRUCache(int c) : cap(c) {}
    int get(int k) {
        auto it = mp.find(k);
        if (it == mp.end()) return -1;
        lst.splice(lst.begin(), lst, it->second);
        return it->second->second;
    }
    void put(int k, int v) {
        auto it = mp.find(k);
        if (it != mp.end()) {
            it->second->second = v;
            lst.splice(lst.begin(), lst, it->second);
            return;
        }
        if ((int)lst.size() == cap) {
            mp.erase(lst.back().first);
            lst.pop_back();
        }
        lst.emplace_front(k, v);
        mp[k] = lst.begin();
    }
};
// Min stack: stack of (value, min so far)
struct MinStack {
    vector<pair<int, int>> s;
    void push(int x) { s.push_back({x, s.empty() ? x : min(x, s.back().second)}); }
    void pop() { s.pop_back(); }
    int top() { return s.back().first; }
    int getMin() { return s.back().second; }
};
```
- **LFU:** map `key → (value, freq)` + map `freq → list of keys` + `minFreq`. **Trie:** insert/search/prefix ([section 17](notes.md#s17)). **Two stacks → queue**, **queue via two stacks amortized O(1)**, **median of stream (two heaps)**, **randomized set (vector + index map, swap-with-last erase)**, **time-based key-value store (`map<time,value>` + `upper_bound`)**, **rate limiter (sliding window/token bucket)**, **iterator design**, **hit counter**, **Twitter feed (merge k lists by time)**, **autocomplete (trie + top-k)**.

### Sorting implementations (often asked verbatim)
```cpp
void mergeSort(vector<int>& a, vector<int>& tmp, int l, int r) {  // [l, r)
    if (r - l < 2) return;
    int m = (l + r) / 2;
    mergeSort(a, tmp, l, m);
    mergeSort(a, tmp, m, r);
    merge(a.begin() + l, a.begin() + m, a.begin() + m, a.begin() + r, tmp.begin() + l);
    copy(tmp.begin() + l, tmp.begin() + r, a.begin() + l);
}
int partitionLomuto(vector<int>& a, int l, int r) {
    swap(a[l + rand() % (r - l + 1)], a[r]);
    int p = a[r], i = l;
    for (int j = l; j < r; j++)
        if (a[j] < p) swap(a[i++], a[j]);
    swap(a[i], a[r]);
    return i;
}
void quickSort(vector<int>& a, int l, int r) {
    if (l >= r) return;
    int p = partitionLomuto(a, l, r);
    quickSort(a, l, p - 1);
    quickSort(a, p + 1, r);
}
int quickselect(vector<int> a, int k) {  // k-th smallest, 0-indexed, expected O(n)
    int l = 0, r = a.size() - 1;
    while (true) {
        int p = partitionLomuto(a, l, r);
        if (p == k) return a[p];
        if (p < k)
            l = p + 1;
        else
            r = p - 1;
    }
}
void heapify(vector<int>& a, int n, int i) {
    int big = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && a[l] > a[big]) big = l;
    if (r < n && a[r] > a[big]) big = r;
    if (big != i) {
        swap(a[i], a[big]);
        heapify(a, n, big);
    }
}
void heapSort(vector<int>& a) {
    int n = a.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(a, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        heapify(a, i, 0);
    }
}
```
- Binary search variants (first/last occurrence, rotated array, search in 2D matrix, sqrt, peak), Dutch flag (sort 0/1/2), merge intervals, next permutation, rotate array (three reversals), kth largest (heap/quickselect), majority element (Boyer–Moore), product except self (prefix/suffix), trapping rain water, container with most water, spiral matrix, set matrix zeroes, search a 2D sorted matrix (staircase search from corner).

### Pattern → typical interview problems
| Pattern | Problems |
|---|---|
| Hash map / counting | Two Sum, Group Anagrams, Subarray Sum Equals K, Longest Consecutive Sequence, Isomorphic Strings |
| Two pointers | 3Sum, Container With Most Water, Remove Duplicates, Sort Colors, Trapping Rain Water |
| Sliding window | Longest Substring Without Repeating, Minimum Window Substring, Permutation in String, Sliding Window Maximum, Max Consecutive Ones III |
| Stack / monotonic | Valid Parentheses, Daily Temperatures, Next Greater Element, Largest Rectangle in Histogram, Decode String |
| Binary search | Search in Rotated Array, Find Peak, Koko Eating Bananas, Split Array Largest Sum, Median of Two Sorted Arrays |
| Linked list | Reverse List, Cycle II, Merge k Lists, Reorder List, Copy List with Random Pointer |
| Tree DFS/BFS | Max Depth, Level Order, LCA, Validate BST, Serialize/Deserialize, Max Path Sum |
| Heap | Kth Largest, Top K Frequent, Merge k Sorted Lists, Find Median from Data Stream, Task Scheduler |
| Backtracking | Subsets, Permutations, Combination Sum, N-Queens, Word Search, Sudoku Solver |
| Graph | Number of Islands, Clone Graph, Course Schedule, Word Ladder, Network Delay Time, Alien Dictionary |
| DSU | Redundant Connection, Accounts Merge, Number of Provinces |
| DP 1D/2D | Climbing Stairs, House Robber, Coin Change, LIS, Longest Palindromic Substring, Unique Paths, Edit Distance, Partition Equal Subset Sum |
| Intervals | Merge Intervals, Insert Interval, Meeting Rooms II, Non-overlapping Intervals |
| Bit manipulation | Single Number I/II/III, Counting Bits, Reverse Bits, Missing Number, Sum of Two Integers |
| Trie | Implement Trie, Word Search II, Design Add and Search Words |
| Math | Pow(x, n), Sqrt(x), Happy Number, Count Primes, Fizz Buzz variants |

> [!NOTE]
> Placement tests often also include **aptitude, OS, DBMS (SQL), computer networks, OOP and system design basics**. Those are outside this file; keep a separate sheet for them (SQL joins/group-by/window functions, normalisation, ACID, process vs thread, deadlock conditions, paging, TCP vs UDP, OOP pillars, SOLID).

[⬆️ Back to contents](#contents)

---

<a id="a9"></a>
## 🗺️ 9. Practice roadmap & resources

> [!NOTE]
> Problem names are written from memory of CSES, LeetCode and Codeforces; search them by name (numbers/URLs change). A few CSES titles may be slightly off, so check the site's category lists. Aim to solve each category in order; revisit unsolved ones after a week.

### Study plan (adjust to your time)
| Phase | Focus | Goal |
|---|---|---|
| 1 (weeks 1–2) | STL, complexity, arrays/strings, hashing, sorting, binary search, two pointers, prefix sums | solve easy/medium confidently in 20 min |
| 2 (weeks 3–4) | Recursion/backtracking, stacks/queues/heaps, linked lists, trees, BFS/DFS | classic interview set (Blind 75 / NeetCode 150) |
| 3 (weeks 5–7) | DP (1D/2D, knapsack, LIS/LCS, intervals, bitmask), greedy, graphs (Dijkstra, DSU, MST, topo) | CSES DP + Graph sections |
| 4 (weeks 8–10) | Segment tree/BIT, number theory/combinatorics, strings (KMP/hash), trees (LCA, rerooting) | CSES Range Queries + Tree + Math |
| 5 (weeks 11+) | Advanced: flows, SCC/2-SAT, HLD/centroid, suffix structures, FFT, geometry, game theory | Codeforces Div 2 D/E, ICPC regional sets |
| Always | 1 virtual contest/week, upsolve every problem you failed, keep an error log | rating growth |

### Where to practice
- **CSES Problem Set** (all fundamentals, in order): Introductory, Sorting & Searching, Dynamic Programming, Graph Algorithms, Range Queries, Tree Algorithms, Mathematics, String Algorithms, Geometry, Advanced Techniques, Sliding Window, Counting Problems.
- **LeetCode:** Blind 75 → NeetCode 150 → company-tagged lists for placements; weekly + biweekly contests.
- **Codeforces:** Div 3/4 for speed, Div 2 A–D for rating; filter by tag and rating (800 → 1600 → 2000); **AtCoder** ABC/ARC for clean problems; **CodeChef** Starters; **USACO Guide** (structured curriculum); **cp-algorithms.com** (reference for every algorithm here); **Codeforces EDU** (segment tree, binary search, suffix structures step-by-step); **Kattis/ICPC archives** for team contests; **HackerRank/HackerEarth** for OA-style tests.
- **Books:** *Competitive Programmer's Handbook* (Laaksonen, free), *Introduction to Algorithms* (CLRS), *Algorithms* (Dasgupta–Papadimitriou–Vazirani, short), *Algorithm Design* (Kleinberg–Tardos), *Cracking the Coding Interview*, *Guide to Competitive Programming*, *Competitive Programming 4* (Halim).

### CSES checklist by section (names as on the site)
- **Sorting & Searching:** Distinct Numbers, Apartments, Ferris Wheel, Concert Tickets, Restaurant Customers, Movie Festival, Sum of Two Values, Maximum Subarray Sum, Stick Lengths, Missing Coin Sum, Collecting Numbers, Playlist, Towers, Traffic Lights, Josephus Problem I, Nested Ranges Check, Room Allocation, Factory Machines, Tasks and Deadlines, Reading Books, Sum of Three/Four Values, Nearest Smaller Values, Subarray Sums I/II, Subarray Divisibility, Array Division, Movie Festival II, Maximum Subarray Sum II.
- **Dynamic Programming:** Dice Combinations, Minimizing Coins, Coin Combinations I/II, Removing Digits, Grid Paths, Book Shop, Array Description, Edit Distance, Rectangle Cutting, Money Sums, Removal Game, Two Sets II, Increasing Subsequence, Projects, Elevator Rides, Counting Tilings, Counting Numbers.
- **Graph Algorithms:** Counting Rooms, Labyrinth, Building Roads, Message Route, Building Teams, Round Trip, Monsters, Shortest Routes I/II, High Score, Flight Discount, Cycle Finding, Flight Routes, Round Trip II, Course Schedule I/II, Longest Flight Route, Game Routes, Investigation, Planets Queries I/II, Planets Cycles, Road Reparation, Road Construction, Flight Routes Check, Planets and Kingdoms, Giant Pizza, Coin Collector, Mail Delivery, De Bruijn Sequence, Teleporters Path, Hamiltonian Flights, Knight's Tour, Download Speed, Police Chase, School Dance, Distinct Routes.
- **Range Queries:** Static/Dynamic Range Sum/Min Queries, Range Xor, Range Update Queries, Forest Queries (I/II), Hotel Queries, List Removals, Salary Queries, Prefix Sum Queries, Pizzeria Queries, Visible Buildings Queries, Range Interval Queries, Subarray Sum Queries, Distinct Values Queries, Increasing Array Queries.
- **Tree Algorithms:** Subordinates, Tree Matching, Tree Diameter, Tree Distances I/II, Company Queries I/II, Distance Queries, Counting Paths, Subtree Queries, Path Queries I/II, Distinct Colors, Finding a Centroid, Fixed-Length Paths I/II.
- **Mathematics:** Exponentiation I/II, Counting Divisors, Common Divisors, Sum of Divisors, Binomial Coefficients, Creating Strings II, Distributing Apples, Christmas Party, Josephus Queries, Nim Game I/II, Stair Game, Grundy's Game, Another Game, Matrix Exponentiation problems (Fibonacci Numbers, Graph Paths I/II).
- **String Algorithms:** Word Combinations, String Matching, Finding Borders, Finding Periods, Minimal Rotation, Longest Palindrome, Required Substring, Palindrome Queries, Distinct Substrings, Distinct Subsequences, Repeating Substring, String Transform.
- **Geometry:** Point Location Test, Line Segment Intersection, Polygon Area, Point in Polygon, Convex Hull, Minimum Euclidean Distance, Lines and Queries, Maximum Manhattan Distances.
- **Advanced Techniques:** Meet in the Middle, Hamming Distance, Corner Subgrid Check, Coin Grid, Mex Grid Queries, Counting Bits, Sum of Divisors II, Triangle Number Sums.

### LeetCode must-do (≈ NeetCode 150 spine)
**Arrays/hash:** Two Sum, Contains Duplicate, Group Anagrams, Top K Frequent Elements, Product of Array Except Self, Longest Consecutive Sequence, Subarray Sum Equals K. **Two pointers/window:** 3Sum, Container With Most Water, Trapping Rain Water, Longest Substring Without Repeating Characters, Longest Repeating Character Replacement, Minimum Window Substring, Sliding Window Maximum. **Stack:** Valid Parentheses, Min Stack, Daily Temperatures, Largest Rectangle in Histogram. **Binary search:** Search in Rotated Sorted Array, Koko Eating Bananas, Median of Two Sorted Arrays, Time Based Key-Value Store. **Linked list:** Reverse Linked List, Merge Two/K Sorted Lists, Linked List Cycle II, LRU Cache, Reverse Nodes in k-Group. **Trees:** Invert Tree, Max Depth, Diameter, LCA, Validate BST, Kth Smallest in BST, Construct from Preorder+Inorder, Serialize/Deserialize, Binary Tree Maximum Path Sum. **Tries/Heaps:** Implement Trie, Word Search II, Kth Largest Element, Task Scheduler, Find Median from Data Stream. **Backtracking:** Subsets, Combination Sum, Permutations, Word Search, N-Queens, Palindrome Partitioning. **Graphs:** Number of Islands, Clone Graph, Pacific Atlantic Water Flow, Course Schedule I/II, Redundant Connection, Word Ladder, Network Delay Time, Cheapest Flights Within K Stops, Alien Dictionary, Critical Connections in a Network, Reconstruct Itinerary, Min Cost to Connect All Points, Swim in Rising Water. **DP:** Climbing Stairs, House Robber I/II, Longest Palindromic Substring, Decode Ways, Coin Change, Maximum Product Subarray, Word Break, Longest Increasing Subsequence, Partition Equal Subset Sum, Unique Paths, Longest Common Subsequence, Edit Distance, Best Time to Buy and Sell Stock (all variants), Burst Balloons, Regular Expression Matching, Distinct Subsequences, Target Sum. **Intervals/greedy:** Merge Intervals, Insert Interval, Non-overlapping Intervals, Meeting Rooms II, Jump Game I/II, Gas Station, Hand of Straights. **Math/bits:** Pow(x, n), Single Number, Number of 1 Bits, Counting Bits, Reverse Bits, Missing Number, Sum of Two Integers.

### Codeforces tag ladder (find by tag + rating filter)
`implementation / math 800–1200` → `greedy, sortings, constructive 1000–1500` → `binary search, two pointers, dp 1200–1700` → `graphs, dfs/bfs, shortest paths, dsu, trees 1400–1900` → `number theory, combinatorics, bitmasks, strings (hashing/KMP) 1600–2100` → `data structures (segtree, BIT) 1700–2200` → `flows, SCC, HLD, FFT, suffix array, geometry 2000+`.

### Habits that move the needle
- **Upsolve** every contest problem you could not finish; read editorials after a real attempt (30–60 min).
- Maintain an **error log**: topic, why it failed (overflow? off-by-one? wrong greedy?), fix.
- **Write the template and core algorithms from memory** (DSU, Dijkstra, segment tree, BIT, sieve, mod pow) until typing them takes < 5 min.
- Stress-test ([section 21](notes.md#s21)) instead of guessing; learn to generate small random inputs.
- Re-solve the same problem after 1–2 weeks without notes (spaced repetition).
- Time-box: 25 min thinking → hint → 25 min → editorial.

[⬆️ Back to contents](#contents)
