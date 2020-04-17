/*
	Author: Nayeemul Islam Swad

	Idea:
		- Let's first fix a destination cell (dx, dy) and find out if it's
		  reachable by a single knight with at most k moves starting at
		  origin (0, 0). WLOG dx, dy >= 0.

		- In each move, the knight can go
		  1 unit horizontally/vertically and 2 units vertically/horizontally.
		  Let, h1, h2 be the number of horizontal 1 unit and 2 units moves,
		  respectively. Similarly define v1, v2. The conditions we get on k,
		  h1, h2, v1, v2 are:

		    * h1 = v2
		    * h2 = v1
		    * h1 + h2 <= k

		- Notice that, among the h1 horizontal 1 unit moves, some are +1
		  and some are -1. Same for h2, v1, v2.Using this fact, and a
		  little math, we can prove the following:

		  Lemma: With h1 1-units and h2 2-units -

		    * dx = h1 (mod 2) must hold.
		    * If h1 > 0, then the knight can reach any horizontal coordinate dx upto (h1 + 2 * h2).
		    * If h1 = 0, then any dx upto 2 * h2 is reachable such that dx = 2 * h2 (mod 4).

		- So, to check if (dx, dy) is reachable our algorithm should be:

		  * Case 1: h1 = 0.
		    Check if dx = h1 = 0 (mod 2), if not then this case is
		    impossible.
		    Check if dy = h2 = dx / 2 (mod 2), if not then this case is
		    impossible.
		    Next, h2 >= dx / 2 and h2 >= dy. So, set h2 = max(dx / 2, dy).
		    Now, check if h2 <= k holds. If it does then (dx, dy) is
		    reachable otherwise not.

		  * Case 2: h2 = 0.
		    Similar to the one above.

		  * Case 3: h1, h2 >= 1.
		    Notice that, increasing h1 or h2 would also increase
		    (h1 + 2 * h2) and (h2 + 2 * h1) in our Lemma. So, we should
		    increase them as much as possible. Which means that h1 + h2
		    should be equal to k. But if dx + dy = h1 + h2 != k (mod 2),
		    then it's impossible to use all of k, so in that case decrease
		    k by 1. Now, we may assume that h1 + h2 = k.

		  From the lemma,
		     dx <= h1 + 2 * h2 = k + h2
		  => h2 >= dx - k
		  So, set h2 = dx - k and if h2 = dy (mod 2) doesn't hold then
		  increase it by 1. This is the lower bound for h2. Similarly
		  we get a lower bound for h1. Then check if h1 + h2 <= k holds.
		  If it does, then (dx, dy) is reachable otherwise not.

		That's the algorithm.

		- Now, for each of the knights we can reach at most 2 * k distance
		  horizontally and vertically from it. So, we can check all the
		  (4 * k) ^ 2 cells around it if they're reachable from this
		  knight in O(1) and if we do this for every knight then we'll
		  have all the reachable cells. Unfortunately my submission with
		  this approach received TLE for some reason. So we need to
		  optimize it more.

		- Notice that, in our reachability checking algorithm if we fix dx
		  and the remainder of dy (mod 4) then we get at most 5 intervals
		  for reachable dy. This will be our opmiziation. For each knight
		  (kx, ky) we move through the rows kx - 2 * k to kx + 2 * k and
		  for each row we find the intervals of reachable y coordinates
		  from that knight. After doing this for all the knights, we'll get
		  some intervals of reachable y coordinates in each row. We'll
		  merge these intervals in each row to get the number of reachable
		  cells for that row. Adding this count over all rows will give us
		  the answer.
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
