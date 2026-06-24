#include <bits/stdc++.h>

#define pii pair<int, int>

using namespace std;

const int Nmax = 2e5 + 5, Qmax = 2e5 + 5;

int N, Q;

struct NOTE{
    int u, v, szu;
};

struct DSU{
    vector<int> p, sz, val;
    vector<NOTE> history;
    int n;
    DSU(int N){
        n = N;
        p.resize(n + 2);
        sz.assign(n + 2, 1);
        val.assign(n + 2, 0);
        for(int i = 0; i <= n; i++) p[i] = i;
    }

    pii finds(int u){
        int dis = 0;
        while(p[u] != u){
            dis ^= val[u];
            u = p[u];
        }
        return {u, dis};
    }
    bool add(int u, int v){
        auto [pu, du] = finds(u);
        auto [pv, dv] = finds(v);

        if(pu == pv){
            if(du == dv) return false;
            return true;
        }
        if(sz[pu] < sz[pv]) {
            swap(pu, pv);
            swap(du, dv);
        }
        history.push_back({pu, pv, sz[pu]});
        p[pv] = pu;
        sz[pu] += sz[pv];
        val[pv] = du ^ dv ^ 1;
        return true;
    }
    void rollback(int target){
        while(history.size() > target){
            auto [u, v, szu] = history.back();
            history.pop_back();

            sz[u] = szu;
            p[v] = v;
            val[v] = 0;
        }
    }
};

vector<pii> st[Qmax << 2];

void addEdge(int id, int l, int r, int L, int R, const pii &val){
    if(l > R || r < L) return;
    if(L <= l && r <= R){
        st[id].push_back(val);
        return;
    }
    int mid = (l + r) >> 1;
    addEdge(id << 1, l, mid, L, R, val);
    addEdge(id << 1 | 1, mid + 1, r, L, R, val);
}
bool ans[Qmax];

DSU dsu(Nmax);

void solve(int id, int l, int r){
    int time = dsu.history.size();
    for(auto [u, v]: st[id]){
        if(!dsu.add(u, v)) {
            for(int i = l; i <= r; i++) ans[i] = false;
            dsu.rollback(time);
            return;
        }
    }
    if(l == r){
        ans[l] = true;
        dsu.rollback(time);
        return;
    }
    int mid = (l + r) >> 1;

    solve(id << 1, l, mid);
    solve(id << 1 | 1, mid + 1, r);

    dsu.rollback(time);
}
void enter(){
    cin >> N >> Q;
    int u, v;
    map<pii, int> lastseen;
    for(int i = 1; i <= Q; i++){
        cin >> u >> v;
        if(u > v) swap(u, v);
        if(lastseen[{u, v}]){
            addEdge(1, 1, Q, lastseen[{u, v}], i - 1, {u, v});
            lastseen.erase({u, v});
        }
        else lastseen[{u, v}] = i;
    }
    for(auto [uv, i]: lastseen){
        addEdge(1, 1, Q, i, Q, uv);
    }
}
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    enter();
    solve(1, 1, Q);
    for(int i = 1; i <= Q; i++) cout << (ans[i] ? "YES\n" : "NO\n");
}
