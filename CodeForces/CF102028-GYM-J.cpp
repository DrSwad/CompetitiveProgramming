/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://codeforces.com/blog/entry/63729
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

const int N = int(3e5) + 10;
const int M = 1510;

int n, m;
ll cnt[M][M], sum[M][M], square[M][M];
int covers[N];
map<pii, int> pair_covers;
int total_covered, pair_max_covers;

void reset() {
	for (int row = 1; row <= m + 1; row++) {
		for (int col = 1; col <= m + 1; col++) {
			cnt[row][col] = 0;
			sum[row][col] = 0;
			square[row][col] = 0;
		}
	}
	for (int i = 1; i <= n; i++) covers[i] = 0;
	pair_covers.clear();
	total_covered = pair_max_covers = 0;
}

pii solveQuadratic(ll a, ll b, ll c) {
	ll z = (ll)sqrt(b * 1LL * b - 4LL * a * c);
	return make_pair((- b - z) / 2 / a, (- b + z) / 2 / a);
}

void cumulativeSum(ll arr[M][M], int row, int col) {
	arr[row][col] += (arr[row - 1][col] + arr[row][col - 1] - arr[row - 1][col - 1]);
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	int t;
	cin >> t;

	while (t--) {
		cin >> n >> m;
		for (int i = 1; i <= n; i++) {
			int row1, row2, col1, col2;
			scanf("%d %d %d %d", &row1, &row2, &col1, &col2);
			cnt[row1][col1] += 1; cnt[row2 + 1][col2 + 1] += 1;
			cnt[row2 + 1][col1] += -1; cnt[row1][col2 + 1] += -1;
			sum[row1][col1] += i; sum[row2 + 1][col2 + 1] += i;
			sum[row2 + 1][col1] += -i; sum[row1][col2 + 1] += -i;
			square[row1][col1] += i * 1LL * i;  square[row2 + 1][col2 + 1] += i * 1LL * i;
			square[row2 + 1][col1] += -i * 1LL * i;  square[row1][col2 + 1] += -i * 1LL * i;
		}

		for (int row = 1; row <= m; row++) {
			for (int col = 1; col <= m; col++) {
				cumulativeSum(cnt, row, col);
				cumulativeSum(sum, row, col);
				cumulativeSum(square, row, col);

				if (cnt[row][col] == 1) covers[sum[row][col]]++;
				else if (cnt[row][col] == 2) {
					ll a = 2LL;
					ll b = -2LL * sum[row][col];
					ll c = sum[row][col] * 1LL * sum[row][col] - square[row][col];
					pii sol = solveQuadratic(a, b, c);
					pair_covers[make_pair(sol.x, sol.y)]++;
				}

				total_covered += (cnt[row][col] > 0);
			}
		}

		int max1 = 0, max2 = 0;
		for (int i = 1; i <= n; i++) {
			if (covers[i] >= max1) {
				max2 = max1;
				max1 = covers[i];
			}
			else if (covers[i] >= max2) {
				max2 = covers[i];
			}
		}
		pair_max_covers = max1 + max2;

		for (auto item : pair_covers) {
			int got = covers[item.x.x] + covers[item.x.y] + item.y;
			pair_max_covers = max(pair_max_covers, got);
		}

		cout << total_covered - pair_max_covers << endl;

		reset();
	}

	return 0;
}
