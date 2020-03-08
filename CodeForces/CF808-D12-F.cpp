/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://codeforces.com/blog/entry/52010
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

const int N = 105;
const int MAXC = int(1e5) + 10;

int n, k;
int p[N], c[N], l[N];
bool isprime[2 * MAXC];

struct MaxFlow {
	const int INF = int(1e6);
	int n;
	vector<vector<int>> adj;

	MaxFlow(int n) : n(n) {
		adj.resize(n, vector<int>(n, 0));
	}

	int findAugmentedPath(vector<vector<int>> &resd) {
		vector<int> from(n, -1);
		vector<bool> vis(n, false);
		queue<int> q;
		q.push(0);
		vis[0] = true;

		while (!q.empty()) {
			int at = q.front(); q.pop();
			if (at == n - 1) break;

			for (int nxt = 0; nxt < n; nxt++) {
				if (!vis[nxt] && resd[at][nxt] > 0) {
					from[nxt] = at;
					vis[nxt] = true;
					q.push(nxt);
				}
			}
		}

		int path_capacity = INF;
		int at = n - 1;
		while (from[at] != -1) {
			path_capacity = min(path_capacity, resd[from[at]][at]);
			at = from[at];
		}

		at = n - 1;
		while (from[at] != -1) {
			resd[from[at]][at] -= path_capacity;
			resd[at][from[at]] += path_capacity;
			at = from[at];
		}

		return path_capacity;
	}

	int solve() {
		vector<vector<int>> resd = adj;

		int flow = 0;
		while (true) {
			int path_capacity = findAugmentedPath(resd);
			if (path_capacity == INF) break;
			flow += path_capacity;
		}

		return flow;
	}
};

bool check(int L) {
	if (L == 0) return true;

	vector<int> ps, cs;
	int max_p1 = 0;
	for (int i = 1; i <= n; i++) {
		if (l[i] <= L) {
			if (c[i] == 1) max_p1 = max(max_p1, p[i]);
			else {
				ps.push_back(p[i]);
				cs.push_back(c[i]);
			}
		}
	}

	if (max_p1 > 0) {
		ps.push_back(max_p1);
		cs.push_back(1);
	}

	MaxFlow maxFlow = MaxFlow(ps.size() + 2);

	for (int i = 0; i < ps.size(); i++) {
		int pi = ps[i], ci = cs[i];

		if (ci & 1) maxFlow.adj[0][i + 1] = pi;
		else maxFlow.adj[i + 1][ps.size() + 1] = pi;

		if (ci & 1) for (int j = 0; j < ps.size(); j++) {
			if (cs[j] % 2 == 0 && isprime[cs[i] + cs[j]]) {
				maxFlow.adj[i + 1][j + 1] = maxFlow.INF;
			}
		}
	}

	return accumulate(ps.begin(), ps.end(), 0) - maxFlow.solve() >= k;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d", &n, &k);
	for (int i = 1; i <= n; i++) {
		scanf("%d %d %d", &p[i], &c[i], &l[i]);
	}

	fill(isprime + 2, isprime + 2 * MAXC, 1);
	for (int i = 2; i < 2 * MAXC; i++) if (isprime[i]) {
		for (int j = 2 * i; j < 2 * MAXC; j += i) isprime[j] = false;
	}

	int L = 1, R = n;
	while (L != R) {
		int mid = (L + R) / 2;
		if (check(mid)) R = mid;
		else L = mid + 1;
	}

	if (check(L)) printf("%d\n", L);
	else puts("-1");

	return 0;
}
