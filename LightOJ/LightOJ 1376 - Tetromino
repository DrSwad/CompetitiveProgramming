/*
	Author: Nayeemul Islam Swad

	Idea:
		- Let's first try to fill the first column with tetrominoes.
		  Notice that a tetrominoe from the first column can reach atmost
		  4 columns ahead.

		  So, we can consider all possible 3 x 4 grids, there are 2 ^ 12
		  such grids, and build a matrix of size (2 ^ 12) x (2 ^ 12) such
		  that the entry (i, j) represents the number of ways to fill up
		  the first column of grid i so that the next 3 columns form grid
		  j.

		  Now notice that, if we take this matrix to an exponent `n`,
		  then the (i, j) entry in the new matrix tells us the number of
		  ways to start with grid i, fill up the first `n` columns, and
		  end up with grid j.

		  If the row 0 and the column 0 of the initial matrix represent
		  the empty grid, then our final answer is just the (0, 0) entry
		  of the initial matrix taken to the exponent `n` given in the
		  test case.

		- Let's denote T = 2 ^ 12 as the number of 3 x 4 grid states.
		  The problem is that in the approach given above, our complexity
		  becomes O(T ^ 3 * log(N)) which is too slow even for a single
		  test case.

		  Instead of considering all the 3 x 4 grids, if we consider only
		  the ones that are reachable from the empty grid, then it turns
		  out that this number drastically reduces to only 86. For T = 86,
		  O(T ^ 3 * log(N)) passes in about 1.77s.

		  But we can further improve by eliminating the 3 x 4 grids from
		  where we cannot reach the empty grid back again, which reduces
		  T to 68 and passes in about 0.86s.
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

const int MOD = int(1e9) + 7;

struct matrix {
	ll cell[100][100];
	int dim;
	matrix() {};
	matrix(int d) {
		dim = d;
		for (int i = 0; i < dim; i++)
			for (int j = 0; j < dim; j++)
				cell[i][j] = 0;
	}
	matrix operator * (const matrix &mul) {
		matrix ret = matrix(dim);
		for (int i = 0; i < dim; i++) {
			for (int j = 0; j < dim; j++) {
				for (int k = 0; k < dim; k++) {
					(ret.cell[i][j] += cell[i][k] * mul.cell[k][j]) %= MOD;
				}
			}
		}
		return ret;
	}
	matrix operator ^ (ll p) {
		matrix ret = matrix(dim);
		for (int i = 0; i < dim; i++) ret.cell[i][i] = 1;
		for (matrix m = *this; p; p >>= 1) {
			if (p & 1) ret = ret * m;
			m = m * m;
		}
		return ret ;
	}
};

vector<pii> tets; // Vector of pairs (`mask`, `height`) where `mask` is a
				  // 16-bit integer corresponding to a tetromino and
				  // representing the 16x16 grid as:
				  // 0 4  8 12
				  // 1 5  9 13
				  // 2 6 10 14
				  // 3 7 11 15
				  // The tetrominoes must be positioned at top-left,
				  // so for example, one of the tetrominoes is
				  // 0 #  8 12         0 4  8 12
				  // 1 #  9 13 but not 1 #  9 13
				  // # # 10 14         2 # 10 14
				  // 3 7 11 15         # # 11 15
				  // And `height` is the height of that tetromino, for
				  // example `height` of the above tetromino is 2.

/*
	Finds all the 19 tetrominoes and stores them in the `tets` vector
*/
void buildTets() {
	for (int mask = 0; mask < 1 << 16; mask++) {
		if (__builtin_popcount(mask) != 4) continue; // Exactly 4 tiles

		bool flag;

		// Tetromino should be at top-left
		flag = false; // Check left column first
		for (int i = 0; i < 4; i++) if (mask & 1 << i) flag = true;
		if (!flag) continue;

		flag = false; // Now check top row
		for (int i = 0; i < 4; i++) if (mask & 1 << i * 4) flag = true;
		if (!flag) continue;

		// Tiles need to be connected
		vector<int> bits;
		for (int i = 0; i < 16; i++) if (mask & 1 << i) bits.push_back(i);
		vector<bool> vis(4, 0);
		vis[0] = 1;
		for (int t = 0; t < 3; t++) {
			for (int i = 0; i < 4; i++) if (vis[i]) {
				for (int j = 0; j < 4; j++) {
					if (bits[i] >= 4 && bits[i] - 4 == bits[j]) vis[j] = 1;
					if (bits[i] < 12 && bits[i] + 4 == bits[j]) vis[j] = 1;
					if (bits[i] % 4 < 3 && bits[i] + 1 == bits[j]) vis[j] = 1;
					if (bits[i] % 4 > 0 && bits[i] - 1 == bits[j]) vis[j] = 1;
				}
			}
		}
		if (accumulate(vis.begin(), vis.end(), 0) != 4) continue;

		// Find it's `height`
		int height;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (mask & 1 << (4 * j + i)) height = i;

		tets.push_back({mask, height});
	}
}

bool vis[1 << 16]; // Asserts whether the `mask` has been visited by dfs

map<int, int> states; // Used to mark all the 12-bit integers `mask` so
					  // (`mask` << 4 | 15) is reachable from 0. Apparently
					  // there're only 86 such `mask`s. And if we further
					  // consider only the `mask`s such that we can go from
					  // (`mask` << 4 | 15) to 0, then as it seems, there
					  // are only 68 possibilities.

vector<vector<int>> ways; // Builds a 68 x 68 vector where the (i, j) entry
						  // stores the number of ways we can reach
						  // `mask_j` from `mask_i` by filling in the first
						  // column of `mask_i` using tetrominoes.

/*
	Here `mask` is a 16-bit integer represting a 16x16 grid as our current
	position.

	If `count_ways` is set to false, then it tries to fill in the first
	empty cell in the first column of `mask` using all the 19 tetrominoes
	and goes to the next states. But if all the cells in the first column
	are already filled up, then (`mask` >> 4) is a valid state for out
	`state` map above, so we store it in the map and `mask` is right
	shifted by 4-bits to remove the first column.

	If `count_ways` is set to true, then `from` is the mask we started the
	dfs from and we're trying to find all the 12-bit `mask`s so that the
	by somehow filling the first column of `from` using tetrominoes, the
	remaining 3 columns of `from` is identical to `mask`. So, during the
	dfs, whenever we see that the first column is filled, we check if the
	remaining 3 columns represent a valid state, if it does then we
	increase the corresponding entry in the `ways` vector, and we backtrack.
*/
void dfs(int mask, int from = 0, bool count_ways = false) {
	if (vis[mask] && !count_ways) return;
	vis[mask] = true;

	if ((mask & 15) == 15) {
		mask >>= 4;
		if (count_ways) {
			if (states.find(mask) != states.end()) {
				ways[states[from]][states[mask]]++;
			}
			return;
		}
		states[mask] = 1;
	}

	for (int i = 0; i < tets.size(); i++) {
		int z = __builtin_ctz(tets[i].x);
		int st = __builtin_ctz(mask + 1) - z;
		if (st < 0 || st + tets[i].y >= 4 || (mask & tets[i].x << st) != 0) continue;
		dfs(mask | tets[i].x << st, from, count_ways);
	}
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	buildTets();

	dfs(0); // Finds all reachable states from 0

	// Now, remove the states from where we cannot reach 0 again
	vector<int> to_remove;
	for (const pii &p : states) {
		int state = (~p.x) & ((1 << 12) - 1);
		int _state = 0;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 3; j++)
				if (state & 1 << (4 * j + i))
					_state |= 1 << (4 * (2 - j) + i);
		while ((_state & 15) == 15) _state >>= 4;
		if (states.find(_state) == states.end()) to_remove.push_back(p.x);
	}
	for (int mask : to_remove) states.erase(mask);

	// Initialize the `ways` vector
	{int i = 0;
	for (const pii &p : states) states[p.x] = i++;}
	int total_states = states.size();
	ways.resize(total_states, vector<int>(total_states));
	for (const pii &p : states) dfs(p.x, p.x, true);

	// Represent the ways vector as a 68 x 68 matrix
	matrix mat = matrix(total_states);
	for (int i = 0; i < total_states; i++) {
		for (int j = 0; j < total_states; j++) {
			mat.cell[i][j] = ways[i][j];
		}
	}

	int t;
	cin >> t;

	map<int, ll> answered;
	for (int cs = 1; cs <= t; cs++) {
		int n;
		scanf("%d", &n);
		// Our answer should be the number of ways to start from 0 and
		// come back to 0 after filling in all the s`n` columns.
		ll ans = answered.find(n) != answered.end() ? answered[n] : (mat ^ n).cell[0][0];
		printf("Case %d: %lld\n", cs, answered[n] = ans);
	}

	return 0;
}
