/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://codeforces.com/blog/entry/9210
*/

#include <bits/stdc++.h>
 
using namespace std;
 
#define debug(a) cerr << #a << ": " << a << endl
 
typedef long long ll;
typedef pair<int, int> ii;
 
#define x first
#define y second
 
const int N = 1e6 + 10;

ll n, m;
char x[N], y[N];
int lenx, leny;
int cnt[N][26];

ll gcd(ll a, ll b) {
	if (a < b) swap(a, b);

	while (b != 0) {
		ll r = a % b;
		a = b;
		b = r;
	}

	return a;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n >> m;
	scanf(" %s %s", x, y);
	lenx = strlen(x);
	leny = strlen(y);

	if (lenx > leny) {
		swap(n, m);

		for (int i = 0; i < lenx; i++)
			swap(x[i], y[i]);

		swap(lenx, leny);
	}

	ll G = gcd(lenx, leny);
	ll L = lenx * 1LL * leny / G;

	for (int i = 0; i < leny; i++) {
		cnt[i % G][y[i] - 'a']++;
	}

	ll ans = 0LL;
	for (int i = 0; i < lenx; i++) {
		ans += cnt[i % G][x[i] - 'a'];
	}

	ans = n * 1LL * lenx - ans * (lenx * 1LL * n / L);

	cout << ans << endl;

	return 0;
}
