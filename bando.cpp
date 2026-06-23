#include <bits/stdc++.h>

#define ll long long

using namespace std;

const int Nmax = 1e6 + 5, inf = 1e9;

int n, k, a[Nmax];

struct DSU {
    vector<int> p, sz;
    int n;
    DSU(int N){
        n = N;
        p.resize(n + 2);
        sz.assign(n + 2, 1);
        for(int i = 0; i <= n; i++){
            p[i] = i;
        }
    }
    int finds(int u){
        if(p[u] == u) return u;
        return p[u] = finds(p[u]);
    }
    bool add(int u, int v){
        u = finds(u), v = finds(v);
        if(u == v) return false;
        p[v] = u;
        sz[u] += sz[v];
        sz[v] = 0;
        return true;
    }
    int Sz(int u){
        return sz[finds(u)];
    }
};


void solve(){
    int l = 0, r = n;
    DSU dsu(n);
    for(int i = 1; i <= k; i++){
        int rot = dsu.finds(a[i]);
        int mid = (l + r) >> 1;
        if(rot <= mid){
            for(int j = 1; j <= rot - l; j++){
                dsu.add(rot + j, rot - j);
            }
            l = rot;
        }
        else {
            for(int j = 1; j <= r - rot; j++){
                dsu.add(rot - j, rot + j);
            }
            r = rot;
        }
    }
    cout << (r - l + 1) << '\n';
    for(int i = l; i <= r; i++){
        cout << dsu.Sz(i) << ' ';
    }

}
void enter(){
    cin >> n >> k;
    for(int i = 1; i <= k; i++){
        cin >> a[i];
    }


}
int main(){
    freopen("bando.inp", "r", stdin);
    freopen("bando.out", "w", stdout);
    enter();
    solve();


}
