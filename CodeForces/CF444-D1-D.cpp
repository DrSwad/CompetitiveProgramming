/*
	Author: Nayeemul Islam Swad

	Idea:
		- For a string `sa` of length atmost 4, let's hash it with a 4-digit number
		  `a`. Simply, replace every character of `sa` with the order of the
		  corresponding digit and if |sa| < 4, then pad with 0s to fill up the rest
		  of the digits. For example, hash(aba) = 0121. Now, notice that there are
		  in total 27^4 such hashes.

		- For all possible hash `a` with corresponding string `sa`, precalculate the
		  vector `pos[a]` which stores the indexes of occurrences of `sa` in the
		  original string `s`.

		- Now, suppose we need to solve the query for (`sa`, `sb`) with corresponding
		  hash (`a`, `b`). First check if we already solved this pair earlier, if so
		  we're done, otherwise let's find it. WLOG assume that |pos[a]| <= |pos[b]|,
		  this is important. Then iterate over pos[a] to fix the position `i` of `sa`,
		  then using binary search on pos[b] find the closest occurrences of `sb` in
		  the original string after i and before i. Update the answer using these two
		  values.

		- After the iteration through pos[a], store the answer of this pair
		  ('sa', 'sb') in a structure to use it for later queries if needed.

		I have a rough sketch of proof that the total cost of iterations over pos[a]
		over all queries don't exceed O(N * sqrt(Q)). And because of the binary search,
		overall complexity is O(N * sqrt(Q) * log(N)).

		Here's the proof:

		Suppose the hashes are,
		    a_1, a_2, ..., a_k
		such that,
		    |pos[a_1]| <= |pos[a_2]| <= ... <= |pos[a_k]|

		Then, for a query (`sa`, `sb`) with hashes (`a`, `b`), the larger
		min(|pos[a]|, |pos[b]|) is the more costly the iteration is going to be. So,
		by greedily picking the queries, worst case scenario is,

		  q_1: (a_k, a_k)
		  q_2: (a_{k - 1}, a_k)
		  q_3: (a_{k - 1}, a_{k - 1})
		  q_4: (a_{k - 2}, a_k)
		  q_5: (a_{k - 2}, a_{k - 1})
		  q_6: (a_{k - 2}, a_{k - 2})
		  .
		  .
		  .

		So, the worst case total iterations is,
		   T = |pos[a_k]| * 1 + |pos[a_{k - 1}]| * 2 + |pos[a_{k - 2}]| * 3 + ... + |pos[a_{k - (l - 1)}]| * l, l <= SQRT(Q)

		Notice that, since |pos[a_{k - (l - 1)}]| holds the maximum weight, we should
		maximize it to (N / l). Continuing with this reasoning, we find that in the
		worst case |pos[a_k]| = |pos[a_{k - 1}]| = ... = |pos[a_{k - (l - 1)}]|.

		So, T = N / l * (1 + 2 + ... + l) = N * (l + 1) / 2 ~ N * SQRT(Q)

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

const int N = int(5e4) + 10;
const int M = int(6e5) + 10;

int n;
char s[N];
vector<int> pos[M];
map<pii, int> dp;

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%s", s + 1);
	n = strlen(s + 1);

	int p27[5];
	p27[0] = 1;
	for (int i = 1; i <= 4; i++) p27[i] = p27[i - 1] * 27;

	for (int len = 1; len <= 4; len++) {
		int num = 0;
		for (int i = 1; i <= n; i++) {
			num %= p27[len - 1];
			num *= 27;
			num += s[i] - 'a' + 1;
			if (i >= len) pos[num].push_back(i - len + 1);
		}
	}

	int q;
	cin >> q;

	while (q--) {
		string sa, sb;
		cin >> sa >> sb;

		int a = 0, b = 0;
		for (int i = 0; i < sa.length(); i++) a *= 27, a += sa[i] - 'a' + 1;
		for (int i = 0; i < sb.length(); i++) b *= 27, b += sb[i] - 'a' + 1;

		if (pos[a].size() > pos[b].size() ||
			(pos[a].size() == pos[b].size() && a > b)) {
			swap(sa, sb);
			swap(a, b);
		}

		if (dp.find(make_pair(a, b)) != dp.end()) printf("%d\n", dp[make_pair(a, b)]);
		else {
			int ans = n + 1;
			for (int i : pos[a]) {
				int _j = lower_bound(pos[b].begin(), pos[b].end(), i) - pos[b].begin();
				if (_j < pos[b].size()) {
					int j = pos[b][_j];
					int l = i;
					int r = max(i + sa.length() - 1, j + sb.length() - 1);
					ans = min(ans, r - l + 1);
				}

				_j--;
				if (_j >= 0) {
					int j = pos[b][_j];
					int l = j;
					int r = max(i + sa.length() - 1, j + sb.length() - 1);
					ans = min(ans, r - l + 1);
				}
			}

			if (ans > n) ans = -1;
			printf("%d\n", dp[make_pair(a, b)] = ans);
		}
	}

	return 0;
}
