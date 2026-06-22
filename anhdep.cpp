#include <bits/stdc++.h>

#define TASK "anhdep"

using namespace std;

const int Nmax = 1e5 + 5, Qmax = 1e5 + 5;

struct ITEM {
    int x, y, z;
    int type, id;
    int zcompress;
};

bool sortX(const ITEM &a, const ITEM &b){
    if(a.x != b.x) return a.x > b.x;
    return a.type < b.type;
}
bool sortY(const ITEM &a, const ITEM &b){
    return a.y > b.y;
}

struct SEGMENT {
    vector<int> st;
    int n;

    SEGMENT(int N){
        n = N;
        st.assign(n << 2, 0);
    }

    void update(int id, int l, int r, int u, int v){
        if(l == r) {
            st[id] += v;
            return;
        }
        int mid = (l + r) >> 1;
        if(u <= mid) update(id << 1, l, mid, u, v);
        else update(id << 1 | 1, mid + 1, r, u, v);
        st[id] = st[id << 1] + st[id << 1 | 1];
    }
    int query(int id, int l, int r, int L, int R){
        if(l > R || r < L) return 0;
        if(L <= l && r <= R) return st[id];
        int mid = (l + r) >> 1;
        return query(id << 1, l, mid, L, R) + query(id << 1 | 1, mid + 1, r, L, R);
    }
};

SEGMENT st(Nmax + Qmax);

vector<ITEM> items;

int N, Q, ans[Qmax];

void DnC(int l, int r){
    if(l == r) return;
    int mid = (l + r) >> 1;

    DnC(l, mid);
    DnC(mid + 1, r);

    vector<ITEM> Lphotos, Rqueries;

    for(int i = l; i <= mid; i++){
        if(items[i].type == 0) Lphotos.push_back(items[i]);
    }
    for(int i = mid + 1; i <= r; i++){
        if(items[i].type == 1) Rqueries.push_back(items[i]);
    }
    if(Lphotos.empty() || Rqueries.empty()) return;

    sort(Lphotos.begin(), Lphotos.end(), sortY);
    sort(Rqueries.begin(), Rqueries.end(), sortY);
    int i = 0;
    for(auto &q: Rqueries){
        while(i < (int)Lphotos.size() && Lphotos[i].y >= q.y){
            st.update(1, 0, Nmax + Qmax - 1, Lphotos[i].zcompress, 1);
            i++;
        }
        ans[q.id] += st.query(1, 0, Nmax + Qmax - 1, q.zcompress, Nmax + Qmax - 1);
    }
    for(int j = 0; j < i; j++){
        st.update(1, 0, Nmax + Qmax - 1, Lphotos[j].zcompress, -1);
    }
}

void compress(){
    vector<int> z;
    for(int i = 0; i < (int)items.size(); i++){
        z.push_back(items[i].z);
    }
    sort(z.begin(), z.end());
    z.erase(unique(z.begin(), z.end()), z.end());

    for(int i = 0; i < (int)items.size(); i++){
        items[i].zcompress = lower_bound(z.begin(), z.end(), items[i].z) - z.begin();
    }
}

void solve(){
    compress();

    sort(items.begin(), items.end(), sortX);

    DnC(0, items.size() - 1);

    for(int i = 0; i < Q; i++){
        cout << ans[i] << '\n';
    }
}

void enter(){
    cin >> N >> Q;
    int s, t, x, y, z;
    for(int i = 0; i < N; i++){
        cin >> s >> t;
        items.push_back({s, t, s + t, 0, -1});
    }
    for(int i = 0; i < Q; i++){
        cin >> x >> y >> z;
        items.push_back({x, y, z, 1, i});
    }
}
int main(){
    freopen(TASK".INP", "r", stdin);
    freopen(TASK".OUT", "w", stdout);

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    enter();
    solve();
}
