/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://img.atcoder.jp/agc043/editorial.pdf
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

const int N = int(1e5) + 10;
const int SQRTN = 500;
const int MOD = 998244353;

int n;
int m[3];
vector<int> adj[3][N];
int g[3][N];
int ans_g[4][SQRTN];
int mod[N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d", &n);
	for (int i = 0; i < 3; i++) {
		scanf("%d", &m[i]);
		for (int j = 0; j < m[i]; j++) {
			int u, v;
			scanf("%d %d", &u, &v);
			adj[i][u].push_back(v);
			adj[i][v].push_back(u);
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = n; j >= 1; j--) {
			vector<int> gs;
			for (int to : adj[i][j]) if (to > j) gs.push_back(g[i][to]);
			sort(gs.begin(), gs.end());
			gs.resize(unique(gs.begin(), gs.end()) - gs.begin());
			g[i][j] = 0;
			while (g[i][j] < gs.size() && gs[g[i][j]] == g[i][j]) g[i][j]++;
		}
	}

	int p = 1;
	for (int i = 0; i < 18; i++) p = p * 10LL % MOD;
	mod[0] = 1;
	for (int i = 1; i < N; i++) mod[i] = mod[i - 1] * 1LL * p % MOD;

	ans_g[3][0] = 1;
	for (int i = 2; i >= 0; i--) {
		for (int j = 1; j <= n; j++) {
			for (int _g = 0; _g < SQRTN; _g++) {
				int ig = _g ^ g[i][j];
				if (ig < SQRTN) {
					(ans_g[i][_g] += mod[j] * 1LL * ans_g[i + 1][ig] % MOD) %= MOD;
				}
			}
		}
	}

	cout << ans_g[0][0] << endl;

	return 0;
}
