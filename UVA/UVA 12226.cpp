/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://www.chegg.com/homework-help/simple-polygon-given-n-points-plane-draw-simple-polygon-n-po-chapter-2.5-problem-26e-solution-9780321573513-exc

		- A slight modification is needed to handle the cases of collinear
		  points.

		  After sorting the points into different groups according to their
		  angles with the lower-left point, sort each group according to
		  their distances from the lower-left point.

		  Then pick the first group in the order they appear, and reverse
		  the later groups.
*/

#include <bits/stdc++.h>

using namespace std;

typedef long double ld;
typedef pair<ld, ld> pld;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

const int N = int(2e3) + 10;
const ld eps = 1e-9;

pld p[N];

ld dist(int i, int j) {
	return hypotl(p[i].x - p[j].x, p[i].y - p[j].y);
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	int t;
	cin >> t;

	while (t--) {
		int n;
		scanf("%d", &n);

		int min_i = 0;
		for (int i = 0; i < n; i++) {
			scanf("%Lf %Lf", &p[i].x, &p[i].y);
			if (i > 0 && p[i] < p[min_i]) min_i = i;
		}

		printf("%d", min_i);

		vector<pair<ld, int>> ang;
		for (int i = 0; i < n; i++) if (i != min_i) {
			ang.push_back({atan2(p[i].y - p[min_i].y, p[i].x - p[min_i].x), i});
		}
		sort(ang.begin(), ang.end());

		for (int i = 0; i < ang.size();) {
			int nxt = i;
			vector<int> v;
			while (nxt < ang.size() && fabs(ang[nxt].x - ang[i].x) < eps) {
				v.push_back(ang[nxt].y);
				nxt++;
			}
			sort(v.begin(), v.end(), [min_i](int i, int j) {return dist(min_i, i) < dist(min_i, j);});
			if (i != 0) reverse(v.begin(), v.end());
			for (int ind : v) printf(" %d", ind);
			i = nxt;
		}

		printf("\n");
	}

	return 0;
}
