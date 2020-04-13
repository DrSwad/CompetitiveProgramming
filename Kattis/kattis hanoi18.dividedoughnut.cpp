/*
	Author: Nayeemul Islam Swad

	Idea:
		- Let f(x) denote the total number of sprinkles in the parts
		  [x, x + 5e8). Then notice that,
		    abs(f(x) - f(x + 1)) <= 1,
		  since,
		    f(x + 1) = f(x) - a + b, where,
		    a = 0 or 1 depending on if part x has a sprinkle
		    b = 0 or 1 depending on if part x + 5e8 has a sprinkle

		- Now, since the two parts [0, 5e8) and [5e8, 1e9) are disjoint
		  and their union is [0, 1e9), we must have
		    * Either f(0) = f(5e8) = n / 2
		    * Or one of them is less than n / 2 and the other one greater
		      than n / 2

		- In the first case the answer is simply 0. So, let's solve the
		  second case.

		  WLOG, assume f(0) < n / 2 and f(5e8) > n / 2. Using the fact of
		  step 1, we see that there must be some 0 < x < 5e8 such that
		  f(x) = n / 2. To find out x we do a binary search in the range
		  (0, 5e8).

		- During the binary search, if we're currently at range [L, R], we
		  query f(mid = (L + R) / 2).
		    * If f(mid) = n / 2, then mid is our answer.
		    * If f(mid) < n / 2, then mid < x, so we update L = mid + 1.
		    * If f(mid) > n / 2, then x < mid, so we update R = mid - 1.

		  Here setting L = mid + 1 and R = mid - 1 is crucial, it makes
		  sure our binary search takes at most floor(log(1e9)) iterations.
		  We're using the fact that, as soon as we get f(mid) = n / 2, we
		  don't have to continue anymore. If we had to continue, then it
		  would've been identical to an ordinary binary search, and we had
		  to do ceil(log(1e9)) iterations in the worst case. It matters
		  because, floor(log(1e9)) = 29, combining it with the query we
		  need to output the final answer, we need exactly 30 queries in
		  the worst case; and 30 queries can be the limit when n is small
		  enough so that floor(log2(sqrt(n))) = 0.
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

const int N = int(1e9);

int n;

int query(int l, int r) {
	cout << "QUERY " << ((ll)l + N) % N << " " << ((ll)r + N) % N << endl;
	int x;
	cin >> x;
	return x;
}

void answer(int x) {
	cout << "YES " << x << endl;
	exit(EXIT_SUCCESS);
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n;

	int L, R;
	int x = query(0, N / 2 - 1);

	if (x == n / 2) answer(0);
	else if (x < n / 2) L = 1, R = N / 2 - 1;
	else L = N / 2 + 1, R = N - 1;

	while (L != R) {
		int mid = (L + R) / 2;
		int x = query(mid, mid + N / 2 - 1);
		if (x == n / 2) answer(mid);
		else if (x > n / 2) {
			R = mid - 1;
		}
		else {
			L = mid + 1;
		}
	}

	answer(L);

	return 0;
}
