/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://img.atcoder.jp/agc027/editorial.pdf
*/

#include <bits/stdc++.h>
 
using namespace std;
 
#define debug(a) cerr << #a << ": " << a << endl
 
typedef long long ll;
typedef pair<int, int> ii;
 
#define x first
#define y second
 
const int N = 2e5 + 10;

ll n;
ll x[N];
ll X;
ll pref[N];

ll solve(ll len) {
	ll ret = 0LL;
	ll cnt = 1LL;

	ret += (pref[n] - pref[n - len]);
	
	for (ll r = n; r > 0; r -= len, cnt++) {
		ll l = max(0LL, r - len);
		
		ll add;
		add = ((pref[r] - pref[l]) - x[l] * (r - l)) * (cnt + 1) * (cnt + 1);
		if (add < 0) return LLONG_MAX;
		ret += add;
		if (ret < 0) return LLONG_MAX;
		if (r < n) {
			add = (x[r] * len - (pref[r] - pref[l])) * cnt * cnt;
			if (add < 0) return LLONG_MAX;
			ret += add;
			if (ret < 0) return LLONG_MAX;
		}
	}

	ret += X * n;
	if (ret < 0) return LLONG_MAX;
	ret += X * len;
	if (ret < 0) return LLONG_MAX;

	return ret;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif
 
	cin >> n >> X;
	for (int i = 1; i <= n; i++) {
		scanf("%lld", &x[i]);
		pref[i] = pref[i - 1] + x[i];
	}

	ll ans = LLONG_MAX;
	for (int i = 1; i <= n; i++) {
		ans = min(ans, solve(i));
	}

	cout << ans << endl;
 
	return 0;
}
