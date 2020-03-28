/*
	Author: Nayeemul Islam Swad

	Idea:
		- Fix the last tree we'll cut, call it `target`. Now, notice that after
		  cutting any tree on the left of `target` we cannot move across `target`
		  to cut any tree on the right, we must move across `0` instead. Similarly
		  after cutting any tree on the right of `target`, we must move across `0`
		  to start cutting trees on the left.

		- Now, notice that, it's always optimal to:
		    * First go left or right and cut down a bunch of consecutive trees.
		    * Then keep changing directions after cutting a single tree.

		- So, we'll fix our `target`, which can take `N` possible values. Then,
		  figure out the answer for this `target` by:
		    * First cut down the excessive trees on left/right side so that both
		      sides now contain equal number of trees.
		    * Then start at left side/right side and keep changing directions
		      after cutting down a single tree. So, we'll have to handle two cases
		      here, but both are similar. They can be solved by keeping the
		      prefix sums and suffix sums of the tree coordinates. In the following
		      code the `solve` function handles this step.
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

int L, n;
int x[N];
vector<ll> psumL, psumR;

ll solve(vector<ll> &psumL, vector<ll> &psumR, int ls, int rs, int len) {
	return 2 * (psumL[ls + len - 1] - psumL[ls - 1]) + 2 * (psumR[rs + len - 1] - psumR[rs - 1]) - (psumR[rs + len - 1] - psumR[rs + len - 2]);
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d", &L, &n);
	for (int i = 1; i <= n; i++) scanf("%d", &x[i]);
	x[n + 1] = L;

	psumL.resize(n + 2, 0);
	psumR.resize(n + 2, 0);
	for (int i = 1; i <= n; i++) psumL[i] = psumL[i - 1] + x[i];
	for (int i = n; i >= 1; i--) psumR[n + 1 - i] = psumR[n - i] + (L - x[i]);

	ll ans = 0LL;
	for (int target = 1; target <= n; target++) {
		int left = target - 1;
		int right = n - target;
		if (left == 0) {ans = max(ans, (ll)(L - x[2]) + max(x[2] - x[1], L - (x[2] - x[1]))); continue;}
		if (right == 0) {ans = max(ans, (ll)x[n - 1] + max(x[n] - x[n - 1], L - (x[n] - x[n - 1]))); continue;}

		int rem = abs(left - right);
		if (left <= right) {
			ans = max(ans,
				2 * (L - x[n + 1 - rem]) +
				solve(psumL, psumR, 1, rem + 1, target - 1) +
				max(x[target + 1] - x[target], L - (x[target + 1] - x[target])));
			ans = max(ans,
				solve(psumR, psumL, rem + 1, 1, target - 1) +
				max(x[target] - x[target - 1], L - (x[target] - x[target - 1])));
		}
		else {
			ans = max(ans,
				2 * x[rem] +
				solve(psumR, psumL, 1, rem + 1, target - 1 - rem) +
				max(x[target] - x[target - 1], L - (x[target] - x[target - 1])));
			ans = max(ans,
				solve(psumL, psumR, rem + 1, 1, target - 1 - rem) +
				max(x[target + 1] - x[target], L - (x[target + 1] - x[target])));
		}
	}

	printf("%lld\n", ans);

	return 0;
}
