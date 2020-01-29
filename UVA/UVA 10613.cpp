/*
	Author: Nayeemul Islam Swad

	Idea:
		- Do a line sweep along the x-axis. Suppore we're at a certain x-coordinate
		  right now.

		- Given a particular circle, among all the cells that lie on this x-coordinate,
		  we'll find an interval of cells that lie inside the circle. Find out this
		  interval for each of the circles.

		- Finally find out the number of cells belonging to atleast one of these
		  intervals. This number will give us the answer for current x-coordinate.
		  After finishing the line sweep we'll have the answer for the whole grid.
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

const int N = int(1e6) + 10;

int size;
int n;
double x[N], y[N], r[N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	int t;
	cin >> t;

	while (t--) {
		cin >> size >> n;
		for (int i = 1; i <= n; i++) {
			scanf("%lf %lf %lf", &x[i], &y[i], &r[i]);
		}

		ll ans = 0LL;

		for (int x_left = 0; x_left <= size - 1; x_left++) {
			int x_right = x_left + 1;
			vector<pair<int, int>> delta_v;

			for (int c_id = 1; c_id <= n; c_id++) {
				double range_rad = -1;

				if (x_left <= x[c_id] && x[c_id] <= x_right) range_rad = r[c_id];
				else {
					double dist = x[c_id] < x_left ? x_left - x[c_id] : x[c_id] - x_right;
					if (0 <= dist) range_rad = sqrt(r[c_id] * r[c_id] - dist * dist);
				}

				if (0 <= range_rad) {
					int from, to;
					from = (int)max(0.0, y[c_id] - range_rad);
					double _to = y[c_id] + range_rad;
					if (fabs(_to - floor(_to)) <= 1e-7) // If _to is an integer
						to = (int)(_to - 1); // Don't include the highest cell
										   // since only it's boundary has
										   // been touched by the circle
					else to = (int)_to;
					to = min(size - 1, to);

					delta_v.push_back({from, 1});
					delta_v.push_back({to + 1, -1});
				}
			}

			delta_v.push_back({size, 0});
			sort(delta_v.begin(), delta_v.end());

			int prv = 0, at;
			int tot_circles = 0;

			for (int i = 0; i < (int)delta_v.size(); ) {
				at = delta_v[(uint)i].x;

				if (tot_circles > 0) ans += at - prv;

				while (i < (int)delta_v.size() && delta_v[(uint)i].x == at) {
					tot_circles += delta_v[(uint)i].y;
					i++;
				}

				prv = at;
			}
		}

		cout << ans << endl;
	}

	return 0;
}
