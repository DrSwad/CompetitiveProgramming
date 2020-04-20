/*
	Author: Nayeemul Islam Swad

	Idea:
		- Any arrangement of guards in the grid can be represented by a
		  bipartite graph with each node in the left/right component
		  representing a row/column in the grid and there's an edge
		  between a row node and a column node iff there's a guard in the
		  corresponding cell in the grid.

		  So the equivalent statement for this new representation is, we
		  need to find dp[k][n] which represents the number of ways to
		  assign edges in the bipartite graph with n nodes in each
		  component, such that all the nodes have degree 2 and there are
		  exactly k connected components.

		- Note that, each connected component must have an even size and
		  the connected component with size 2 * i must have i nodes in
		  of the components of the bipartite graph.

		- Let's take one of the nodes, suppose the nodes corresponding to
		  row 1 and call it r1. Now, consider the size of the connected
		  component that includes the node r1. Suppose it's size is 2 * i.
		  Then, among the rest (2 * i - 1) nodes apart from r1 in the
		  connected component, (i - 1) nodes come from the left component
		  and i nodes come from the right one. So, there are
		  [(n - 1) P (i - 1)] * [n P i] different connected components with
		  size 2 * i that includes r1.

		- For each connected component of size 2 * i that includes r1,
		  there are dp[k - 1][n - i] different ways for the rest of the
		  nodes to have edges among themselves. So,

		    dp[k][n] = [sum of [(n - 1) P (i - 1)] * [n P i] * dp[k - 1][n - i]], for all 1 <= i <= n
		             = [sum of (n - 1)! / (n - i)! * n! / (n - i)! * dp[k - 1][n - i]]
		             = n! * (n - 1)! * [sum of dp[k - 1][n - i] / [(n - i)!] ^ 2]

		  If we keep the prefix sum of dp[k - 1][n - i] / [(n - i)!] ^ 2
		  for 1 <= i <= n, then we can find dp[k][...] in O(N). So, we
		  can preprocess all possible states of the dp in O(K * N) and then
		  answer queries in O(1).
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
const int K = 52;
const int MOD = int(1e9) + 7;

int dp[K][N];
int inv[N], fact[N], ifact[N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	inv[1] = fact[0] = ifact[0] = 1;
	for (int i = 2; i < N; i++) inv[i] = (ll)inv[MOD % i] * (MOD - MOD / i) % MOD;
	for (int i = 1; i < N; i++) fact[i] = (ll)fact[i - 1] * i % MOD;
	for (int i = 1; i < N; i++) ifact[i] = (ll)ifact[i - 1] * inv[i] % MOD;

	dp[0][0] = 1;
	for (int k = 1; k < K; k++) {
		int sum = 0;
		for (int n = 2; n < N; n++) {
			sum += (ll)dp[k - 1][n - 2] * ifact[n - 2] % MOD * ifact[n - 2] % MOD;
			if (sum >= MOD) sum -= MOD;
			dp[k][n] = (ll)sum * fact[n] % MOD * fact[n - 1] % MOD * inv[2] % MOD;
		}
	}

	int t;
	cin >> t;

	for (int cs = 1; cs <= t; cs++) {
		int n, k;
		scanf("%d %d", &n, &k);
		printf("Case %d: %d\n", cs, dp[k][n]);
	}

	return 0;
}
