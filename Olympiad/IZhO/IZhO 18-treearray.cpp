/*
	Author: Nayeemul Islam Swad

	Idea:
		- For some query l r v, if we find an x such that a[x] = v, then
		  we can just set y = x and thus have found a solution (x, y).

		- If there is no such x, then a pair (x, y) would be a solution iff
		  l <= x <= y <= r and there exists two distinct subtrees of v such
		  that
		    * All a[i] lies in one of these subtrees
		    * Each subtree has at least one a[i] between [x, y]
		  Now, suppose we're iterating from x to y and keep track of which
		  subtree we're currently in of the two subtrees. Then, there must
		  be a point when we're change the current subtree to the other one.
		  This implies that if there exists a solution (x, y) with  x != y,
		  then there also is a solution (x0, y0) such that y0 = x0 + 1.

		- So, we just maintain all the lca's of consecutive elements of the
		  array and answer queries of second types using them.
*/

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "debug.h"
#endif

typedef long long lint;
typedef unsigned int uint;

typedef pair<int, int> pii;
#define x first
#define y second

#define size(a) (int)a.size()

const int N = int(2e5) + 10, LOGN = 20;

int n, m, q;
vector<int> adj[N];
int a[N], lv[N], par[N], P[LOGN][N];
set<int> a_pos[N], lca_pos[N];

int lca(int u, int v) {
    if (lv[u] < lv[v]) {
        swap(u, v);
    }
    for (int i = LOGN - 1; i >= 0; i--) {
        if (lv[u] - (1 << i) >= lv[v]) {
            u = P[i][u];
        }
    }
    if (u == v) {
        return u;
    }
    for (int i = LOGN - 1; i >= 0; i--) {
        if (P[i][u] != P[i][v]) {
            u = P[i][u];
            v = P[i][v];
        }
    }
    return par[u];
}

void initSp() {
    memset(P, -1, sizeof P);
    for (int i = 0; i < n; i++) {
        P[0][i] = par[i];
    }
    for (int i = 1; i < LOGN; i++) {
        for (int j = 0; j < n; j++) {
            if (P[i - 1][j] != -1) {
                P[i][j] = P[i - 1][P[i - 1][j]];
            }
        }
    }
}

void dfs(int at, int p) {
	par[at] = p;
	lv[at] = (p >= 0 ? lv[p] : -1) + 1;
	for (int nxt : adj[at]) {
		if (nxt != p) dfs(nxt, at);
	}
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n >> m >> q;
	for (int i = 0; i < n - 1; i++) {
		int u, v;
		scanf("%d %d", &u, &v);
		u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	dfs(0, -1);
	initSp();

	for(int i = 0; i < m; i++) {
		scanf("%d", &a[i]);
		a[i]--;
		a_pos[a[i]].insert(i);
		if (i > 0) lca_pos[lca(a[i - 1], a[i])].insert(i - 1);
	}

	for(int i = 0; i < q; i++) {
		int type;
		scanf("%d", &type);

		if (type == 1) {
			int pos, v;
			scanf("%d %d", &pos, &v);
			pos--, v--;

			a_pos[a[pos]].erase(pos);
			a_pos[v].insert(pos);

			if (pos > 0) {
				lca_pos[lca(a[pos - 1], a[pos])].erase(pos - 1);
				lca_pos[lca(a[pos - 1], v)].insert(pos - 1);
			}

			if (pos < m - 1) {
				lca_pos[lca(a[pos], a[pos + 1])].erase(pos);
				lca_pos[lca(v, a[pos + 1])].insert(pos);
			}

			a[pos] = v;
		}
		else {
			int l, r, v;
			scanf("%d %d %d", &l, &r, &v);
			l--, r--, v--;

			auto it = a_pos[v].lower_bound(l);
			if (it != a_pos[v].end() && *it <= r) {
				printf("%d %d\n", *it + 1, *it + 1);
				continue;
			}

			if (r > l) {
				it = lca_pos[v].lower_bound(l);
				if (it != lca_pos[v].end() && *it <= r - 1) {
					printf("%d %d\n", *it + 1, *it + 2);
					continue;
				}
			}

			printf("-1 -1\n");
		}
	}

	return 0;
}
