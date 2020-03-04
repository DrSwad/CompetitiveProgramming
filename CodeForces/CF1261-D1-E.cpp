/*
	Author: Nayeemul Islam Swad

	Idea:
		- The problem can be restated as follows: We are given a (n + 1) x n
		  grid and we have to place a_i '1's in the i'th column. After placing
		  all the '1's, the rows must be pairwise distinct.

		- We process column by column. Let's start at the first column. Place
		  all the a_1 '1's in the first a_1 rows. So, at the moment {1, ..., a_1}
		  rows are identical to each other, and {a_i + 1, a_{n + 1}} rows are
		  identical.

		  We store these two sets of rows into some kind of data structure. What
		  we'er trying to do here is that, after processing each column, we're
		  actually maintaining some sets of rows in this data stucture in such
		  a way that, in each set it's element rows are identical to each other
		  if we consider only the columns we have processed so far.

		  Now, let's move on the second column.

		- At column 2, we have to distribute a_2 '1's among the (n + 1) rows. We
		  first check if we have a set of rows in our data strcuture that contains
		  more than one rows.

		    * If we do, then select any two element rows r_1, r_2 from that set,
		      and set grid[r_1][2] = 1, grid[r_2][2] = 0. So now, these two rows
		      have become different from each other since one of them has '1' in
		      the current column, and the other has '0'. Thus, we have split this
		      set of rows into at least two parts.

		      Fill up the rest of the rows in the current column any way we wish.

		    * If we can't find such a set, then that means that all the (n + 1)
		      rows already are different from each other. So we can fill up the
		      rows in the current column in any way we wish, it doesn't matter.

		- So, if we keep processing the columns like this, at each column one of
		  the sets are having more than one element is split into at least two
		  more sets and thus the number of total sets increases by at least one.
		  Since initially we have 2 sets, and we'll do this procedure for the
		  (n - 1) columns following column 1, we'll have (n + 1) sets in the end,
		  each containing a single row. Which implies that the rows will all be
		  different from each other.
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

const int N = int(1e3) + 10;

int n;
int a[N];
int b[N][N];
vector<vector<int>> c[N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d", &n);
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);

	if (a[1] > 1) c[1].push_back(vector<int>());
	for (int row = 1; row <= a[1]; row++) {
		b[row][1] = 1;
		if (a[1] > 1) c[1].back().push_back(row);
	}

	if ((n + 1) - a[1] > 1) c[1].push_back(vector<int>());
	for (int row = a[1] + 1; row <= n + 1; row++) {
		b[row][1] = 0;
		if ((n + 1) - a[1] > 1) c[1].back().push_back(row);
	}

	for (int col = 2; col <= n; col++) {
		if (c[col - 1].empty()) {
			for (int row = 1; row <= a[col]; row++) b[row][col] = 1;
			continue;
		}

		int rem = a[col];
		b[c[col - 1][0][0]][col] = 0;
		b[c[col - 1][0][1]][col] = 1; rem--;

		for (int row = 1; rem; row++) {
			if (row != c[col - 1][0][0] && row != c[col - 1][0][1]) {
				b[row][col] = 1;
				rem--;
			}
		}

		for (vector<int> &_c : c[col - 1]) {
			vector<int> zero, one;

			for (int row : _c) {
				if (b[row][col] == 0) zero.push_back(row);
				else one.push_back(row);
			}

			if (zero.size() > 1) c[col].push_back(zero);
			if (one.size() > 1) c[col].push_back(one);
		}
	}

	int tot_rows = 0;
	for (int row = 1; row <= n + 1; row++) {
		tot_rows += accumulate(b[row] + 1, b[row] + n + 1, 0) > 0;
	}
	cout << tot_rows << "\n";

	for (int row = 1; row <= n + 1; row++) {
		int cnt = accumulate(b[row] + 1, b[row] + n + 1, 0);
		if (cnt > 0) {
			for (int col = 1; col <= n; col++) {
				cout << b[row][col];
			}
			cout << "\n";
		}
	}

	return 0;
}
