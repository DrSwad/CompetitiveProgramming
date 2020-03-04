/*
	Author: Nayeemul Islam Swad

	Idea:
		- We'll use divide and conquer method.

		- For the merge process, we intially set `max_diff` = d[mid + 1] - d[mid].
		  Then as we're moving our left/right pointers L/R, we compare
		  diff(d[L], d[L + 1]) with diff(d[R], d[R - 1]) and whichever one is
		  smaller, we go that way, updating `max_diff` accordingly.

		- Also, as we're moving our two pointers to left or right, we also
		  maintain minimum prefix sum `prefL` for the left pointer and maximum
		  prefix sum `prefR` for the right pointer. So, as we're moving the
		  pointers, the best possible gain for the current position of the
		  two pointers would be (prefR - prefL - max_diff * max_diff).
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

const int N = int(3e5) + 10;

int n;
ll a;
ll d[N], c;
ll pref[N];

ll solve(int L, int R) {
	if (L == R) return max(0LL, pref[L] - pref[L - 1]);

	ll ret = 0LL;
	int mid = (L + R) / 2;
	ret = max(ret, solve(L, mid));
	ret = max(ret, solve(mid + 1, R));

	int ptrL = mid, ptrR = mid + 1;
	ll diff = 0;
	ll prefL = pref[mid], prefR = pref[mid];

	while (L <= ptrL || ptrR <= R) {
		if (R < ptrR || (L <= ptrL && (d[ptrL + 1] - d[ptrL] <= d[ptrR] - d[ptrR - 1]))) {
			prefL = min(prefL, pref[ptrL - 1]);
			diff = max(diff, d[ptrL + 1] - d[ptrL]);
			ptrL--;
		}
		else {
			prefR = max(prefR, pref[ptrR]);
			diff = max(diff, d[ptrR] - d[ptrR - 1]);
			ptrR++;
		}

		ret = max(ret, prefR - prefL - diff * diff);
	};

	return ret;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %lld", &n, &a);
	for (int i = 1; i <= n; i++) {
		scanf("%lld %lld", &d[i], &c);
		c = a - c;
		pref[i] = pref[i - 1] + c;
	}
	printf("%lld\n", solve(1, n));

	return 0;
}
