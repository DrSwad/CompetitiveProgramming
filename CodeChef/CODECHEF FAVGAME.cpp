/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://discuss.codechef.com/t/favgame-editorial/14144

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

const int N = int(1e3) + 10;

int n, h;
int t[N];
vector<int> ch[N];
pii dp[N][24];

void go(int at) {
	for (int to : ch[at]) go(to);

	int tot = ch[at].size();
	pii subdp[1 << tot][h];

	for (int start = 0; start < h; start++) subdp[0][start] = {0, start};

	for (int mask = 1; mask < 1 << tot; mask++) {
		for (int start = 0; start < h; start++) {
			subdp[mask][start] = {INT_MAX, INT_MAX};

			for (int bit = 0; bit < tot; bit++) if (mask & 1 << bit) {
				pii take_one = dp[ch[at][bit]][start];
				pii take_rest = subdp[mask ^ 1 << bit][take_one.y];
				subdp[mask][start] = min(subdp[mask][start], {take_one.x + take_rest.x, take_rest.y});
			}
		}
	}

	for (int start = 0; start < h; start++) {
		int _start = start;
		int took = 0;
		int nxt = _start + t[at];

		if (nxt > h) _start = 0, took++;
		nxt = _start + t[at];
		if (nxt == h) nxt = 0, took++;

		pii take_rest = subdp[(1 << tot) - 1][nxt];
		dp[at][start] = {took + take_rest.x, take_rest.y};
	}
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	int T;
	cin >> T;

	while (T--) {
		scanf("%d %d", &n, &h);
		for (int i = 1; i <= n; i++) scanf("%d", &t[i]);

		vector<bool> vis(n + 1, true);

		for (int i = 1; i <= n; i++) {
			ch[i].clear();
			int sz;
			scanf("%d", &sz);
			ch[i].resize(sz);
			for (int j = 0; j < sz; j++) {
				scanf("%d", &ch[i][j]);
				vis[ch[i][j]] = false;
			}
		}

		int root;
		for (root = 1; root <= n; root++) if (vis[root]) break;

		go(root);

		printf("%d\n", 1 + dp[root][0].x - (dp[root][0].y == 0));
	}

	return 0;
}
