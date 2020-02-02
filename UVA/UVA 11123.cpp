/*
	Author: Nayeemul Islam Swad

	Idea:
		- First, find out all the different lines among those points. Then sort
		  them wrt thier slopes. Now, using two pointer technique, we can iterate
		  over intervals of lines with equal slopes.

		- Now, if we are to find a trapezoid or parallelogram, two pairs of it's
		  vertices must lie on two different lines with equal slope.

		- So, at each step, we take an interval of lines with equal slopes. Then
		  using inclusion-exclusion principle, find out the number of quads with
		  at least two oppositie sides parallel and also the number of parallel-
		  ograms. Subtracting the later from the former yields the number of
		  trapezoids consisting of points only from that interval of lines.

		- Summing up all these numbers for all intervals of lines of equal slopes
		  we get our answer.
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef unsigned int uint;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

const int N = 205;

int n;
pii p[N];
map<pii, int> slope_from_point[N];
int ans;

struct line {
	pii slope;
	vector<int> p_ids;
	map<pii, int> dist_cnt;
	bool operator<(const line &l2) const {
		return slope < l2.slope;
	}
};

vector<line> lines;

int gcd(int a, int b) {
	a = abs(a), b = abs(b);
	if (a < b) swap(a, b);
	while (b != 0) {
		int r = a % b;
		a = b;
		b = r;
	}
	return a;
}

pii normalizedDistance(int i, int j) {
	int dx = p[i].x - p[j].x;
	int dy = p[i].y - p[j].y;
	if (dx < 0) dx = -dx, dy = -dy;
	if (dx == 0) dy = abs(dy);
	return make_pair(dx, dy);
}

pii normalizedSlope(int i, int j) {
	pii dist_p = normalizedDistance(i, j);
	if (dist_p == make_pair(0, 0)) return dist_p;

	int dx = dist_p.x, dy = dist_p.y;
	int dg = gcd(dx, dy);
	return make_pair(dx / dg, dy / dg);
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	for (int cs = 1; true; cs++) {
		scanf("%d", &n);
		if (n == 0) break;

		for (int i = 1; i <= n; i++) {
			scanf("%d %d", &p[i].x, &p[i].y);
		}

		for (int i = 1; i <= n; i++) {
			for (int j = i + 1; j <= n; j++) {
				pii slope = normalizedSlope(i, j);
				if (slope_from_point[i].find(slope) != slope_from_point[i].end()) {
					int line_id = slope_from_point[i][slope];
					if (slope_from_point[j].find(slope) == slope_from_point[j].end()) {
						lines[(uint)line_id].p_ids.push_back(j);
						slope_from_point[j][slope] = line_id;
					}
				}
				else {
					lines.push_back(line());
					int line_id = (int)lines.size() - 1;
					lines[(uint)line_id].slope = slope;
					lines[(uint)line_id].p_ids.push_back(i);
					lines[(uint)line_id].p_ids.push_back(j);
					slope_from_point[i][slope] = line_id;
					slope_from_point[j][slope] = line_id;
				}
			}
		}

		sort(lines.begin(), lines.end());

		for (line &l : lines) {
			for (int i = 0; i < (int)l.p_ids.size(); i++) {
				for (int j = i + 1; j < (int)l.p_ids.size(); j++) {
					pii dist = normalizedDistance(l.p_ids[(uint)i], l.p_ids[(uint)j]);
					l.dist_cnt[dist]++;
				}
			}
		}

		ans = 0;

		for (int l = 0; l < (int)lines.size(); ) {
			int r = l;
			pii slope = lines[(uint)l].slope;

			while (r < (int)lines.size() && lines[(uint)r].slope == slope) r++;

			int trapezoid_cnt = 0;
			int tot_pairs = 0;
			for (int i = l; i < r; i++) {
				int curr_points = (int)lines[(uint)i].p_ids.size();
				int curr_pairs = curr_points * (curr_points - 1) / 2;
				tot_pairs += curr_pairs;
				trapezoid_cnt -= curr_pairs * (curr_pairs - 1) / 2;
			}
			trapezoid_cnt += tot_pairs * (tot_pairs - 1) / 2;

			int parallelogram_cnt = 0;
			map<pii, int> all_dist_cnt;
			for (int i = l; i < r; i++) {
				for (auto item : lines[(uint)i].dist_cnt) {
					pii dist = item.first;
					int cnt = item.second;
					parallelogram_cnt -= cnt * (cnt - 1) / 2;
					all_dist_cnt[dist] += cnt;
				}
			}
			for (auto item : all_dist_cnt) {
				int cnt = item.second;
				parallelogram_cnt += cnt * (cnt - 1) / 2;
			}

			ans += trapezoid_cnt - parallelogram_cnt;

			l = r;
		}

		printf("Case %d: %d\n", cs, ans);

		// reset everything
		for (int i = 1; i <= n; i++) slope_from_point[i].clear();
		lines.clear();
	}

	return 0;
}
