/*
	Author: Nayeemul Islam Swad

	Idea:
		- All of the triangles must be of the form (2^i, 2^j, 2^j), i <= j.

		- We will focus on collecting as many pairs (2^j, 2^j) as we can
		  so that, later on, if some 2^i is isolated alone then we can
		  use one our collected pairs with it to form a triangle. This
		  greedy strategy won't cause a problem, because in the end if
		  we have a lot of pairs, say `p` pairs, unused then we can
		  always make `(2 * p) / 3` triangles using them:
		    * Start from the smallest power of two unused and match it
		      with a pair of highest power two unused.
		    * Keep doing this until we have a number of unused elements
		      all of the same power. Then use triples of them.

		- So, our strategy should be:
		  * Iterate from the end
		  * If the there's only one instance of current power then we
		    must match it with a pair already collected. If there's no
		    such pair then skip it, otherwise use one of those pairs and
		    form a triangle.
		  * But if there's more than one instance, then form as many pairs
		    as possible and create triples from the extras.
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

const int N = int(3e5) + 10;

int n;
int a[N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n;
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);

	ll twos = 0;
	ll ans = 0;
	for (int i = n; i >= 1; i--) {
		if (a[i] == 1) {
			if (twos > 0) {
				ans++;
				twos--;
			}
		}
		else {
			if (a[i] % 2 == 1) {
				a[i] -= 3;
				ans++;
			}
			twos += a[i] / 2;
		}
	}
	ans += (2 * twos) / 3;
	cout << ans << endl;

	return 0;
}
