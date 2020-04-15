/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- 
*/

#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "/Users/swad/Desktop/CP/debug.h"
#endif

pair<vector<pii>, vector<pii>> reachableYs(int from_y, int dx, int dy_rem4, int max_steps) {
	#define q(x) ((x) >= 0 ? (x) / 4 : ((x) - 3) / 4)
	vector<pii> dy_ranges;

	int dx_rem4 = dx % 4, dx_rem2 = dx_rem4 % 2;
	int dy_rem2 = dy_rem4 % 2;

	if (dx_rem2 == 0 && dx <= 2 * max_steps && dx_rem4 / 2 == dy_rem2) {
		// Insert 0 to max_steps
		if (q(max_steps - dy_rem4) >= 0) dy_ranges.push_back({0, q(max_steps - dy_rem4)});
	}
	if (dy_rem2 == 0 && dx <= max_steps && dy_rem4 / 2 == dx_rem2) {
		// Insert 0 to 2 * max_steps
		if (q(2 * max_steps - dy_rem4) >= 0) dy_ranges.push_back({0, q(2 * max_steps - dy_rem4)});
	}

	if ((dx_rem2 + dy_rem2) % 2 != max_steps % 2) max_steps--;
	int b = max(dx - max_steps, 1); if (b % 2 != dy_rem2) b++;

	// Case: dy = max_steps
	int a = 1; if (a % 2 != dx_rem2) a++;
	if (a + b <= max_steps) {
		if (max_steps % 4 == dy_rem4) dy_ranges.push_back({q(max_steps), q(max_steps)});
	}

	// Case: dy < max_steps
	a = 1; if (a % 2 != dx_rem2) a++;
	if (a + b <= max_steps) {
		// Insert 0 to max_steps - 1
		int st = 0;
		int en = q(max_steps - 1 - dy_rem4);
		if (st <= en) dy_ranges.push_back({st, en});
	}

	// Case: dy > max_steps
	int max_dy = 2 * max_steps - b - ((dy_rem2 + max_steps) % 2 != dx_rem2);
	// Insert max_steps + 1 to max_dy
	if (max_dy >= max_steps + 1) {
		int st = q(max_steps - dy_rem4) + 1;
		int en = q(max_dy - dy_rem4);
		if (st <= en) dy_ranges.push_back({st, en});
	}

	pair<vector<pii>, vector<pii>> ret;
	for (pii range : dy_ranges) {
		ret.x.push_back({
			q(from_y + range.x * 4 + dy_rem4),
			q(from_y + range.y * 4 + dy_rem4)
		});
		ret.y.push_back({
			q(from_y - (range.y * 4 + dy_rem4)),
			q(from_y - (range.x * 4 + dy_rem4))
		});
	}

	return ret;
}

int solve(int n, vector<int> x, vector<int> y, vector<int> k) {
	map<int, vector<pii>> mp[4];

	for (int i = 0; i < n; i++) {
		for (int _x = x[i] - 2 * k[i]; _x <= x[i] + 2 * k[i]; _x++) {
			for (int rem4 = 0; rem4 < 4; rem4++) {
				pair<vector<pii>, vector<pii>> ranges = reachableYs(y[i], abs(_x - x[i]), rem4, k[i]);
				copy(ranges.x.begin(), ranges.x.end(), back_inserter(mp[(y[i] + int(1e9 + 12) + rem4) % 4][_x]));
				copy(ranges.y.begin(), ranges.y.end(), back_inserter(mp[(y[i] + int(1e9 + 12) - rem4) % 4][_x]));
			}
		}
	}

	int ans = 0;

	for (int rem4 = 0; rem4 < 4; rem4++) {
		for (auto item : mp[rem4]) {
			vector<pii> ranges = item.y;
			map<int, int> cum;
			for (pii range : ranges) {
				cum[range.x]++;
				cum[range.y + 1]--;
			}
			int range_cnt = 0, last;
			for (pii p : cum) {
				if (range_cnt > 0) ans += p.x - last;
				last = p.x;
				range_cnt += p.y;
			}
		}
	}

	return ans;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	int n;
	while (cin >> n) {
		if (n == 0) break;

		vector<int> x(n), y(n), k(n);
		for (int i = 0; i < n; i++) scanf("%d %d %d", &x[i], &y[i], &k[i]);
		printf("%d\n", solve(n, x, y, k));
	}

	return 0;
}
