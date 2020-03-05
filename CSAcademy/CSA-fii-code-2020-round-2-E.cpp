/*
	Author: Nayeemul Islam Swad

	Idea:
		- We progress one minute at a time and maintain the cells in a boolean
		  array `vis` that are reachable from (1, 1) after the current minute.
		  As we're processing every minute, we can also update the minimum time
		  needed to reach a cell, dist[cell], each time we're visiting that cell.
		  In the end we just need to output dist[(n, m)].

		- For each time `t`, we maintain a queue q[t] that stores the cells that
		  are reachable from (1, 1) at time `t` but not at time `t - 1`.

		- So, suppose there are no restrictions on any cell. Then we could just
		  do a normal bfs: At time `t` check the cells that we have in q[t], for
		  each cell check it's adjacent cells and add the unvisited ones to
		  q[t + 1].

		- But the additional restrictions on cells mean that some cells are
		  blocked at certain time intervals. But this doesn't really change our
		  bfs much. We'll need an additional boolean array `blocked` to handle
		  the restrictions:

		    * As we're moving through times, suppose we're at time `t` at the
		      moment. For each cell in q[t], before inserting any of it's
		      adjacent cells to q[t + 1] we need to make sure it's not blocked.

		    * The cells that are supposed to be blocked starting at time `t + 1`
		      need to be set to `true` in our `blocked` array and to false in
		      our `vis` array.

		    * The cells that are supposed to be free from blocked status at
		      time `t + 1` need to be set to `false` in our `blocked` array and
		      if we find any of it's adjacent cells as `true` in the `vis` array
		      we also set it to `true` in the `vis` array.

		- Finally, to make sure we don't have to iterate over every possible
		  time(that'll surely get TLE, since max needed time can be around 1e9 + 1e6),
		  when we're at time `t` we check if both q[t] and q[t + 1] are empty,
		  because then this'll mean that there'll be no change in the `vis` or
		  the `blocked` array until we reach the start/end of the next restriction
		  on some cell. So, we just set `t` to the next start/end of any restriction.
		  And if we've already passed all the restrictions, then that means there'll
		  be no change any more by our bfs, so we should just end.
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

const int N = int(2e3) + 10;

int n, m;
bool grid[N][N];
map<int, set<pii>> st, en;
vector<int> times;
int dist[N][N];
queue<pii> q[2];
bool vis[N][N];
bool blocked[N][N];
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

void visit(int at_time, int x, int y) {
	if (1 <= x && x <= n && 1 <= y && y <= m && grid[x][y] && !blocked[x][y] && !vis[x][y]) {
		vis[x][y] = true;
		q[(at_time & 1) ^ 1].push(make_pair(x, y));
	}
}
bool isVisited(int x, int y) {
	return 1 <= x && x <= n && 1 <= y && y <= m && vis[x][y];
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d", &n, &m);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			char ch;
			scanf(" %c", &ch);
			grid[i][j] = ch == '.';
		}
	}

	int r;
	scanf("%d", &r);
	while (r--) {
		int x, y, s, t;
		scanf("%d %d %d %d", &x, &y, &s, &t);
		st[s].insert(make_pair(x, y));
		en[t].insert(make_pair(x, y));
		times.push_back(s - 1);
		times.push_back(t);
	}

	sort(times.begin(), times.end());
	times.resize(unique(times.begin(), times.end()) - times.begin());

	memset(dist, -1, sizeof dist);
	q[0].push(make_pair(1, 1));
	vis[1][1] = true;
	int at_time = 0;

	while (true) {
		vector<pii> to_push;
		if (en.find(at_time) != en.end()) {
			for (const pii &cell : en[at_time]) {
				if ((st.find(at_time + 1) == st.end()) ||
					(st[at_time + 1].find(cell) == st[at_time + 1].end())) {
					blocked[cell.x][cell.y] = false;

					bool flag = false;
					for (int di = 0; di < 4; di++) {
						flag = flag || isVisited(cell.x + dx[di], cell.y + dy[di]);
					}

					if (flag) to_push.push_back(cell);
				}
			}
		}
		for (pii &cell : to_push) {
			vis[cell.x][cell.y] = true;
			q[(at_time & 1) ^ 1].push(cell);
		}
		if (st.find(at_time + 1) != st.end()) {
			for (const pii &cell : st[at_time + 1]) {
				blocked[cell.x][cell.y] = true;
				vis[cell.x][cell.y] = false;
			}
		}

		if (!q[at_time & 1].empty()) {
			while (!q[at_time & 1].empty()) {
				pii cell = q[at_time & 1].front(); q[at_time & 1].pop();
				if (dist[cell.x][cell.y] == -1) dist[cell.x][cell.y] = at_time;

				for (int di = 0; di < 4; di++) {
					visit(at_time, cell.x + dx[di], cell.y + dy[di]);
				}
			}
		}
		else if (q[(at_time & 1) ^ 1].empty()) {
			int nxt = upper_bound(times.begin(), times.end(), at_time) - times.begin();
			if (nxt == times.size()) break;

			at_time = times[nxt];
			continue;
		}

		at_time = at_time + 1;
	}

	printf("%d\n", dist[n][m]);

	return 0;
}
