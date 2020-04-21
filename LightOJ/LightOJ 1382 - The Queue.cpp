/*
	Author: Nayeemul Islam Swad

	Idea:
		- Build a tree by making every employee node a child of his
		  supervisor node. Let dp[i] be the answer if we solve the same
		  problem by considering only the nodes in the subtree of i. Our
		  final answer is dp[1].

		- For every queue arrangement in dp[i], node i has to be at the
		  front, so his position is fixed. Notice that, for two different
		  children of i, c1 and c2, the position of any node from the
		  subtree of c1 in the queue doesn't affect the position of any
		  node from the subtree of c2. So, all the child-subtrees are
		  completely independent of each other.

		- So, if sz[i] denotes the size of subtree i, then leaving the
		  first position in the queue for i, we'll have (sz[i] - 1) positions
		  left. Every distinct distribution of these positions among the
		  child subtrees makes a different queue, and for each such
		  distribution a different arrangement among the nodes of a single
		  child-subtree will also make a different queue.

		  So, dp[i] = [number of ways to distribute (sz[i] - 1) positions among the child subtrees]
		  			  * [product of dp[children]]
		  			= [(sz[i] - 1) choose (sz[child1], sz[child2], ...)]
		  			  * dp[child1] * dp[child2] * ...
		  			= [factorial(sz[i] - 1)! / factorial(sz[child1]) / factorial(sz[child2]) / ...]
		  			  * dp[child1] * dp[child2] * ...
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

const int N = int(1e3) + 10;
const int MOD = int(1e9) + 7;

int n;
vector<int> adj[N];
int sz[N];
int inv[N], fact[N], ifact[N];

ll dfs(int at) {
	ll ret = 1LL;
	sz[at] = 1;
	for (int to : adj[at]) {
		(ret *= dfs(to) * ifact[sz[to]] % MOD) %= MOD;
		sz[at] += sz[to];
	}
	(ret *= fact[sz[at] - 1]) %= MOD;
	return ret;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	inv[1] = fact[0] = ifact[0] = 1;
	for (int i = 2; i < N; i++) inv[i] = (ll)inv[MOD % i] * (MOD - MOD / i) % MOD;
	for (int i = 1; i < N; i++) fact[i] = (ll)fact[i - 1] * i % MOD;
	for (int i = 1; i < N; i++) ifact[i] = (ll)ifact[i - 1] * inv[i] % MOD;

	int t;
	cin >> t;

	for (int cs = 1; cs <= t; cs++) {
		int n;
		scanf("%d", &n);

		vector<int> vis(n + 1);
		iota(vis.begin(), vis.end(), 0);

		for (int i = 1; i <= n - 1; i++) {
			int from, to;
			scanf("%d %d", &from, &to);
			vis[to] = 0;
			adj[from].push_back(to);
		}

		int root = accumulate(vis.begin(), vis.end(), 0);

		printf("Case %d: %lld\n", cs, dfs(root));

		for (int i = 1; i <= n; i++) {
			adj[i].clear();
			sz[i] = 0;
		}
	}

	return 0;
}
