/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- First find all the connected components using dsu.

		- Now, we'll consider every possible placement of the (k x k) square and
		for each one, calculate the component size of that (k x k) square. In
		order to find that for a fixed (k x k) square, we need to sum the sizes of
		every different component that either touches the border of the (k x k)
		square or is inside the (k x k) square and finally add the number of 'X's
		inside the (k x k) square to the answer.

		- Finding the sum of the relevant components can be done by the sliding
		window technique and find the number of of 'X's can be found by
		pre-calculating 2D prefix sum.
*/

#include <bits/stdc++.h>

using namespace std;

#define debug(a) cerr << #a << ": " << a << endl

typedef long long ll;
typedef pair<int, int> pii;

#define x first
#define y second

const int N = 505;

int n, k;
char cell[N][N];
int pref[N][N];
int par[N * N], sz[N * N];
int freq[N * N], sum;

int parent(int v) {
	return par[v] == v ? v : (par[v] = parent(par[v]));
}
void merge(int x, int y) {
	x = parent(x);
	y = parent(y);

	if (x == y) return;

	if (sz[x] < sz[y]) swap(x, y);
	par[y] = x;
	sz[x] += sz[y];
}

void includeCol(int X, int Y, int len) {
	for (int atX = X, atY = Y, i = 0; i < len; atX++, i++) {
		if (atX <= 0 || n < atX || atY <= 0 || n < atY) continue;
		
		int currCell = (atX - 1) * n + atY;
		int parCell = parent(currCell);

		if (freq[parCell] == 0) sum += sz[parCell];
		freq[parCell]++;
	}
}
void removeCol(int X, int Y, int len) {
	for (int atX = X, atY = Y, i = 0; i < len; atX++, i++) {
		if (atX <= 0 || n < atX || atY <= 0 || n < atY) continue;

		int currCell = (atX - 1) * n + atY;
		int parCell = parent(currCell);

		freq[parCell]--;
		if (freq[parCell] == 0) sum -= sz[parCell];
	}
}

int calcRect(int x1, int y1, int x2, int y2) {
	return pref[x2][y2] - pref[x1 - 1][y2] - pref[x2][y1 - 1] + pref[x1 - 1][y1 - 1];
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif
	
	cin >> n >> k;

	for (int r = 1; r <= n; r++) {
		scanf("%s", cell[r] + 1);
		for (int c = 1; c <= n; c++) {
			if (cell[r][c] == '.') {
				int currCell = (r - 1) * n + c;

				par[currCell] = currCell;
				sz[currCell] = 1;

				if (r > 1 && cell[r - 1][c] == '.') merge(currCell, currCell - n);
				if (c > 1 && cell[r][c - 1] == '.') merge(currCell, currCell - 1);
			}

			pref[r][c] = pref[r - 1][c] + pref[r][c - 1] - pref[r - 1][c - 1] + (cell[r][c] == 'X');
		}
	}

	int ans = *max_element(sz + 1, sz + n * n + 1);

	for (int r = 1; r <= n - (k - 1); r++) {
		for (int c = 1; c < k; c++) includeCol(r - 1, c, k + 2);
		includeCol(r, k, k);

		for (int c = k; c <= n; c++) {
			removeCol(r, c - (k - 1) - 2, k);

			removeCol(r - 1, c - (k - 1) - 1, k + 2);
			includeCol(r, c - (k - 1) - 1, k);

			removeCol(r, c, k);
			includeCol(r - 1, c, k + 2);

			includeCol(r, c + 1, k);

			ans = max(ans, sum + calcRect(r, c - (k - 1), r + (k - 1), c));
		}
		for (int c = n; c >= n - (k - 1); c--) removeCol(r - 1, c, k + 2);
		removeCol(r, n - (k - 1) - 1, k);
	}

	cout << ans << endl;

	return 0;
}
