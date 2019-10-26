/*
	Author: Nayeemul Islam Swad

	Idea:
		- http://usaco.org/current/data/sol_sort_gold_open18.html
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

const int N = int(1e5) + 10;

int n;
pii a[N];
int ft[N];
void update(int at, int val) {
	for ( ; at <= n; at += (at & -at)) ft[at] += val;
}
int prefix_sum(int at) {
	int sum = 0;
	for ( ; at > 0; at -= (at & -at)) sum += ft[at];
	return sum;
}

int main() {
	// #ifdef LOCAL
	freopen("sort.in", "r", stdin);
	freopen("sort.out", "w", stdout);
	// #endif

	cin >> n;
	for (int i = 1; i <= n; i++) {
		scanf("%d", &a[i].x);
		a[i].y = i;
	}
	sort(a + 1, a + n + 1);
	int ans = 0;
	for (int i = 1; i <= n; i++) {
		update(a[i].y, 1);
		ans = max(ans, i - prefix_sum(i));
	}
	ans = max(ans, 1);
	cout << ans << endl;

	return 0;
}
