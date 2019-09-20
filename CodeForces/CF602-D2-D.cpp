/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://codeforces.com/blog/entry/21755
*/

#include <bits/stdc++.h>

using namespace std;

#define debug(a) cerr << #a << ": " << a << endl

typedef long long ll;
typedef pair<int, int> pii;

#define x first
#define y second

const int N = 1e5 + 10;

int n, q;
int a[N];
int d[N];
int L[N], R[N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n >> q;
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
	for (int i = 1; i < n; i++) d[i] = abs(a[i + 1] - a[i]);

	for (int i = 1; i < n; i++) {
		int at = i - 1;
		while (at > 0) {
			if (d[at] >= d[i]) break;
			at = L[at];
		}
		L[i] = at;
	}

	for (int i = n - 1; i >= 1; i--) {
		int at = i + 1;
		while (at < n) {
			if (d[at] > d[i]) break;
			else at = R[at];
		}
		R[i] = at;
	}

	while (q--) {
		int l, r;
		cin >> l >> r;

		ll ans = 0LL;
		for (int i = l; i < r; i++) {
			int _l = max(L[i], l - 1);
			int _r = min(R[i], r);

			ans += d[i] * 1LL * (i - _l) * (_r - i);
		}
		cout << ans << endl;
	}

	return 0;
}
