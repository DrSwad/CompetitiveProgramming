/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- We use the following easy observation:
			In an optimal divisor tree only the root or a leaf can hold a value
			other than one of a[i].
		  Hence, our tree consists of the root, vertices with numbers a[i] tied
		  to each other or to the root, and leaves, which are tied to vertices
		  with numbers a[i] and contain these numbers' prime factorizations.

		- Let's consider each subset of the set of `n` numbers as a mask. Let,
		  dp[up_mask][down_mask], where `up_mask` and `down_mask` are disjoint,
		  be `true` if there exists a configuration where the numbers corresponding
		  to `up_mask` are direct children of the root and the numbers corresponding
		  to `down_mask` lies in subtrees of the `up_mask` numbers.

		- Now, to evaluate dp[up_mask][down_mask], we split `up_mask` into two
		  smaller sets,
		    `up_lsb` = least significant bit of `up_mask`, and
		    `up_rest` = `up_mask` ^ `up_lsb`.

		  Then, we iterate over all possible ways to split `down_mask` into two
		  partitions `down_mask1` and `down_mask2`, and check if we can put
		  `down_mask1` under `up_lsb` and `down_mask2` under `up_rest` by checking
		  if both dp[up_lsb][down_mask1] and dp[up_rest][down_mask2] are `true`.

		- To get our final answer, we first set our answer `ans` as the sum of the
		  exponents in prime factorizations of the given numbers.
		  We then iterate over all subsets of [(1 << n) - 1] as `up_mask` and set
		  `down_mask` = [(1 << n) - 1] ^ `up_mask`. If dp[up_mask][down_mask] is `true`,
		  then we consider it as a candidate answer and subtract the sum of exponents
		  in prime factorizations of the `down_mask` numbers from `ans`.

		- Total states in the dp is O(3^n) and considering transitions, time complexity
		  becomes O(4^n). And the memory complexity is O(4^n) for dp.
*/

#include<bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

#define x first
#define y second

const int N = 12, MAX_DIV = 1e6 + 5;

int n;
ll a[N];
bool dp[1 << N][1 << N];
int p_fact[N];
ll ans;

bool doesContain(int i, int mask) {
	ll x = a[i];
	for (int i = 0; i < n; i++) {
		if (mask & (1 << i)) {
			if (x % a[i] != 0)
				return false;

			x /= a[i];
		}
	}

	return true;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> a[i];

		ll x = a[i];
		for (int divi = 2; divi < min(x, (ll)MAX_DIV); divi++) {
			while (x % divi == 0) {
				p_fact[i]++;
				x /= divi;
			}
		}
		p_fact[i] += (x > 1);
	}

	for (int mask = 1; mask < 1 << n; mask++) {
		for (int up_mask = mask; up_mask > 0; up_mask = (up_mask - 1) & mask) {
			int down_mask = mask ^ up_mask;
			if (down_mask == 0) {
				dp[up_mask][down_mask] = true;
				continue;
			}

			int up_lsb = 1 << __builtin_ctz(up_mask);
			int up_rest = up_mask ^ up_lsb;
			if (up_rest != 0) {
				int down_mask1 = down_mask;
				do {
					int down_mask2 = down_mask ^ down_mask1;

					dp[up_mask][down_mask] = dp[up_mask][down_mask] || (dp[up_lsb][down_mask1] && dp[up_rest][down_mask2]);
					down_mask1 = (down_mask1 - 1) & down_mask;
				} while (down_mask1 != down_mask);
			}
			else {
				ll x;
				for (x = 0; x < n; x++) {
					if (up_lsb == 1 << x) break;
				}

				int down_mask1 = down_mask;
				do {
					int down_mask2 = down_mask ^ down_mask1;

					dp[up_mask][down_mask] = dp[up_mask][down_mask] || (doesContain(x, down_mask1) && dp[down_mask1][down_mask2]);
					down_mask1 = (down_mask1 - 1) & down_mask;
				} while (down_mask1 != down_mask);
			}
		}
	}

	int tot_p_fact = 0;
	for (int i = 0; i < n; i++) tot_p_fact += p_fact[i];

	int reduce_p_fact = -1;
	for (int up = 1; up < 1 << n; up++) {
		int down = ((1 << n) - 1) ^ up;
		if (dp[up][down]) {
			int curr_reduce = 0;
			for (int bit = 0; bit < n; bit++) {
				if (down & (1 << bit)) curr_reduce += p_fact[bit];
			}

			curr_reduce -= (up & (up - 1)) != 0;

			reduce_p_fact = max(reduce_p_fact, curr_reduce);
		}
	}

	ans = tot_p_fact - reduce_p_fact;

	for (int i = 0; i < n; i++) {
		bool isComposite = p_fact[i] > 1;
		ans += isComposite;
	}

	cout << ans << endl;

	return 0;
}
