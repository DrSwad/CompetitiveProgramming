/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://codeforces.com/blog/entry/63151
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

const int N = int(3e5) + 10;

int n;
ll k;
ll x[N], y[N];
ll minX, minY;

ll getMin(ll minX, ll minY, ll x, ll y) {
	if (minX == -1 && minY == -1) return -1;
	if (x > y) {
		ll startX = minY != -1 ? 0 : minX;
		ll extra = x - (k - startX + (y - 1) * k);

		if (extra > k) return -1;
		else return max(1LL, extra);
	}
	else {
		ll startY = minX != -1 ? 0 : minY;
		ll extra = y - (k - startY + (x - 1) * k);

		if (extra > 0) return -1;
		else return 1LL;
	}
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %lld", &n, &k);
	for (int i = 1; i <= n; i++) scanf("%lld", &x[i]);
	for (int i = 1; i <= n; i++) scanf("%lld", &y[i]);

	for (int i = 1; i <= n; i++) {
		scanf("%lld %lld", &x[i], &y[i]);
		ll _minX = getMin(minX, minY, x[i], y[i]);
		ll _minY = getMin(minY, minX, y[i], x[i]);
		minX = _minX;
		minY = _minY;
	}

	if (minX == -1 && minY == -1) puts("NO");
	else puts("YES");

	return 0;
}
