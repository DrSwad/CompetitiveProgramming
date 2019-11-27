/*
	Author: Nayeemul Islam Swad

	Idea:
		- Sort the points wrt their angles.

		- For each point, try to divide the plane by connecting it with the
		  origin and check if this division satisfies the conditions.
*/

#include <bits/stdc++.h>

using namespace std;

typedef long double ld;
typedef pair<ld, ld> pld;

#ifdef LOCAL
#include "debug.h"
#endif

const int N = int(3e4) + 10;
const ld PI = acos(-1);

struct point {
	ld x, y;
	int type;
	ld angle;
	bool operator < (const point &p2) const {
		return angle < p2.angle;
	}
};

int n1, n2;
point p[2 * N];
int cnt[2];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	while (true) {
		scanf("%d %d", &n1, &n2);
		if (n1 == -1 || n2 == -1) break;
		int n = n1 + n2;

		for (int i = 0; i < n1; i++) {
			scanf("%Lf %Lf", &p[i].x, &p[i].y);
			p[i].angle = atan2(p[i].y, p[i].x);
			p[i].type = 0;
		}
		for (int i = n1; i < n; i++) {
			scanf("%Lf %Lf", &p[i].x, &p[i].y);
			p[i].angle = atan2(p[i].y, p[i].x);
			p[i].type = 1;
		}
		for (int i = 0; i < n; i++) {
			p[n + i].type = p[i].type;
			p[n + i].angle = p[i].angle + 2 * PI;
		}
		n *= 2;

		sort(p, p + n);

		bool flag = false;
		cnt[0] = cnt[1] = 0;
		for (int l = 0, r = 0; l < n && p[l].angle <= PI && !flag; l++) {
			while (r < n && p[r].angle - p[l].angle < PI) {
				cnt[p[r].type]++;
				r++;
			}

			if (cnt[0] == n1 / 2 && cnt[1] == n2 / 2) {
				flag = true;
				printf("YES\n");
				break;
			}

			cnt[p[l].type]--;
		}

		if (!flag) printf("NO\n");
	}

	return 0;
}
