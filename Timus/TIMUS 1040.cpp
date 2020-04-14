/*
	Author: Nayeemul Islam Swad

	Idea:
		- Since the graph is guaranteed to be connected, let's do an euler
		  tour in it's dfs tree. We will keep a counter and assign answers
		  to the edges in the order they are visited. This way,

		    * The root has an incident edge with assigned answer 1, so it's
		      edges' assigned answers have total gcd 1.

		    * For any node other than the root or leaves, let `p` be the
		      assigned answer to the edge incident to it's parent. Then the
		      first edge that we visit next will be assigned p + 1. So,
		      no matter what the other incident edges are assigned, the
		      total gcd of the incident edges of the node will be 1.

		    * To handle the cases of leaves, we need to also assign answers
		      to the edges that we don't visit during the dfs and increase
		      the counter. Notice that this doesn't cause any problems for
		      the previous two points, and it also solves the leaf case.
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

const int N = 55;

int n, m;
vector<pii> e;
vector<int> adj[N];
vector<int> ans;

bool vis[N];
int cnt;
void dfs(int at) {
	vis[at] = true;
	for (int ei : adj[at]) {
		int to = e[ei].x ^ e[ei].y ^ at;
		if (vis[to] && ans[ei] == -1) {
			ans[ei] = ++cnt;
		}
	}
	for (int ei : adj[at]) {
		int to = e[ei].x ^ e[ei].y ^ at;
		if (!vis[to]) {
			ans[ei] = ++cnt;
			dfs(to);
		}
	}
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		int u, v;
		scanf("%d %d", &u, &v);
		adj[u].push_back(e.size());
		adj[v].push_back(e.size());
		e.push_back({u, v});
	}

	ans.resize(e.size(), -1);
	dfs(1);

	puts("YES");
	for (int i = 0; i < ans.size(); i++) {
		if (i > 0) printf(" ");
		printf("%d", ans[i]);
	}
	puts("");

	return 0;
}
