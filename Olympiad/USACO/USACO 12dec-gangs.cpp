/*
	Author: Nayeemul Islam Swad

	Idea:
		- Suppose currently gang `x` is ruling and a cow from a different gang
		  `y` came and confronted one cow from gang `x`.

		  Claim: `x` must be smaller than `y`.
		  Proof: Suppose `x` > `y` and there were `c` cows from gang `x` before
		    the confrontation. Then after the confrontation there would be
		    `c` - 1 cows remaining in the stack from gang `x`.

		    But another way of achiving the same effect would be: before inserting
		    any of the `c` cows from gang `x`, we could have just inserted a single
		    cow from gang `y`, and then confront it with a cow from gang `x` to make
		    the stack empty, and then insert the remaining `c` - 1 cows from gang `x`.

		    This ordering is certainly lexicographically smaller than the previous
		    one, but achieves the same final state.

		- Now, let's partition each gang of cows into two parts, `x_i` and `y_i` such
		  that `x_i` + `y_i` = `a[i]` and `x_i` cows from gang `i` is inserted while
		  this gang ruled, and the rest `y_i` cows is inserted to confront cows from
		  some other ruling gang.

		- Then our claim implies that, if `rem_one` is our answer of evaluating the
		  maximum number of cows we can have from gang 1 ruling in the end, then we
		  can construct the smallest legicographical ordering by:
		    * Insert `x_1` - `rem_one` cows in the stack to start with.
		    * Pop `y_2` cows from the stack. Notice that there must be `y_2` cows in
		      the stack, because cows from gang 2 can only be confronted with cows
		      from gang 1 as implied by our claim.
		    * If the stack is empty, then insert cows from gang 2. Otherwise, insert
		      the cows of gang 2 in a queue; these cows will be added to the stack
		      later as soon as the stack becomes empty.
		    * Again try to pop `y_3` cows from the stack. If the stack becomes empty
		      before removing `y_3` cows, then insert cows from gang 2 in the queue
		      to the stack and again remove rest of the `y_3` cows from the stack.
		    * Keep continuing in this manner. In the end the stack will be empty.
		      Then insert the remaining `rem_one` cows from gang 1 in the stack.

		- The only thing that remains is to figure out how to partition the gangs
		  into `x_i` and `y_i` so that after there exists an order in which if the
		  cows are inserted then the stack will be empty at the end.

		- To do that, we can use dp. Suppose, `poss[carry][at]` = `true` if, we're
		  given that (sum of `x_i` - sum of `y_i` = `carry`,  1 <= `i` < `at`), and
		  there is a way to partition gangs from `at` and onwards such that
		  (sum of `x_i` - sum of `y_i` = 0,  1 <= `i` <= `m`).

		- In order to evaluate `poss[carry][at]` we can interate over all possible
		  values of `x_at` and `y_at` and use `poss[(carry - y_at) + (x_at - y_at)][at + 1]`.
*/

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "debug.h"
#endif

typedef long long ll;

typedef pair<int, int> pii;
#define x first
#define y second

const int N = 110;

int n, m;
int a[N];
int poss[N][N];

// Checks if the stack can be made empty, provided that there are current `carry`
// elements in the stack after all the gangs upto `at` - 1 have been processed,
// and we're currently at gang `at`
bool DP(int carry, int at) {
	if (at == m + 1) return carry == 0;
	if (poss[carry][at] != -1) return poss[carry][at];

	for (int minus = min(a[at], carry); minus >= 0; minus--) {
		if (DP(carry - minus + a[at] - minus, at + 1)) return poss[carry][at] = true;
	}

	return poss[carry][at] = false;
}

int main() {
	// #ifdef LOCAL
	freopen("gangs.in", "r", stdin);
	freopen("gangs.out", "w", stdout);
	// #endif

	cin >> n >> m;
	for (int i = 1; i <= m; i++) scanf("%d", &a[i]);

	memset(poss, -1, sizeof poss);

	int rem_one;
	for (rem_one = a[1]; rem_one >= 1; rem_one--) {
		if (DP(a[1] - rem_one, 2)) break;
	}

	if (rem_one == 0) {
		printf("NO\n");
	}
	else {
		printf("YES\n");
		printf("%d\n", rem_one);

		// Check if this is a special case: If we should insert all of gang 1
		// at the start, and then remove the rest of the gangs
		if (a[1] - (n - a[1]) == rem_one) {
			for (int i = 1; i <= a[1]; i++) printf("1\n");
			for (int i = 2; i <= m; i++) {
				for (int j = 1; j <= a[i]; j++) printf("%d\n", i);
			}
			return 0;
		}

		queue<pair<int, int>> to_be_added; // Gangs that will be added as soon as the stack becomes empty
		int inserted = a[1] - rem_one, carry = inserted; // `inserted` = current size of the stack, `carry` = inserted + to_be_added.size()
		for (int i = 1; i <= inserted; i++) printf("1\n");

		for (int at = 2; at <= m; at++) {
			int minus;
			for (minus = min(carry, a[at]); minus >= 0; minus--) {
				if (DP(carry - minus + a[at] - minus, at + 1)) break;
			}

			int to_be_removed = minus;
			// While we have to remove cows from the stack using the current gang,
			// keep popping from the stack, and if the stack is empty but our removal
			// is not done, insert new cows from to_be_added
			while (to_be_removed > 0) {
				if (inserted <= to_be_removed) {
					for (int i = 1; i <= inserted; i++) printf("%d\n", at);
					to_be_removed -= inserted;
					if (to_be_added.empty()) {
						inserted = 0;
						break;
					}
					inserted = to_be_added.front().y;
					for (int i = 1; i <= inserted; i++) printf("%d\n", to_be_added.front().x);
					to_be_added.pop();
				}
				else {
					for (int i = 1; i <= to_be_removed; i++) printf("%d\n", at);
					inserted -= to_be_removed;
					to_be_removed = 0;
				}
			}

			carry -= minus; // `minus` cows have been removed from the stack using the current gang
			// Insert cows from the current gang
			if (a[at] > minus) {
				if (inserted == 0) {
					inserted += a[at] - minus;
					for (int i = 1; i <= inserted; i++) printf("%d\n", at);
				}
				else to_be_added.push({at, a[at] - minus});
				carry += a[at] - minus;
			}
		}

		while (rem_one--) printf("1\n");
	}

	return 0;
}
