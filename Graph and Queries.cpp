#include <bits/stdc++.h>

#define pii pair<int, int>

using namespace std;

const int Nmax = 2e5 + 5, Mmax = 3e5 + 5, Qmax = 5e5 + 5;

int N, M, Q, A[Nmax << 1];
int par[Nmax << 1][20];
int tin[Nmax << 1], tout[Nmax << 1], pos[Nmax << 1], t = 0;
bool delEdge[Mmax];
vector<int> way[Nmax << 1];
pii edge[Mmax], q[Qmax];

struct DSU {
    vector<int> p, deadtime;
    int n;

    DSU(int N){
        n = N;
        p.resize(n * 2 + 2);
        deadtime.assign(n * 2 + 2, 0);
        for(int i = 0; i <= n * 2; i++) p[i] = i;
    }
    int finds(int u){
        if(p[u] == u) return u;
        return p[u] = finds(p[u]);
    }
    bool add(int u, int v, int timedead){
        u = finds(u), v = finds(v);
        if(u == v) return false;
        n++;
        p[u] = p[v] = n;
        way[n].push_back(v);
        way[n].push_back(u);
        deadtime[n] = timedead;
        return true;
    }
};

struct SEGMENT {
    vector<pii> st;
    int n;

    SEGMENT(int _n){
        n = _n;
        st.resize(n << 2);
    }
    void build(int id, int l, int r){
        if(l == r) {
            if(pos[l] <= N) st[id] = {A[pos[l]], l};
            else st[id] = {-1, l};
            return;
        }
        int mid = (l + r) >> 1;
        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);
        st[id] = max(st[id << 1], st[id << 1 | 1]);
    }
    void update(int id, int l, int r, int u, int v){
        if(l == r){
            st[id].first = v;
            return;
        }
        int mid = (l + r) >> 1;
        if(u <= mid) update(id << 1, l, mid, u, v);
        else update(id << 1 | 1, mid + 1, r, u, v);
        st[id] = max(st[id << 1], st[id << 1 | 1]);
    }
    pii query(int id, int l, int r, int L, int R){
        if(l > R || r < L) return {-1, -1};
        if(L <= l && r <= R) return st[id];
        int mid = (l + r) >> 1;
        return max(query(id << 1, l, mid, L, R), query(id << 1 | 1, mid + 1, r, L, R));
    }
};

void dfs(int u){
    tin[u] = ++t;
    pos[t] = u;

    for(int i = 1; i < 20; i++){
        par[u][i] = par[par[u][i - 1]][i - 1];
    }

    for(int v: way[u]){
        par[v][0] = u;
        dfs(v);
    }
    tout[u] = t;
}

int getpar(int u, int timedead, vector<int> &deadtime){
    for(int i = 19; i >= 0; i--){
        if(par[u][i] != 0 && deadtime[par[u][i]] > timedead){
            u = par[u][i];
        }
    }
    return u;
}

void solve(){
    DSU dsu(N);
    for(int i = 1; i <= M; i++){
        if(!delEdge[i]) dsu.add(edge[i].first, edge[i].second, Q + 1);
    }
    for(int i = Q; i > 0; i--){
        auto [c, u] = q[i];
        if(c == 2) dsu.add(edge[u].first, edge[u].second, i);
    }
    for(int i = 1; i <= N; i++){
        dsu.deadtime[i] = Q + 1;
    }
    for(int i = 1; i <= dsu.n; i++){
        if(dsu.p[i] == i) dfs(i);
    }

    SEGMENT st(dsu.n);
    st.build(1, 1, dsu.n);
    for(int i = 1; i <= Q; i++){
        auto [c, u] = q[i];
        if(c == 1){
            u = getpar(u, i, dsu.deadtime);
            auto [res, node] = st.query(1, 1, dsu.n, tin[u], tout[u]);
            cout << res << '\n';
            if(res > 0) st.update(1, 1, dsu.n, node, 0);
        }
    }
}
void enter(){
    cin >> N >> M >> Q;
    for(int i = 1; i <= N; i++){
        cin >> A[i];
    }
    int u, v;
    for(int i = 1; i <= M; i++){
        cin >> u >> v;
        edge[i] = {u, v};
    }
    for(int i = 1; i <= Q; i++){
        cin >> u >> v;
        q[i] = {u, v};
        if(u == 2) delEdge[v] = true;
    }
}
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    enter();
    solve();
}
