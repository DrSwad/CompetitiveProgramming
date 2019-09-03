/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- For each 0 <= mask < (1 << m), we count the sum of all price-products
		such that the AND of those prices is equal to mask and put the sum in
		and_cnt[mask]. Once we can initialize this array for all possible masks,
		computing the final answer can be done by just iterating over all the
		masks and doing,
			ans[__builtin_popcount(mask)] += and_cnt[mask]

		- Now, to count and_cnt[mask], we first compute and_cnt for all the
		supersets of mask and we also compute and_sum[mask] = sum of and_cnt
		for all supersets of mask including mask. Then,
			and_cnt[mask] = and_sum[mask] - [sum of and_cnt[supersets of mask]]

		- Here, notice that and_sum[mask] is just the product,
				(price_i1 + 1) * (price_i2 + 1) * ... * (price_ik + 1)
			where each price_i is such that the nutrition mask associated with
		it's product is a superset of our mask. Thus, this product can be
		calculated using SOS DP.

		- Also, For some mask, sum of and_cnt[supersets of mask] can be calculated
		using SOS DP.

		- Combining these two, we get and_cnt for all masks and thus the final answer.
*/

#include <bits/stdc++.h>

using namespace std;

#define debug(a) cerr << #a << ": " << a << endl

typedef long long ll;
typedef pair<int, int> ii;

#define x first
#define y second

const int M = 20, N = (1 << M) + 10;
const int MOD = 1e9 + 7;

int n, m;
int p[N];
int p_dp[N];
int sos_dp[N][M];
int and_cnt[N];
int ans[M + 1];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n >> m;
	fill(p, p + N, 1);
	for (int i = 1; i <= n; i++) {
		int price;
		char s[M + 1];
		scanf("%d %s", &price, s);

		bitset<M> bs(s);
		int mask = bs.to_ulong();
		p[mask] = p[mask] * 1LL * (1 + price) % MOD;
	}

	for (int mask = 0; mask < (1 << m); mask++) p_dp[mask] = p[mask];

	for (int i = 0; i < m; i++)
		for (int mask = (1 << m) - 1; mask >= 0; mask--) {
			if ((mask & 1 << i) == 0)
				p_dp[mask] = p_dp[mask] * 1LL * p_dp[mask ^ 1 << i] % MOD;
		}

	for (int mask = (1 << m) - 1; mask >= 0; mask--) {
		for (int i = 0; i < m; i++) {
			if (i == 0) {
				if ((mask & 1) == 0) {
					sos_dp[mask][0] = sos_dp[mask ^ 1][0];
				}
			}
			else {
				sos_dp[mask][i] = sos_dp[mask][i - 1];
				if ((mask & 1 << i) == 0) {
					sos_dp[mask][i] = (sos_dp[mask][i] + sos_dp[mask ^ 1 << i][i - 1]) % MOD;
				}
			}
		}

		and_cnt[mask] = (p_dp[mask] - sos_dp[mask][m - 1] + MOD - 1) % MOD;

		for (int i = 0; i < m; i++)
			sos_dp[mask][i] = (sos_dp[mask][i] + and_cnt[mask]) % MOD;
	}

	for (int mask = 0; mask < (1 << m); mask++) {
		int cnt = __builtin_popcount(mask);
		ans[cnt] = (ans[cnt] + and_cnt[mask]) % MOD;
	}

	for (int i = 0; i <= m; i++) cout << ans[i] << " ";

	return 0;
}
