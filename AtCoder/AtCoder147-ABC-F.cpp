/*
	Author: Nayeemul Islam Swad

	Idea:
		- Let's fix `s` as the number of elements we'll take from S. Then for
		  any such S and corresponding T, we can represent S - T as,
		    S - T = x * (s - (n - s)) + diff * d,
		  where `diff` is the difference between the sum of indices of elements
		  in S and in T.

		- Now, notice that the smallest possible value of `diff` is when the
		  indices in S are {1, 2, ..., s} and in T are {s + 1, ..., n}. So,
		  `diff_min` = (1 + 2 + ... + s) - (s + 1 + ... + n).

		  And it is maximum when the indices in S are {n, n - 1, ..., n - s + 1}
		  and in T are {n - s, ..., 1}. So, `diff_max` = (n + ... + n - s + 1) -
		  (n - s + ... + 1).

		  Also, `diff` can take any value in-between these `diff_min` and
		  `diff_max` on the condition that the parity of `diff` needs to
		  be the same as of `diff_min` and `diff_max`.

		- So, we fix `s` as the number of elements we'll take from S, then,
		    S - T = x * (s - (n - s)) + diff * d = X + diff * d
		  Thus, we need to include all the elements of format X + diff * d,
		  where, diff_min <= diff <= diff_max and parity(diff) = parity(diff_min),
		  to our answer.

		- To do this quickly, we will first find out all these pairs
		  (X + diff_min * d, X + diff_max * d) = (p, q) for all possible `s`.
		  Then notice that elements in two such ranges might overlap only if
		  their p values yield the same remainder upon division by (2 * d).

		  So, we can first sort the pairs first and them process them in increasing
		  order of p, and for all possible remainder p % (2 * d), maintaining the
		  highest q we've found for that remainder.
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

const int N = int(2e5) + 10;

ll n, x, d;
ll pref[N], suff[N];
map<ll, ll> series_from;

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n >> x >> d;

	if (d == 0) {
		if (x == 0) cout << 1 << endl;
		else cout << n + 1 << endl;
		return 0;
	}

	if (d < 0) {
		x += d * (n - 1);
		d *= -1;
	}

	for (int i = 1; i <= n; i++) {
		pref[i] = pref[i - 1] + (i - 1);
		suff[i] = suff[i - 1] + (n - i);
	}

	vector<pair<ll, ll>> seriesv(n + 1);

	for (int i = 0; i <= n; i++) {
		ll add = x * (i - (n - i));
		ll L = d * (pref[i] - suff[n - i]);
		ll R = d * (suff[i] - pref[n - i]);

		seriesv[i] = make_pair(add + L, add + R);
	}

	sort(seriesv.begin(), seriesv.end());

	ll ans = 0LL;
	d *= 2;

	for (pair<ll, ll> p : seriesv) {
		ll l = p.x, r = p.y;
		ll rem = (l % d + d) % d;

		ll R = LLONG_MIN;
		if (series_from.find(rem) != series_from.end()) {
			R = series_from[rem];
		}

		if (R < r) {
			l = max(l, R + d);
			ans += (r - l + d) / d;
			series_from[rem] = r;
		}
	}

	cout << ans << endl;

	return 0;
}
