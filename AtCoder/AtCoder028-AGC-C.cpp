/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://img.atcoder.jp/agc028/editorial.pdf
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

const int N = int(1e5) + 10;

int n;
int a[N], b[N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d", &n);
	for (int i = 1; i <= n; i++) scanf("%d %d", &a[i], &b[i]);

	vector<pii> v(2 * n);
	for (int i = 1; i <= n; i++) {
		v[2 * (i - 1)] = make_pair(a[i], i);
		v[2 * (i - 1) + 1] = make_pair(b[i], i);
	}
	sort(v.begin(), v.end());

	vector<pii> p(n + 1);
	for (int i = 0; i < 2 * n; i++) {
		swap(p[v[i].y].x, p[v[i].y].y);
		p[v[i].y].y = i;
	}

	ll ans = LLONG_MAX;

	ll sum = 0LL;
	for (int i = 1; i <= n; i++) sum += a[i];
	ans = min(ans, sum);
	sum = 0LL;
	for (int i = 1; i <= n; i++) sum += b[i];
	ans = min(ans, sum);

	sum = 0LL;
	for (int i = 0; i < n - 2; i++) sum += v[i].x;
	for (int i = 1; i <= n; i++) {
		ll curr = sum + a[i] + b[i];
		if (p[i].x < n - 2) curr -= v[p[i].x].x, curr += v[n - 2].x;
		if (p[i].y < n - 1) curr -= v[p[i].y].x, curr += v[n - 1].x;
		ans = min(ans, curr);
	}

	printf("%lld\n", ans);

	return 0;
}
