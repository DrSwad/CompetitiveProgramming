/*
	Author: Nayeemul Islam Swad

	Idea:
		- The observations are same as in the official editorial reference below,
		  but we'll optimize the idea differently:
		    Official editorial: https://github.com/mostafa-saad/MyCompetitiveProgramming/blob/master/Olympiad/Baltic/official/boi2010_solutions/candies-sol.pdf

		- The second step of the editorial can be done with a simple knapsack in
		  O(B * N ^ 2). But doing the first step using usual knapsack results in
		  O(B * N ^ 3) which would be too slow, but using bitsets speeds it up
		  enough to pass within the TL comfortably.
*/

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "/Users/swad/Desktop/CP/debug.h"
#endif

const int N = 105;
const int S = int(7e5) + 5;

int n;
int a[N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d", &n);
	for (int i = 0; i < n; i++) scanf("%d", &a[i]);

	int mx, cnt = -1;
	for (int i = 0; i < n; i++) {
		bitset<S> bset;
		bset.set(0);
		for (int j = 0; j < n; j++) if (j != i) bset |= bset << a[j];
		if ((int)bset.count() > cnt) {
			mx = i;
			cnt = bset.count();
		}
	}

	bitset<2 * S> bset;
	bset.set(S);
	for (int i = 0; i < n; i++) if (i != mx) {
		bset |= (bset << a[i]) | (bset >> a[i]);
	}

	int b = 1;
	while (bset.test(S + b)) b++;

	printf("%d %d\n", a[mx], b);

	return 0;
}
