/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://img.atcoder.jp/agc043/editorial.pdf
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

const int N = 2010;

int n;
ll dp[3 * N][N];
int MOD;

ll DP(int n, int rem) {
	if (dp[n][rem] >= 0) return dp[n][rem];
	if (n == 0) return dp[n][rem] = 1;

	ll ret = 0;
	if (n >= 1) (ret += DP(n - 1, rem)) %= MOD;
	if (rem >= 1) {
		if (n >= 2) (ret += DP(n - 2, rem - 1) * (n - 1)) %= MOD;
		if (n >= 3) (ret += DP(n - 3, rem - 1) * (n - 1) * (n - 2)) %= MOD;
	}
	return dp[n][rem] = ret;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n >> MOD;
	memset(dp, -1, sizeof dp);
	cout << DP(3 * n, n) << endl;

	return 0;
}
