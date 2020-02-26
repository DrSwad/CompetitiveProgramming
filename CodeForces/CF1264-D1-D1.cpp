/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://codeforces.com/blog/entry/71995
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef unsigned int uint;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

const int N = 2005;
const int MOD = 998244353;

int n;
char s[N];
int cnt[N];
int p[N];
int dp[N][N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%s", s + 1);
	n = strlen(s + 1);

	for (int i = 1; i <= n; i++) {
		cnt[i] = cnt[i - 1] + (s[i] == '?');
	}

	p[0] = 1;
	for (int i = 1; i < N; i++) {
		p[i] = 2 * p[i - 1];
		if (p[i] > MOD) p[i] -= MOD;
	}

	for (int len = 1; len < n; len++) {
		for (int i = 1; i + len <= n; i++) {
			int &currdp = dp[i][i + len];
			if (s[i] != '(') {
				currdp += dp[i + 1][i + len];
				if (currdp > MOD) currdp -= MOD;
			}
			if (s[i + len] != ')') {
				currdp += dp[i][i + len - 1];
				if (currdp > MOD) currdp -= MOD;
			}
			if (s[i] != '(' && s[i + len] != ')') {
				currdp -= dp[i + 1][i + len - 1];
				if (currdp < 0) currdp += MOD;
			}
			if (s[i] != ')' && s[i + len] != '(') {
				currdp += dp[i + 1][i + len - 1];
				if (currdp > MOD) currdp -= MOD;
				currdp += p[cnt[i + len - 1] - cnt[i]];
				if (currdp > MOD) currdp -= MOD;
			}
		}
	}

	printf("%d\n", dp[1][n]);

	return 0;
}
