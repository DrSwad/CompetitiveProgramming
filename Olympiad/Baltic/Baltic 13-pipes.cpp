/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- There can't be any cycle of even-length:
		    We prove it using contradiction. If there is such a cycle, let the
		  edges of this cycle be e_1, e_2, ..., e_k, where k is even. Then
		  increase each odd indexed edge by 1 and decrease each even indexed
		  edge by 1. The resulting values in nodes stays the same, thus the
		  solution wouldn't be unique.

		- For any cycle, no two nodes of this cycle are connected with each other
		  using edges outside the cycle:
		    We prove it using contradiction. Firstly the cycle length must be
		  odd. Now, let two nodes, x and y of this cycle be connected with
		  each other using edges excluding the ones of the cycle. Let the
		  length of the two paths from x to y inside the cycle be d_1, d_2 and
		  let the length of the new path from x to y be d_3. Since d_1 + d_2 is
		  the total length of the cycle and this length is odd, parity of d_1
		  and d_2 must differ. Thus one of them must have the same parity with
		  d_3, WLOG let it be d_1. Then we get a cycle from x to y with length
		  d_1 + d_3 which is of even length, contradiction.

		- So, with a little work, it's easy to deduce that the graph must be a
		  tree of rings, each ring of odd-length.

		  But, if the graph has no ring of length 1(a single node) and if there
		  is more than one ring, then we can see that increasing any single edge
		  by 2 can give another solution set.

		  Thus, we first remove all nodes with degree 1 from the graph and then
		  check if the graph has only a single ring of odd-length. If it doesn't
		  then there is no solution, and if it does it's not hard to find the
		  solutions for the edges.

*/

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "debug.h"
#endif

typedef long long ll;
typedef pair<int, int> pii;

#define x first
#define y second

const int N = 1e5 + 5;

int n, m;
vector<int> c;
vector<set<int>> adj;
vector<pii> e;
vector<ll> ans;

int adjacentNode(int node, int i) {
	assert(i == 0 || i == 1);
	int adj_edge = (i == 0 ? *adj[node].begin() : *adj[node].rbegin());
	int adj_node = e[adj_edge].x ^ e[adj_edge].y ^ node;
	return adj_node;
}

void dfs(int at, int p, int root, vector<pii> &order) {
	int nxt;

	nxt = adjacentNode(at, 0);
	if (nxt != p) order.push_back({at, *adj[at].begin()});
	else {
		nxt = adjacentNode(at, 1);
		if (nxt != p) order.push_back({at, *adj[at].rbegin()});
	}

	if (nxt != root) dfs(nxt, at, root, order);
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d", &n, &m);

	c.resize(n);
	for (int i = 0; i < n; i++) scanf("%d", &c[i]);

	adj.resize(n);
	e.resize(m);
	for (int i = 0; i < m; i++) {
		int u, v;
		scanf("%d %d", &u, &v);
		u--, v--;
		adj[u].insert(i);
		adj[v].insert(i);
		e[i] = {u, v};
	}

	ans.resize(m);
	queue<int> q;
	for (int i = 0; i < n; i++) q.push(i);

	while (!q.empty()) {
		 int curr_node = q.front(); q.pop();
		 if (adj[curr_node].size() == 1) {
		 	int adj_edge = *adj[curr_node].begin();
		 	int adj_node = adjacentNode(curr_node, 0);

		 	ans[adj_edge] = 2LL * c[curr_node];
		 	c[curr_node] -= ans[adj_edge] / 2;
		 	c[adj_node] -= ans[adj_edge] / 2;

		 	adj[curr_node].erase(adj_edge);
		 	adj[adj_node].erase(adj_edge);

		 	if (adj[adj_node].size() == 1) q.push(adj_node);
		 }
	}

	int cnt_node = 0;
	for (int i = 0; i < n; i++) {
		if (adj[i].size() > 2) {
			printf("0\n");
			return 0;
		}
		cnt_node += adj[i].size() == 2;
	}

	if (cnt_node > 0 && cnt_node % 2 == 0) {
		printf("0\n");
		return 0;
	}

	int first_node;
	for (first_node = 0; first_node < n; first_node++) {
		if (adj[first_node].size() == 2) break;
	}

	if (first_node < n) {
		vector<pii> order;
		dfs(first_node, -1, first_node, order);

		ll sum = 2LL * c[first_node];
		ll diff = 0;
		for (int i = 1; i < order.size(); i++) {
			diff = 2LL * c[order[i].x] - diff;
		}

		ans[order[0].y] = (sum - diff) / 2LL;

		for (int i = 1; i < order.size(); i++) {
			ans[order[i].y] = 2LL * c[order[i].x] - ans[order[i - 1].y];
		}
	}

	for (int i = 0; i < m; i++) printf("%lld\n", ans[i]);

	return 0;
}
