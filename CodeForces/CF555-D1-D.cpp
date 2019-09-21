/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- Suppose, we are at the peg with id `at` and length of remaining rope is
		`len`. We'll roll the rope 360 degrees anti-clockwise, switching pegs in-
		between if needed. Now, after the complete roll, suppose the new position
		of the rope is at peg `at2` and remaining length is `len2`.

		- Case 1: If `at2` > `at`, then it can be shown that the new
		`len2` <= `len` / 2.

		- Case 2: If `at2` < `at`, then it can be shown that for all subsequent
		rolls, newer peg positions `at3` must satisfy `at2` <= `at3`, since
		there won't be enough remaining rope to go to the left of `at2`.
		So, we'll follow the same procedure for (`at2`, `len2`) as in (`at`, `len`),
		but this time we won't face Case 2.

		- Case 3: If `at2` == `at`, then let the length change after the roll be
		`lenChange` = (`len` - `len2`). Notice, that we'll keep rolling and coming
		back to this initial peg as long as the rope length is greater than or
		equal to `lenChange`. So we'll set `len` = `len` % `lenChange` and try to
		solve again. If after the change, `len` becomes 0 then the initial is the
		the answer, otherwise after the next roll `at2` must be greater than `at1`
		and from Case 1 we know that the length will then be at most half of current
		length.

		- Thus for each case, we can translate it to Case 1 with no more than one
		extra roll, and thus half the rope length.
*/

#include <bits/stdc++.h>

using namespace std;

#define debug(a) cerr << #a << ": " << a << endl

typedef long long ll;
typedef pair<int, int> pii;

#define x first
#define y second

const int N = 2e5 + 10;

int n, m;
int x[N];
int qid[N], xid[N];

pii loopOnce(int at, int len) {
	int at1 = upper_bound(x + 1, x + n + 1, x[at] + len) - x - 1;
	int len1 = len - (x[at1] - x[at]);

	int at2 = lower_bound(x + 1, x + n + 1, x[at1] - len1) - x;
	int len2 = len1 - (x[at1] - x[at2]);

	return {at2, len2};
}

int solve(int at, int len) {
	pii p = loopOnce(at, len);

	if (p.x != at) return solve(p.x, p.y);

	int lenChange = len - p.y;
	if (lenChange == 0) return at;
	len %= lenChange;
	return solve(at, len);
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif
	
	cin >> n >> m;
	for (int i = 1; i <= n; i++) scanf("%d", &x[i]);

	iota(qid + 1, qid + n + 1, 1);
	sort(qid + 1, qid + n + 1, [](int i, int j) {return x[i] < x[j];});
	for (int i = 1; i <= n; i++) xid[qid[i]] = i;

	sort(x + 1, x + n + 1);

	while (m--) {
		int at, len;
		scanf("%d %d", &at, &len);
		printf("%d\n", qid[solve(xid[at], len)]);
	}

	return 0;
}
