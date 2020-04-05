/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://codeforces.com/blog/entry/50854
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

const int N = int(2e5) + 10;

int n, m, k;
vector<int> adj[N];

vector<int> line;
bool vis[N];

void dfs(int at) {
	vis[at] = true;

	for (int to : adj[at]) if (!vis[to]) {
		line.push_back(at);
		dfs(to);
	}

	line.push_back(at);
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d %d", &n, &m, &k);

	for (int i = 1; i <= m; i++) {
		int u, v;
		scanf("%d %d", &u, &v);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	dfs(1);

	int len = (2 * n + k - 1) / k;
	for (int i = 0; i < k; i++) {
		int l = min(i * len, (int)line.size() - 1);
		int r = min(l + len, (int)line.size());
		printf("%d", r - l);
		for (int i = l; i < r; i++) printf(" %d", line[i]);
		puts("");
	}

	return 0;
}
