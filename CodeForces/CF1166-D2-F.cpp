/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://codeforces.com/blog/entry/67081
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "/Users/swad/Desktop/CP/debug.h"
#endif

const int N = int(1e5) + 10;

int n, m, c, q;
map<int, int> edge_colored[N];
int root[N];
vector<int> component[N];
vector<int> adj[N];
map<int, int> adj_roots[N];

void merge(int u, int v) {
	u = root[u];
	v = root[v];
	if (u == v) return;
	if (component[u].size() > component[v].size()) swap(u, v);

	for (int elem : component[u]) {
		root[elem] = v;
		for (int to : adj[elem]) adj_roots[to][root[elem]] = 1;
	}
	copy(component[u].begin(), component[u].end(), back_inserter(component[v]));
	component[u].clear();
}

void addEdge(int u, int v, int color) {
	adj[u].push_back(v);
	adj[v].push_back(u);

	adj_roots[u][root[v]] = 1;
	adj_roots[v][root[u]] = 1;

	if (edge_colored[u].find(color) != edge_colored[u].end()) merge(v, edge_colored[u][color]);
	else edge_colored[u][color] = v;
	if (edge_colored[v].find(color) != edge_colored[v].end()) merge(u, edge_colored[v][color]);
	else edge_colored[v][color] = u;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d %d %d", &n, &m, &c, &q);

	for (int i = 1; i <= n; i++) {
		root[i] = i;
		component[i].push_back(i);
		adj[i].push_back(i);
		adj_roots[i][i] = 1;
	}

	for (int i = 1; i <= m; i++) {
		int u, v, color;
		scanf("%d %d %d", &u, &v, &color);
		addEdge(u, v, color);
	}

	for (int i = 1; i <= q; i++) {
		char type;
		scanf(" %c", &type);

		if (type == '+') {
			int u, v, color;
			scanf("%d %d %d", &u, &v, &color);
			addEdge(u, v, color);
		}
		else {
			int u, v;
			scanf("%d %d", &u, &v);
			if (adj_roots[v].find(root[u]) != adj_roots[v].end()) puts("Yes");
			else puts("No");
		}
	}

	return 0;
}
