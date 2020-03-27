/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://codeforces.com/blog/entry/66101
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

const int N = int(5e3) + 10;

int n, m;
int p[N];
int c[N];
vector<int> adj[N + N];
int c_to[N];
vector<int> kv;
int curr_ans;

void add_edge(int p, int c) {
	adj[N + p].push_back(c);

	while (true) {
		int need_mex = N + curr_ans;
		vector<int> from(N + N, 0);
		from[need_mex] = need_mex;
		queue<int> q;
		for (int to : adj[need_mex]) if (from[to] == 0) {
			from[to] = need_mex;
			q.push(to);
		}
		int end = -1;

		while (!q.empty()) {
			int at = q.front(); q.pop();
			if (at < N && c_to[at] == 0) {
				end = at;
				break;
			}

			for (int to : adj[at]) if (from[to] == 0) {
				from[to] = at;
				q.push(to);
			}
		}

		if (end == -1) break;

		int at = end;
		adj[at].resize(1);

		while (at != need_mex) {
			c_to[at] = from[at];
			assert(adj[at].size() == 1);
			adj[at][0] = from[at];
			at = from[from[at]];
		}

		curr_ans++;
	}
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d", &n, &m);
	for (int i = 1; i <= n; i++) scanf("%d", &p[i]);
	for (int i = 1; i <= n; i++) scanf("%d", &c[i]);

	vector<bool> vis(n + 1, false);
	int d;
	scanf("%d", &d);
	for (int i = 1; i <= d; i++) {
		int k;
		scanf("%d", &k);
		kv.push_back(k);
		vis[k] = true;
	}
	for (int i = 1; i <= n; i++) if (!vis[i]) kv.push_back(i);

	for (int i = n; i > d; i--) add_edge(p[kv[i - 1]], c[kv[i - 1]]);

	stack<int> ans;
	for (int i = d; i >= 1; i--) {
		ans.push(curr_ans);
		add_edge(p[kv[i - 1]], c[kv[i - 1]]);
	}

	while (!ans.empty()) {
		printf("%d\n", ans.top());
		ans.pop();
	}

	return 0;
}
