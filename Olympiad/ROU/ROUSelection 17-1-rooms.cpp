/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://github.com/dolphingarlic/CompetitiveProgramming/blob/master/infoarena/ROUSelection%2017-rooms.cpp
*/

#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "/Users/swad/Desktop/CP/debug.h"
#endif

const int N = int(2e3) + 10;
const int Q = int(5e3) + 10;

int n, m;
char cell[N][N];
pii p[N][N];
int sz[N][N];
pii p_at[N][N];
int dp[N][N];
int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};

pii root(pii at) {
	while (at != p[at.x][at.y]) {
		pii &dad = p[at.x][at.y];
		at = dad = p[dad.x][dad.y];
	}
	return at;
}

void merge(pii p1, pii p2) {
	p1 = root(p1);
	p2 = root(p2);
	if (p1 == p2) return;

	if (sz[p1.x][p1.y] > sz[p2.x][p2.y]) swap(p1, p2);

	sz[p2.x][p2.y] += sz[p1.x][p1.y];
	sz[p1.x][p1.y] = 0;
	p[p1.x][p1.y] = p2;
}

bool is_out(pii p, pii x, pii y) {
	return p.x < x.x || x.y < p.x || p.y < y.x || y.y < p.y;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d", &n, &m);

	for (int i = 1; i <= n; i++)
	for (int j = 1; j <= m; j++) {
		scanf(" %c", &cell[i][j]);
		p[i][j] = make_pair(i, j);
		sz[i][j] = 1;
	}

	for (int i = 1; i <= n; i++)
	for (int j = 1; j <= m; j++)
	for (int d = 0; d < 4; d++)
	if (0 < i + dx[d] && i + dx[d] <= n &&
		0 < j + dy[d] && j + dy[d] <= m &&
		cell[i][j] == cell[i + dx[d]][j + dy[d]])
		merge(make_pair(i, j), make_pair(i + dx[d], j + dy[d]));

	for (int i = 1; i <= n; i++)
	for (int j = 1; j <= m; j++) {
		pii r = root(make_pair(i, j));
		p_at[r.x][r.y] = max(p_at[r.x][r.y], make_pair(i, j));
	}

	for (int i = 1; i <= n; i++)
	for (int j = 1; j <= m; j++)
	if (sz[i][j]) dp[p_at[i][j].x][p_at[i][j].y]++;

	for (int i = 1; i <= n; i++)
	for (int j = 1; j <= m; j++)
		dp[i][j] += dp[i - 1][j] + dp[i][j - 1] - dp[i - 1][j - 1];

	int q;
	scanf("%d", &q);

	for (int i = 1; i <= q; i++) {
		pii x, y;
		scanf("%d %d %d %d", &x.x, &y.x, &x.y, &y.y);
		if (x.x > x.y) swap(x.x, x.y);
		if (y.x > y.y) swap(y.x, y.y);

		int ans = dp[x.y][y.y] - dp[x.x - 1][y.y] - dp[x.y][y.x - 1] + dp[x.x - 1][y.x - 1];

		map<pii, int> mp;
		pii r;
		for (int i = x.x; r = root(make_pair(i, y.x)), i <= x.y; i++) if (is_out(p_at[r.x][r.y], x, y)) mp[r] = 1;
		for (int i = x.x; r = root(make_pair(i, y.y)), i <= x.y; i++) if (is_out(p_at[r.x][r.y], x, y)) mp[r] = 1;
		for (int j = y.x; r = root(make_pair(x.x, j)), j <= y.y; j++) if (is_out(p_at[r.x][r.y], x, y)) mp[r] = 1;
		for (int j = y.x; r = root(make_pair(x.y, j)), j <= y.y; j++) if (is_out(p_at[r.x][r.y], x, y)) mp[r] = 1;

		ans += mp.size();

		printf("%d\n", ans);
	}

	return 0;
}
