/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://www.quora.com/What-is-an-algorithm-for-enclosing-the-maximum-number-of-points-in-a-2-D-plane-with-a-fixed-radius-circle/answer/Chun-Ho-Hung
*/

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "debug.h"
#endif

typedef long long ll;
typedef long double ld;

typedef pair<int, int> pii;
typedef pair<ld, ld> pld;
#define x first
#define y second

const int N = int(2e3) + 10;
const ld PI = acos(-1);
const ld eps = 1e-9;

int n, r;
pii p[N];

ld dist(pld p1, pld p2) {
	return hypotl(p1.x - p2.x, p1.y - p2.y);
}

int solve(int pId) {
	vector<pair<ld, int>> rList;
	for (int i = 0; i < n; i++) if (i != pId) {
		ld d = dist(p[pId], p[i]);
		if (d > 2 * r) continue;

		ld pAngle = atan2(p[i].y - p[pId].y, p[i].x - p[pId].x);
		if (pAngle < 0) pAngle += 2 * PI;
		ld dAngle = acos((d / 2) / r);
		ld rAngle1 = pAngle - dAngle;
		ld rAngle2 = pAngle + dAngle;

		rList.push_back({rAngle1, 1});
		rList.push_back({rAngle2, -1});
		for (ld delta = 2 * PI; rAngle2 + delta < 4 * PI; delta += 2 * PI) {
			rList.push_back({rAngle1 + delta, 1});
			rList.push_back({rAngle2 + delta, -1});
		}
		for (ld delta = -2 * PI; rAngle1 + delta >= -2 * PI; delta -= 2 * PI) {
			rList.push_back({rAngle1 + delta, 1});
			rList.push_back({rAngle2 + delta, -1});
		}
	}
	sort(rList.begin(), rList.end());

	int ret = 1, curr = 0;
	for (uint i = 0; i < rList.size();) {
		uint nxt = i;
		int plus = 0, minus = 0;
		while (nxt < rList.size() && fabs(rList[nxt].x - rList[i].x) < eps) {
			if (rList[nxt].y == 1) plus++;
			else minus++;
			nxt++;
		}
		i = nxt;

		curr += plus;
		ret = max(ret, 1 + curr);
		curr -= minus;
	}

	return ret;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	while (cin >> n >> r, n || r) {
		for (int i = 0; i < n; i++) {
			scanf("%d %d", &p[i].x, &p[i].y);
		}

		int ans = 0;
		for (int i = 0; i < n; i++) ans = max(ans, solve(i));
		cout << "It is possible to cover " << ans << " points." << endl;
	}

	return 0;
}
