/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://codeforces.com/blog/entry/72950
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

const int N = int(1e5) + 3;

int n;
bool vis[N];
int mu[N];
vector<int> divs[N];
int cnt[N];
ll ans;

int gcd(int a, int b) {
	if (a < b) swap(a, b);
	while (b) {
		int r = a % b;
		a = b;
		b = r;
	}
	return a;
}

void update(int at, int val) {
	for (int d : divs[at]) cnt[d] += val;
}

int coprime(int at) {
	int ret = 0;
	for (int d : divs[at]) ret += mu[d] * cnt[d];
	return ret;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	for (int i = 1; i < N; i++) {
		for (int j = i; j < N; j += i) {
			divs[j].push_back(i);
		}
		if (i == 1) mu[i] = 1;
		else if (divs[i / divs[i][1]][1] != divs[i][1]) mu[i] = -mu[i / divs[i][1]];
	}

	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		int a;
		scanf("%d", &a);
		vis[a] = true;
		ans = max(ans, (ll)a);
	}

	for (int g = 1; g < N; g++) {
		stack<int> st;
		for (int i = N / g; i >= 1; i--) {
			if (!vis[g * i]) continue;

			int c = coprime(i);
			if (c) {
				int num;
				while (c) {
					num = st.top();
					update(num, -1);
					st.pop();

					c -= gcd(i, num) == 1;
				}
				ans = max(ans, 1LL * g * i * num);
			}
			else {
				update(i, 1);
				st.push(i);
			}
		}
		while (!st.empty()) {
			update(st.top(), -1);
			st.pop();
		}
	}

	printf("%lld\n", ans);

	return 0;
}
