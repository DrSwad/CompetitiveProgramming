/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- http://agc007.contest.atcoder.jp/data/agc/007/editorial.pdf
*/

#include <bits/stdc++.h>
 
using namespace std;
 
#define debug(a) cerr << #a << ": " << a << endl
 
typedef long long ll;
typedef pair<int, int> ii;
 
#define x first
#define y second
 
const int N = 1e5 + 10;

int n, e, t;
int x[N];
ll dp[N];
ll suffix_min[N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif
 
	cin >> n >> e >> t;
	for (int i = 1; i <= n; i++) scanf("%d", &x[i]);
 
	suffix_min[n + 1] = 2 * x[n];
	dp[n] = t;
	suffix_min[n] = min(suffix_min[n + 1], dp[n] + 2 * x[n - 1]);
 
	for (int i = n - 1; i >= 1; i--) {
		ll &res = dp[i];
		int nxt = upper_bound(x + 1, x + n + 1, x[i] + t / 2) - x;
 
		if (nxt > n) res = t;
		else {
			res = dp[nxt] + t;
			res = min(res, suffix_min[nxt + 1] - 2 * x[i]);
		}
 
		suffix_min[i] = min(suffix_min[i + 1], res + 2 * x[i - 1]);
	}
 
	cout << dp[1] + e << endl;

	return 0;
}
