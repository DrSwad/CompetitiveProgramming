/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://codeforces.com/blog/entry/67829
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "/Users/swad/Desktop/CP/debug.h"
#endif

const int MAXT = 2505;
const int MAXN = 55;
const int MOD = int(1e9) + 7;

int n, T;
int cnt[3];
vector<int> gen[3];
int gen_ways[3][MAXN][MAXT];
int gen2_ways[MAXN][MAXN][MAXT];
int take_ways[MAXN][MAXN][MAXN][3];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d", &n, &T);

	for (int i = 1; i <= n; i++) {
		int t, g;
		scanf("%d %d", &t, &g);
		g--;
		gen[g].push_back(t);
		cnt[g]++;
	}

	for (int g = 0; g < 3; g++) {
		gen_ways[g][0][0] = 1;

		for (int i = 0; i < cnt[g]; i++) {
			for (int take = cnt[g]; take >= 1; take--) {
				for (int sum = 0; sum < MAXT; sum++) {
					if (sum >= gen[g][i]) {
						gen_ways[g][take][sum] += gen_ways[g][take - 1][sum - gen[g][i]];

						if (gen_ways[g][take][sum] >= MOD) gen_ways[g][take][sum] -= MOD;
					}
				}
			}
		}
	}

	for (int take0 = 0; take0 <= cnt[0]; take0++) {
		for (int sum0 = 0; sum0 < MAXT; sum0++) {
			for (int take1 = 0; take1 <= cnt[1]; take1++) {
				for (int sum1 = 0; sum0 + sum1 < MAXT; sum1++) {
					gen2_ways[take0][take1][sum0 + sum1] +=
						(ll)gen_ways[0][take0][sum0] *
						gen_ways[1][take1][sum1] % MOD;

					if (gen2_ways[take0][take1][sum0 + sum1] >= MOD)
						gen2_ways[take0][take1][sum0 + sum1] -= MOD;
				}
			}
		}
	}

	take_ways[1][0][0][0] = take_ways[0][1][0][1] = take_ways[0][0][1][2] = 1;
	for (int take0 = 0; take0 <= cnt[0]; take0++) {
		for (int take1 = 0; take1 <= cnt[1]; take1++) {
			for (int take2 = 0; take2 <= cnt[2]; take2++) {
				if (take0 + take1 + take2 <= 1) continue;

				if (take0 > 0) {
					take_ways[take0][take1][take2][0] =
						(ll)take0 *
						(take_ways[take0 - 1][take1][take2][1] +
						take_ways[take0 - 1][take1][take2][2]) % MOD;
				}
				if (take1 > 0) {
					take_ways[take0][take1][take2][1] =
						(ll)take1 *
						(take_ways[take0][take1 - 1][take2][0] +
						take_ways[take0][take1 - 1][take2][2]) % MOD;
				}
				if (take2 > 0) {
					take_ways[take0][take1][take2][2] =
						(ll)take2 *
						(take_ways[take0][take1][take2 - 1][0] +
						take_ways[take0][take1][take2 - 1][1]) % MOD;
				}
			}
		}
	}

	int ans = 0;
	for (int take0 = 0; take0 <= cnt[0]; take0++) {
		for (int take1 = 0; take1 <= cnt[1]; take1++) {
			for (int take2 = 0; take2 <= cnt[2]; take2++) {
				for (int sum = 0; sum <= T; sum++) {
					int curr_take_ways = 0;
					for (int g = 0; g < 3; g++) {
						curr_take_ways += take_ways[take0][take1][take2][g];
						if (curr_take_ways >= MOD) curr_take_ways -= MOD;
					}

					ans += (ll)gen2_ways[take0][take1][sum] *
							gen_ways[2][take2][T - sum] % MOD
							* curr_take_ways % MOD;
					if (ans >= MOD) ans -= MOD;
				}
			}
		}
	}

	printf("%d\n", ans);

	return 0;
}
