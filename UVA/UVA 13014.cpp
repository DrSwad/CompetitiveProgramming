/*
	Author: Nayeemul Islam Swad

	Idea:
		- Iterate from the last level to the first, find out the lowest
		  cost needed if we start at that level and insert them into some
		  structure that allows range query.

		- While iterating, suppose we're at level `at`. Consider all the shops
		  that are located at this level. Then, for each shop, compute the
		  number of levels that can be handled if we purchase from this shop
		  using `c` cost. Let this number be `jump`. Then, find the minimum
		  cost `mini` among all the levels from `at + 1` to `at + jump` and
		  consider `mini + c` as a candidate cost for the current level `at`.
*/

#include <bits/stdc++.h>

using namespace std;

typedef unsigned int uint;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

const int N = int(1e5) + 10;

int n, m;
int e[N], pref[N];
vector<pii> shops[N];
vector<pii> cost;

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	while (cin >> n >> m) {
		for (int i = 1; i <= n; i++) {
			scanf("%d", &e[i]);
			pref[i] = pref[i - 1] + e[i];
		}
		for (int i = 1; i <= m; i++) {
			int l, s, c;
			scanf("%d %d %d", &l, &s, &c);
			shops[l].push_back({s, c});
		}

		cost.clear();
		cost.push_back({-(n + 1), 0});

		for (int at = n; at >= 1; at--) {
			int curr_cost = -1;

			for (pii p : shops[at]) {
				int s = p.x;
				int c = p.y;

				int L = 0, R = n - (at - 1);
				while (L != R) {
					int mid = (L + R + 1) / 2;
					if (pref[(at - 1) + mid] - pref[at - 1] <= s) L = mid;
					else R = mid - 1;
				}

				int jump = L;
				if (jump == 0) continue;
				uint min_at = (uint)(lower_bound(cost.begin(), cost.end(), make_pair(-(at + jump), 0)) - cost.begin());
				if (min_at == cost.size()) continue;

				if (curr_cost == -1 || c + cost[min_at].y < curr_cost) curr_cost = c + cost[min_at].y;
			}

			if (curr_cost != -1) {
				while (!cost.empty() && cost.back().y >= curr_cost) cost.pop_back();
				cost.push_back({-at, curr_cost});
			}
		}

		if (cost.back().x != -1) printf("-1\n");
		else printf("%d\n", cost.back().y);

		for (int i = 1; i <= n; i++) shops[i].clear();
	}

	return 0;
}
