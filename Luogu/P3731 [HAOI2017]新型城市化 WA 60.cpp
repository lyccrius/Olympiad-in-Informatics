#include <iostream>
#include <algorithm>
#include <vector>

typedef std::pair<int, int> pii;
typedef std::vector<pii> vpc;
typedef std::vector<int> vic;

const int maxN = 1e4;
const int maxM = 1.5e5;

int n, m;
int x[maxM + 10], y[maxM + 10];
int l[maxN + 10], r[maxN + 10];
vpc ans;

namespace graph {
    struct Vertex {
        int vis;
        int head;
        int match;
        int dfn;
        int low;
        int ins;
        int bel;
    } vertex[maxN + 10];

    struct Edge {
        int head;
        int next;
    } edge[maxM + 10];

    int ecnt;

    void addEdge(int tail, int head) {
        ecnt++;
        edge[ecnt].head = head;
        edge[ecnt].next = vertex[tail].head;
        vertex[tail].head = ecnt;
        return;
    }

    bool DFS(int u, int t) {
        for (int e = vertex[u].head; e; e = edge[e].next) {
            int v = edge[e].head;
            if (vertex[v].vis == t) continue;
            vertex[v].vis = t;
            if (vertex[v].match == 0 || DFS(vertex[v].match, t)) {
                vertex[v].match = u;
                return true;
            }
        }
        return false;
    }

    namespace residual {
        Vertex vertex[maxN + 10];
        Edge edge[maxM + 10];
        int ecnt;
        int scnt;
        int dfn;
        vic s;

        void addEdge(int tail, int head) {
            //printf("%d %d\n", tail, head);
            ecnt++;
            edge[ecnt].head = head;
            edge[ecnt].next = vertex[tail].head;
            vertex[tail].head = ecnt;
            return;
        }

        void addSCC(int u) {
            scnt++;
            int v;
            do {
                v = s.back();
                s.pop_back();
                vertex[v].ins = false;
                vertex[v].bel = scnt;
            } while (v != u);
            return;
        }

        void Tarjan(int u) {
            dfn++;
            vertex[u].dfn = dfn;
            vertex[u].low = dfn;
            vertex[u].ins = true;
            s.push_back(u);
            for (int e = vertex[u].head; e; e = edge[e].next) {
                int v = edge[e].head;
                if (!vertex[v].dfn) {
                    Tarjan(v);
                    vertex[u].low = std::min(vertex[u].low, vertex[v].low);
                } else if (vertex[v].ins) {
                    vertex[u].low = std::min(vertex[u].low, vertex[v].dfn);
                }
            }
            if (vertex[u].low == vertex[u].dfn) addSCC(u);
            return;
        }
    }
}

vic e[maxN + 10];
int v[maxN + 10];
int c[maxN + 10];

void color(int u, int col) {
    if (v[u]) return;
    v[u] = true;
    c[u] = col;
    for (auto w : e[u]) color(w, col ^ 1);
    return;
}

int a[maxN + 10];

int main() {
    std::cin >> n >> m;
    for (int i = 1; i <= m; i++) std::cin >> x[i] >> y[i], e[x[i]].push_back(y[i]), e[y[i]].push_back(x[i]), a[x[i]] = true, a[y[i]] = true;
    for (int i = 1; i <= n; i++) if (a[i]) if (!v[i]) color(i, 1);
    for (int i = 1; i <= n; i++) if (a[i]) if (c[i]) l[i] = true; else r[i] = true;
    for (int i = 1; i <= m; i++) if (r[x[i]]) std::swap(x[i], y[i]);
    for (int i = 1; i <= m; i++) graph::addEdge(x[i], y[i]); 
    for (int i = 1; i <= n; i++) if (l[i]) graph::DFS(i, i);
    for (int i = 1; i <= m; i++) {
        if (graph::vertex[y[i]].match == x[i]) {
            graph::vertex[x[i]].match = y[i];
            graph::residual::addEdge(y[i], x[i]); 
            //printf("match %d, %d\n", y[i], x[i]);
        } else {
            graph::residual::addEdge(x[i], y[i]);
            //printf("unmatch %d, %d\n", x[i], y[i]);
        }
    }
    for (int i = 1; i <= n; i++) {
        if (r[i]) continue;
        if (l[i] == false) continue;
        if (graph::vertex[i].match == false) continue;
        for (int j = 1; j <= n; j++) {
            if (i == j) continue;
            if (r[j]) continue;
            if (l[j] == false) continue;
            if (graph::vertex[j].match != false) continue;
            graph::residual::addEdge(i, j);
            //printf("l, %d, %d\n", i, j);
        }
    }
    for (int i = 1; i <= n; i++) {
        if (l[i]) continue;
        if (r[i] == false) continue;
        if (graph::vertex[i].match != false) continue;
        for (int j = 1; j <= n; j++) {
            if (i == j) continue;
            if (l[j]) continue;
            if (r[j] == false) continue;
            if (graph::vertex[j].match == false) continue;
            graph::residual::addEdge(i, j);
            //printf("r, %d, %d\n", i, j);
        }
    }
    /*
    for (int i = 1; i <= n; i++) if (graph::vertex[i].match) {
        if (l[i]) graph::residual::addEdge(i, 0);
        else graph::residual::addEdge(n + 1, i);
    } else {
        if (l[i]) graph::residual::addEdge(0, i);
        else graph::residual::addEdge(i, n + 1);
    }
    */
    for (int i = 1; i <= n; i++) if (graph::residual::vertex[i].dfn == 0) graph::residual::Tarjan(i);
    for (int i = 1; i <= m; i++) if (graph::vertex[y[i]].match == x[i]) if (graph::residual::vertex[x[i]].bel != graph::residual::vertex[y[i]].bel) ans.push_back(std::make_pair(std::min(x[i], y[i]), std::max(x[i], y[i])));
    std::sort(ans.begin(), ans.end());
    std::cout << ans.size() << '\n';
    for (auto i : ans) std::cout << i.first << ' ' << i.second << '\n';
    return 0;
}