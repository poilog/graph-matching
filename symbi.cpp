#include <bits/stdc++.h>
#define oo 1005
using namespace std;

struct Graph {
    int n, m;
    int label[oo];
    vector <int> candidates[oo];
    unordered_map <int, int> D1[oo];
    unordered_map <int, int> D2[oo];
    unordered_map <int, int> neighbour[oo];
} q, g;

int m;
struct Edge {
    int opt, x, y;
} stream[oo];

struct EDCS {
    int u, v;
    int u_, v_;
};

struct EDCSs {
    vector <EDCS> e;
};

int vis[oo], dep[oo];
unordered_map<int, int>::iterator it;
unordered_map<int, int>::iterator it2;
int bfs (int s, bool opt) {
    int cnt = 0;
    memset(vis, 0, sizeof vis);
    vis[s] = ++ cnt;
    queue <int> Q;
    Q.push(s);
    dep[s] = 0;
    int ret = 0;
    while(!Q.empty()) {
        int u = Q.front();
        for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
            int v = it->first;
            if(vis[v]) continue;
            ret = dep[v] = dep[u] + 1;
            Q.push(v);
            vis[v] = ++ cnt;
        }
    }
    return ret;
}
int s = 1, id[oo];
bool cmp (int i, int j) {
    return vis[i] < vis[j];
}
void Build_DAG () {
    int h = bfs(1, 0); 
    for(int i = 2; i <= q.n; ++ i) {
        if(bfs(i, 0) > h) s = i;
    }
    bfs(s, 1);
    for(int i = 1; i <= q.n; ++ i) {
        id[i] = i;
    }
    sort(id + 1, id + q.n + 1, cmp);
}
int N1[oo][oo][oo], N2[oo][oo][oo], Np[oo][oo], Nc[oo][oo];
int parent_num[oo], child_num[oo];
int get_parent_num (int u) {
    int ret = 0;
    for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
        int v = it->first;
        if(vis[u] > vis[v]) ++ ret;
    }
    return ret;
}

int get_child_num (int u) {
    int ret = 0;
    for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
        int v = it->first;
        if(vis[u] < vis[v]) ++ ret;
    }
    return ret;
}
void Build_DCS () {
    // 
    for(int i = 1; i <= q.n; ++ i) {
        parent_num[i] = get_parent_num(i);
        child_num[i] = get_child_num(i);
        for(int j = 1; j <= g.n; ++ j) {
            if(q.label[i] == g.label[j]) {
                q.candidates[i].push_back(j);
            }
        }
    }
    //
    for(int i = 1; i <= q.n; ++ i) {
        int u = id[i];
        for(int j = 0; j < q.candidates[u].size(); ++ j) {
            int v = q.candidates[u][j];
            for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
                int up = it->first;
                if(vis[up] > vis[u]) continue;
                for(it2 = q.D1[up].begin(); it2 != q.D1[up].end(); ++ it2) {
                    int vp = it2->first;
                    if(g.neighbour[v].count(vp)) {
                        ++ N1[u][v][up];
                    }
                }
                if(N1[u][v][up]) {
                    ++ Np[u][v];
                }
            }
            if(Np[u][v] == parent_num[u]) {
                q.D1[u].insert({v, 1});
            }
        }
    }


    for(int i = q.n; i; -- i) {
        int u = id[i];
        for(int j = 0; j < q.candidates[u].size(); ++ j) {
            int v = q.candidates[u][j];
            for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
                int uc = it->first;
                if(vis[uc] < vis[u]) continue;
                for(it2 = q.D2[uc].begin(); it2 != q.D2[uc].end(); ++ it2) {
                    int vc = it2->first;
                    if(g.neighbour[v].count(vc)) {
                        ++ N2[u][v][uc];
                    }
                }
                if(N2[u][v][uc]) {
                    ++ Nc[u][v];
                }
            }
            if(Nc[u][v] == child_num[u]) {
                q.D2[u].insert({v, 1});
                for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
                    int uc = it->first;
                    if(vis[uc] < vis[u]) continue;
                    for(int i = 0; i < q.candidates[uc].size(); ++ i) {
                        int vc = q.candidates[uc][i];
                        if(g.neighbour[v].count(vc)) {
                            N2[uc][vc][u] ++;
                        }
                    }
                }
            }
        }
    }
}
EDCSs DCSChangedEdge (Edge o) {
    int v = o.x, v_ = o.y;
    EDCSs edcss;
    for(int u = 1; u <= q.n; ++ u) {
        for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
            int u_ = it->first;
            if(vis[u_] < vis[u]) continue;
            if(q.label[u] == g.label[v] && q.label[u_] == g.label[v_]) {
                edcss.e.push_back((EDCS){u, v, u_, v_});
            }
        }
    }
    return edcss;
}
void InsertEdge (Edge o) {
    g.neighbour[o.x].insert({o.y, 1});
    g.neighbour[o.y].insert({o.x, 1});
}
void DeleteEdge (Edge o) {
    g.neighbour[o.x].erase(o.y);
    g.neighbour[o.y].erase(o.x);
}
queue <pair<int, int>> Q1, Q2;
//unordered_map <pair<int, int>, int> N1[oo];
//unordered_map <pair<int, int>, int> N2[oo];




void InsertTopDown (int u, int v, int uc, int vc) {
    if(N1[uc][vc][u] == 0) {
        Np[uc][vc] ++;
        if(Np[uc][vc] == parent_num[uc]) {
            q.D1[uc].insert({vc, 1});
            Q1.push({uc, vc});
        }
        if(Nc[uc][vc] == child_num[uc]) {
            q.D2[uc].insert({vc, 1});
            Q2.push({uc, vc});
        }
    }
    N1[uc][vc][u] ++;
}

void InsertBottomUp (int u, int v, int up, int vp) {
    if(N2[up][vp][u] == 0) {
        Nc[up][vp] ++;
        if(q.D1[up].count(vp) && Nc[up][vp] == child_num[up]) {
            q.D2[up].insert({vp, 1});
            Q2.push({up, vp});
        }
    }
    N2[up][vp][u] ++;
}

void DCSInsert (EDCSs edcss) {
    for(int i = 0; i < edcss.e.size(); ++ i) {
        int u1 = edcss.e[i].u;
        int v1 = edcss.e[i].v;
        int u2 = edcss.e[i].u_;
        int v2 = edcss.e[i].v_;
        if(q.D1[u1].count(v1)) {
            InsertTopDown(u1, v1, u2, v2);
        }
        if(q.D2[u2].count(v2)) {
            InsertBottomUp(u2, v2, u1, v1);
        }
        if(q.D2[u1].count(v1)) {
            N2[u2][v2][u1] ++;
        }
        while(!Q1.empty()) {
            int u = Q1.front.first;
            int v = Q1.front.second;
            Q1.pop();
            for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
                int uc = it->first;
                if(vis[uc] < vis[u]) continue;
                for(int i = 0; i < q.candidates[uc].size(); ++ i) {
                    int vc = q.candidates[uc][i];
                    if(g.neighbour[v].count(vc)) {
                        InsertTopDown(u, v, uc, vc);
                    }
                }
            }
        }

        while(!Q2.empty()) {
            int u = Q2.front.first;
            int v = Q2.front.second;
            Q2.pop();
            for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
                int up = it->first;
                if(vis[up] > vis[u]) continue;
                for(int i = 0; i < q.candidates[up].size(); ++ i) {
                    int vp = q.candidates[up][i];
                    if(g.neighbour[v].count(vp)) {
                        InsertBottomUp(u, v, up, vp);
                    }
                }
            }
            for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
                int uc = it->first;
                if(vis[uc] < vis[u]) continue;
                for(int i = 0; i < q.candidates[uc].size(); ++ i) {
                    int vc = q.candidates[uc][i];
                    if(g.neighbour[v].count(vc)) {
                        N2[uc][vc][u] ++;
                    }
                }
            }
        }
    }
}

void DeleteTopDown (int u, int v, int uc, int vc) {
    if(N1[uc][vc][u] == 1) {
        Np[uc][vc] --;
        if(q.D1[uc].count(vc)) {
            q.D1[uc].erase(vc);
            Q1.push({uc, vc});
        }
        if(q.D2[uc].count(vc)) {
            q.D2[uc].erase(vc);
            Q2.push({uc, vc});
        }
    }
    N1[uc][vc][u] --;
}

void DeleteBottomUp (int u, int v, int up, int vp) {
    if(N2[up][vp][u] == 1) {
        Nc[up][vp] --;
        if(q.D2[up].count(vp)) {
            q.D2[up].erase(vp);
            Q2.push({up, vp});
        }
    }
    N2[up][vp][u] --;
}

void DCSDelete (EDCSs edcss) {
    for(int i = 0; i < edcss.e.size(); ++ i) {
        int u1 = edcss.e[i].u;
        int v1 = edcss.e[i].v;
        int u2 = edcss.e[i].u_;
        int v2 = edcss.e[i].v_;
        if(q.D2[u1].count(v1)) {
            N2[u2][v2][u1] --;
        }
        if(q.D1[u1].count(v1)) {
            DeleteTopDown(u1, v1, u2, v2);
        }
        if(q.D2[u2].count(v2)) {
            DeleteBottomUp(u2, v2, u1, v1);
        }
        while(!Q1.empty()) {
            int u = Q1.front.first;
            int v = Q1.front.second;
            Q1.pop();
            for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
                int uc = it->first;
                if(vis[uc] < vis[u]) continue;
                for(int i = 0; i < q.candidates[uc].size(); ++ i) {
                    int vc = q.candidates[uc][i];
                    if(g.neighbour[v].count(vc)) {
                        DeleteTopDown(u, v, uc, vc);
                    }
                }
            }
        }

        while(!Q2.empty()) {
            int u = Q2.front.first;
            int v = Q2.front.second;
            Q2.pop();
            for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
                int up = it->first;
                if(vis[up] > vis[u]) continue;
                for(int i = 0; i < q.candidates[up].size(); ++ i) {
                    int vp = q.candidates[up][i];
                    if(g.neighbour[v].count(vp)) {
                        DeleteBottomUp(u, v, up, vp);
                    }
                }
            }
            for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
                int uc = it->first;
                if(vis[uc] < vis[u]) continue;
                for(int i = 0; i < q.candidates[uc].size(); ++ i) {
                    int vc = q.candidates[uc][i];
                    if(g.neighbour[v].count(vc)) {
                        N2[uc][vc][u] --;
                    }
                }
            }
        }
    }
}


void input_q () {

}
void input_g () {

}
int M[oo], mcnt, umin[oo];
vector<int> Nbrm[oo];
void update (int u) {
    Nbrm[u].clear();
    for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
        int v = it->first;
        if(M[v] == 0) continue;
        Nbrm[u].push_back(v);
    }
    if(Nbrm[u].size() == 0) {
        umin[u] = 0;
    }
    else {
        for(int i = 0; i < Nbrm[u].size(); ++ i) {
            int u_ = Nbrm[u][i];
            if(umin[u] == 0 || N2[u_][M[u_]][u] < N2[umin[u]][M[umin[u]]][u]) {
                umin[u] = u_;
            }
        }
    }
}
void UpdateEmbedding (int u) {
    for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
        int v = it->first;
        Update(v);
    }
}
void RestoreEmbedding (int u) {
    M[u] = 0;
    for(it = q.neighbour[u].begin(); it != q.neighbour[u].end(); ++ it) {
        int v = it->first;
        Update(v);
    }
}

vector <int> ComputeCm (int u) {
    vector <int> Cm;
    for(int v = 1; v <= q.n; ++ v) {
        if(q.D2[u].count(v) && g.neighbour[v].count(M[umin[u]])) {
            int flag = 1;
            for(int i = 0; i < Nbrm.size(); ++ i) {
                int u_ = Nbrm[u][i];
                if(!g.neighbour[v].count(M[u_])) {
                    flag = 0;
                }
            }
            if(flag) {
                Cm.push_back(v);
            }
        }
    }
    return Cm;
}

int get_nxt () {
    int ret = 0;
    for(int u = 1; u <= q.n; ++ u) {
        int u_ = umin[u];
        if(umin[u] && (ret == 0 || N2[u_][M[u_]][u] < N2[umin[ret]][M[umin[ret]]][ret])) {
            ret = u;
        }
    }
    return ret;
}

void FindMatches (EDCSs edcss) {
    if(mcnt == q.n) {
        return;
    }
    if(mcnt == 0) {
        for(int i = 0; i < edcss.e.size(); ++ i) {
            int u = edcss.e[i].u;
            int v = edcss.e[i].v;
            int u_ = edcss.e[i].u_;
            int v_ = edcss.e[i].v_;
            if(q.D2[u].count(v) && q.D2[u_].count(v_)) {
                M[u] = v;
                M[u_] = v_;
                UpdateEmbedding(u);
                UpdateEmbedding(u_);
                mcnt += 2;
                FindMatches(edcss);
                mcnt -= 2;
                RestoreEmbedding(u);
                RestoreEmbedding(u_);
            }
        }
    }
    else {
        int u = get_nxt();
        vector <int> Cm = ComputeCm(u);
        for(int i = 0; i < Cm.size(); ++ i) {
            M[u] = v;
            UpdateEmbedding(u);
            FindMatches(edcss);
            RestoreEmbedding(u);
        }
    }
}
int main () {
    input_q();
    input_g();
    Build_DAG();
    Build_DCS();
    for(int i = 1; i <= m; ++ i) {
        if(stream[i].opt == 1) {
            EDCSs edcss = DCSChangedEdge(stream[i]);
            InsertEdge(stream[i]);
            DCSInsert(edcss);
            FindMatches(edcss);
        }
        else {
            EDCSs edcss = DCSChangedEdge(stream[i]);
            FindMatches(edcss);
            DeleteEdge(stream[i]);
            DCSDelete(edcss);
        }
    }
    return 0;
}