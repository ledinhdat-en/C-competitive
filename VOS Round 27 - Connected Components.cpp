#include <bits/stdc++.h>

#define ll long long
#define pii pair<int, int>

using namespace std;

const int Nmax = 1e5 + 5, Qmax = 1e5 + 5;

int N, Q, ans[Qmax];

struct NOTE{
    int sz, u, v;
};

struct DSU {
    vector<int> p, sz;
    vector<NOTE> history;
    int n;
    DSU(int N){
        n = N;
        p.resize(n + 2);
        sz.assign(n + 2, 1);
        for(int i = 0; i <= n; i++) p[i] = i;
    }
    int finds(int u){
        if(p[u] == u) return u;
        return finds(p[u]);
    }
    bool add(int u, int v){
        u = finds(u), v = finds(v);
        if(u == v) return false;
        if(sz[u] < sz[v]) swap(u, v);
        history.push_back({sz[u], u, v});
        p[v] = u;
        sz[u] += sz[v];
        return true;
    }
    void rollback(int target){
        while(history.size() > target){
            auto [szu, u, v] = history.back();
            history.pop_back();
            p[v] = v;
            sz[u] = szu;
        }
    }
    bool check(int u, int v){
        return finds(u) == finds(v);
    }
};

struct QUERY{
    int c, u, v;
} q[Qmax];

vector<pii> st[Qmax << 2];

void add(int id, int l, int r, int L, int R, pii edge){
    if(l > R || r < L) return;
    if(L <= l && r <= R){
        st[id].push_back(edge);
        return;
    }
    int mid = (l + r) >> 1;
    add(id << 1, l, mid, L, R, edge);
    add(id << 1 | 1, mid + 1, r, L, R, edge);
}

DSU dsu(Nmax);
void solve(int id, int l, int r){
    int tin = dsu.history.size();

    for(auto [u, v]: st[id]){
        dsu.add(u, v);
    }

    if(l == r){
        if(q[l].c == 3){
            ans[l] = dsu.check(q[l].u, q[l].v);
        }
        dsu.rollback(tin);
        return;
    }
    int mid = (l + r) >> 1;

    solve(id << 1, l, mid);
    solve(id << 1 | 1, mid + 1, r);

    dsu.rollback(tin);
}

void enter(){
    cin >> N >> Q;
    map<pii, pii> check;
    for(int i = 1; i <= Q; i++){
        cin >> q[i].c >> q[i].u >> q[i].v;
        if(q[i].u > q[i].v) swap(q[i].u, q[i].v);
    }
    for(int i = 1; i <= Q; i++){
        auto [c, u, v] = q[i];
        if(c == 1){
            if(check.count({u, v})) check[{u, v}].second++;
            else check[{u, v}] = {i, 1};
        }
        else if(c == 2){
            if(check.count({u, v})){
                if(check[{u, v}].second == 1){
                    add(1, 1, Q, check[{u, v}].first, i - 1, {u, v});
                    check.erase({u, v});
                }
                else {
                    check[{u, v}].second--;
                }
            }
        }
    }
    for(auto [uv, i]: check){
        add(1, 1, Q, i.first, Q, uv);
    }
}
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    enter();
    solve(1, 1, Q);
    for(int i = 1; i <= Q; i++){
        if(q[i].c == 3) cout << ans[i];
    }
}
