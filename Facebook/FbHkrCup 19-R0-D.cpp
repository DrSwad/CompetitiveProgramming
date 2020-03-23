/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://web.facebook.com/notes/facebook-hacker-cup/hacker-cup-2019-qualification-round-solutions/2797355073613709/
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

const int N = 65;
const int M = 125;

int n;
int p[N];
int m;
int x[M], y[M], z[M];

int findClan(int i, vector<int> &clan) {
	if (i == clan[i]) return i;
	else return clan[i] = findClan(clan[i], clan);
}

void merge(int i, int j, vector<int> &clan) {
	i = findClan(i, clan);
	j = findClan(j, clan);
	if (i != j) clan[i] = j;
}

int solve(vector<int> v) {
	vector<bool> vis(n + 1, false);
	vector<int> clan(n + 1);
	for (int i : v) vis[i] = true;

	for (int root : v) {
		bool possible = true;
		for (int i = 1; i <= m; i++) {
			if (z[i] != root && vis[z[i]] && (x[i] == root || y[i] == root)) {
				possible = false;
				break;
			}
		}
		if (!possible) continue;

		fill(clan.begin(), clan.end(), 0);
		for (int i : v) clan[i] = i;

		for (int i = 1; i <= m; i++) {
			if (z[i] != root && vis[x[i]] && vis[y[i]] && vis[z[i]]) {
				merge(x[i], y[i], clan);
				merge(x[i], z[i], clan);
			}
		}

		for (int i = 1; i <= m; i++) {
			if (z[i] == root && x[i] != root && y[i] != root && findClan(x[i], clan) == findClan(y[i], clan)) {
				possible = false;
				break;
			}
		}
		if (!possible) continue;

		clan[root] = 0;
		sort(v.begin(), v.end(), [&](int i, int j) {return findClan(i, clan) < findClan(j, clan);});

		for (int l = 1; l < v.size(); ) {
			int r = l;
			while (r < v.size() && findClan(v[r], clan) == findClan(v[l], clan)) r++;

			int child = solve(vector<int>(v.begin() + l, v.begin() + r));
			if (child == 0) return 0;
			else p[child] = root;

			l = r;
		}

		return root;
	}

	return 0;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	int t;
	cin >> t;

	for (int cs = 1; cs <= t; cs++) {
		scanf("%d %d", &n, &m);
		for (int i = 1; i <= m; i++) scanf("%d %d %d", &x[i], &y[i], &z[i]);
		vector<int> v(n);
		iota(v.begin(), v.end(), 1);
		int root = solve(v);

		printf("Case #%d:", cs);
		if (root == 0) puts(" Impossible");
		else {
			p[root] = 0;
			for (int i = 1; i <= n; i++) printf(" %d", p[i]);
			puts("");
		}
	}

	return 0;
}
